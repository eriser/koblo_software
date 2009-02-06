
#include "KSOS.h"

#ifdef _DEBUG
const tint32 iMaxToProcess = 0x4000;
#else _DEBUG
const tint32 iMaxToProcess = 0x10000;
#endif


CExportClipTask::CExportClipTask( const tchar* pszClipName, tuint64 uiStartIx, tuint64 uiDuration)
{
	sScreenName = pszClipName;

	constructor_helper(ac::geAudioCodecUndefined, uiStartIx, uiDuration);
} // constructor

CExportClipTask::CExportClipTask()
{
	constructor_helper(ac::geAudioCodecUndefined, 0, (tuint64)-1);
} // constructor


void CExportClipTask::constructor_helper(ac::EAudioCodec eCodecDst, tuint64 uiStartIx, tuint64 uiDuration)
{
	muiStartIx = uiStartIx;
	muiDuration = uiDuration;
	
	mpcSilence = NULL;
	
	iFiles = 0;
	pfWaveL = IFile::Create();
	pfWaveR = IFile::Create();
	bIsDecompressed = false;
	
	eCodecOfOriginal = ac::geAudioCodecUndefined;
	eQualityOfOriginal = eQualityOfCompressed = ac::geQualityUnknown;
	iChannels = iSampleRate = 0;
	
	bDoEncode = bDoCopy = bSuccess = false;
	pConcatenateNextTask = NULL;
	
	miActionOrder = geExportClip_Before;
	mpfDst = NULL;
	miChannelsDst = -1;
	mpEncoder = NULL;
	meCodecDst = eCodecDst;
	meQualityDst = ac::geQualityUnknown;
	muiCurrEncodeIx = uiStartIx;
	muiSamplesNeeded = uiDuration;
} // constructor_helper


tbool CExportClipTask::Init(const CTake_Data* pTake_Input, const tchar* pszFilePathDst, ac::EAudioCodec eCodecDst, ac::EQuality eQuality, tuint64 uiStartIx /*= 0*/, tuint64 uiDuration /*= (tuint64)-1*/)
{
	//mFileItem = *pTake_Input;
	sScreenName = pTake_Input->Screen_Name();
	std::string sWaveL = pTake_Input->Left_Wave_File_Path();
	std::string sWaveR = pTake_Input->Right_Wave_File_Path();
	//tbool bStereo = pTake_Input->Stereo();
	std::string sMode = pTake_Input->Mode();
	tbool bStereo = (stricmp(sMode.c_str(), "stereo") == 0);
	iChannels = (bStereo) ? 2 : 1;
	if (!pfWaveL->Open(sWaveL.c_str(), IFile::FileRead)) {
		msExtendedError = "Can't read-open file:\n  " + sWaveL;
		return false;
	}
	if ((bStereo) && (!pfWaveR->Open(sWaveL.c_str(), IFile::FileRead))) {
		msExtendedError = "Can't read-open file:\n  " + sWaveR;
		return false;
	}
	sOut = pszFilePathDst;
	meCodecDst = eCodecDst;
	meQualityDst = eQuality;
	bDoEncode = true;
	bDoCopy = false;
	bSuccess = false;
	muiStartIx = uiStartIx;
	muiDuration = uiDuration;
	return true;
} // Init


CExportClipTask::~CExportClipTask()
{
	tbool bAbort = false;
	if (mpEncoder) {
		bAbort = true;
		mpEncoder->Destroy();
		mpEncoder = NULL;
	}

	if (bAbort) {
		if (mpfDst) {
			DeleteDstFile(mpfDst);
			mpfDst->Destroy();
			mpfDst = NULL;
		}
	}

	pfWaveL->Destroy();
	pfWaveR->Destroy();
} // destructor


void CExportClipTask::Destroy()
{
	delete dynamic_cast<CExportClipTask*>(this);
} // Destroy


tbool CExportClipTask::DoWork()
{
	tbool bSuccess = false;

	switch (miActionOrder) {
		case geExportClip_Before:
			{
				sOut = sDestFolder + sDestNameAndExt;

				if (bDoEncode) {
					mpEncoder = ac::IEncoder::Create(meCodecDst);

					switch (meCodecDst)
					{
						case ac::geAudioCodecWave:
							msProgress = "Exporting";
							break;
						case ac::geAudioCodecLame:
							msProgress = "Making mp3";
							break;
						case ac::geAudioCodecVorbis:
							msProgress = "Making ogg";
							break;
						default:
							// Huh?
							msProgress = "Making ???";
							break;
					}
				}
				if (bDoCopy)
					msProgress = "Copying";
				if (IsSilence()) {
					msProgress += " silent bit";
				}
				else {
					msProgress += std::string(" '") + sScreenName + "'";
					muiProgressIx = 0;
					muiProgressTarget = muiDuration - muiStartIx;
				}

				miActionOrder++;
				bSuccess = (mpEncoder != NULL); //true;
			}
			break;

		case geExportClip_Action:
			{
				if (bDoEncode) {
					bSuccess = DoEncode();
				}
				else if (bDoCopy) {
					bSuccess = DoCopy();
				}
			}
			break;

		case geExportClip_After:
			{
				msProgress = "Export done";
				muiProgressIx = muiProgressTarget = 1;

				miActionOrder++;
				bSuccess = true;
			}
			break;

		default:
			// Why are we here?
			break;

	}

	if (!bSuccess) {
		miActionOrder = geExportClip_Done;
	}
	return bSuccess;
} // DoWork


tbool CExportClipTask::DoEncode_FirstTimeHere()
{
	tbool bError = false;

	mpfDst = IFile::Create();
	if ((mpfDst == NULL) || (!mpfDst->Open(sOut.c_str(), IFile::FileCreate))) {
		msExtendedError = std::string("Unable to create file:\n  ") + sOut;
		bError = true;
	}
	else {
		if (mpEncoder) {
			if (miChannelsDst < 1)
				miChannelsDst = iChannels;
			if (!mpEncoder->Init(mpfDst, meQualityDst, miChannelsDst)) {
				tchar pszErr[1024];
				mpEncoder->GetErrMsg(pszErr, 1024, true);
				msExtendedError = pszErr;
				bError = true;
			}
			else {
				// Maybe decrease quality info due to previous encoding
				// Maybe test quality for several inputs (when concatenating)
				// This operation needs to be finished before writing anything to encoder
				// so we hurry through all inputs
				CExportClipTask* p = this;
				for ( ; (p != NULL); p = p->pConcatenateNextTask) {
					if (!p->IsSilence()) {
						mpEncoder->SetQualityOfOriginal(p->eQualityOfOriginal);
					}
				}

				// Go to position 0 (and kill effect tail)
				gpApplication->Playback_InProgressTask(0);
			}
		}
	}

	return !bError;
} // DoEncode_FirstTimeHere


tbool CExportClipTask::DoEncode()
{
	tbool bError = false;

	if (mpfDst == NULL) {
		// First time here
		bError = !DoEncode_FirstTimeHere();
	}

	if (!bError) {
		tint64 iActuallyProcessed = 0;

		// Maybe prepare a chunk of silence
		if (IsSilence() && (mpcSilence == NULL)) {
			tint32 iSilence24bitSize = iMaxToProcess * (24 / 8);
			mpcSilence = new tchar[iSilence24bitSize];
			memset(mpcSilence, '\0', iSilence24bitSize);
		}

		if (muiSamplesNeeded > 0) {
			// How many samples now?
			tint32 iWantsToProcess = iMaxToProcess;
			if (muiSamplesNeeded < iWantsToProcess)
				iWantsToProcess = (tint32)muiSamplesNeeded;

			tbool bPartSuccess = false;
			switch (iFiles) {
				case 0: // silence
					{
						tint64 iOF_Dummy = 0;
						bPartSuccess = mpEncoder->SetRawMode(true, 1, false, 24, iSampleRate);
						if (bPartSuccess) {
							tint32 iBytes = iWantsToProcess * (24 / 8);
							bPartSuccess = mpEncoder->ProcessRaw(mpcSilence, NULL, iBytes, &iOF_Dummy);
							mpEncoder->SetRawMode(false, 0, false, 0, 0);
							iActuallyProcessed = iWantsToProcess;
						}
					}
					break;
				case 1: // mono
					bPartSuccess = mpEncoder->Process(
						pfWaveL,
						(tint32)muiCurrEncodeIx, iWantsToProcess, &iActuallyProcessed);
					break;
				default: // stereo
					bPartSuccess = mpEncoder->Process(
						pfWaveL, pfWaveR,
						(tint32)muiCurrEncodeIx, iWantsToProcess, &iActuallyProcessed);
					break;
			}
			if (bPartSuccess) {
				if (muiSamplesNeeded == (tuint64)-1) {
					// Figure out how big the file really is (to show progress correctly)
					muiSamplesNeeded = mpEncoder->miInputSamples_IncludingSkipped;
					muiProgressTarget = muiSamplesNeeded;
				}

				muiProgressIx += iActuallyProcessed;
				muiCurrEncodeIx += iActuallyProcessed;
				muiSamplesNeeded -= iActuallyProcessed;
			}
			else {
				tchar pszErr[1024];
				mpEncoder->GetErrMsg(pszErr, 1024, true);
				msExtendedError += pszErr;
				iActuallyProcessed = 0;
				bError = true;
			}
		}
	}

	if ((!bError) && (muiSamplesNeeded == 0)) {
		// We're done with this clip / silent bit
		
		// Prepare to advance to next task
		miActionOrder++;

		// Test if we should concatenate another file
		if (pConcatenateNextTask) {
			// Transfer encoder and other stuff to next clip (we don't close them)
			pConcatenateNextTask->mpEncoder = mpEncoder;
			mpEncoder = NULL;
			pConcatenateNextTask->mpfDst = mpfDst;
			mpfDst = NULL;
			pConcatenateNextTask->sOut = sOut;
		}
		else {
			// We're done - close encoder and other stuff
			if (!mpEncoder->Finalize()) {
				// Encoder finalize failed!
				tchar pszErr[1024];
				mpEncoder->GetErrMsg(pszErr, 1024, true);
				msExtendedError = pszErr;
				bError = true;
			}
			else {
				mpEncoder->Destroy();
				mpEncoder = NULL;

				mpfDst->Destroy();
				mpfDst = NULL;
			}
		}
	}

	return !bError;
} // DoEncode


tbool CExportClipTask::DoCopy()
{
	tbool bError = false;

	IFile::DeleteFile((sDestFolder + sDestNameAndExt).c_str());
	if (!IFile::CopyFile(
		sDestFolder.c_str(),
		gpApplication->GetProjDir_Clips().c_str(),
		sDestNameAndExt.c_str())
	) {
		msExtendedError = std::string("Unable to copy compressed file for clip: ") + sScreenName;
		bError = true;
	}

	miActionOrder++;
	return !bError;
} // DoCopy


tbool CExportClipTask::IsDone()
{
	return (miActionOrder >= geExportClip_Done);
} // IsDone
