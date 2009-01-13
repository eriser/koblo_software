
#include "ineInternalOS.h"

CDownloader::CDownloader()
{
	Constructor_OSSpecific();
} // constructor


CDownloader::~CDownloader()
{
	Abort();
	WipeParams();
	CloseConnection();
	//
	Destructor_OSSpecific();
} // destructor


IDownloader* IDownloader::Create()
{
	return dynamic_cast<IDownloader*>(new CDownloader());
} // Create


void CDownloader::Destroy()
{
	delete dynamic_cast<CDownloader*>(this);
} // Destroy


tbool CDownloader::Init(const tchar* pszHost, const tchar* pszPage, tint32 iPort /*= 80*/, const tchar* pszUser /*= NULL*/, const tchar* pszPassword /*= NULL*/, tint32 iTimeOutSecs /*= 10*/)
{
	Abort();

	return CUpAndDownloader_Common::Init(pszHost, pszPage, iPort, pszUser, pszPassword, iTimeOutSecs);
} // Init


tbool CDownloader::SetDesiredMIMEType(E_MIME_Type eType)
{
	return CUpAndDownloader_Common::SetMIMEType(eType);
} // SetDesiredMIMEType


tbool CDownloader::SetSpecificVerb(EVerbType eVerb)
{
	return CUpAndDownloader_Common::SetSpecificVerb(eVerb, false);
} // SetSpecificVerb


tbool CDownloader::AddParam(const tchar* pszParamName, const tchar* pcParamData, tint32 iParamDataLen)
{
	return CUpAndDownloader_Common::AddParam(pszParamName, pcParamData, iParamDataLen);
} // AddParam


tbool CDownloader::OpenConnection()
{
	return OpenConnection_OSSpecific();
} // OpenConnection


void CDownloader::CloseConnection()
{
	CloseConnection_OSSpecific();
} // CloseConnection


tbool CDownloader::DownloadPortion(tchar* pszBuffer, tint32 iBufferSize, tint32* piPortionSize, tuint64* puiTotalSize)
{
	*piPortionSize = 0;
	
	if (IsFailed()) {
		//SetError("Previous error");
		return false;
	}
	
	if (!IsInitialized()) {
		SetError("Not initialized");
		return false;
	}
	
	tbool bFirstTime = (!IsTransfering());
	SetIsTransfering();
	if (bFirstTime) {
		CAutoLock Lock(mMutex_Connection);
		
		CloseConnection();
		if (!AssembleParams()) return false;
		if (!OpenConnection()) {
			CloseConnection();
			return false;
		}
		// We're alive
		RefreshAlive();
	}
	
	return DownloadPortion_OSSpecific(pszBuffer, iBufferSize, piPortionSize, puiTotalSize);
} // DownloadPortion


tbool CDownloader::Abort()
{
	if (IsTransfering()) {
		CAutoLock Lock(mMutex_Connection);
		
		CloseConnection();
		SetIsDone();
	}
	
	WipeParams();
	
	return true;
} // Abort


tbool CDownloader::IsDone()
{
	return CUpAndDownloader_Common::IsDone();
} // IsDone


tbool CDownloader::IsFailed()
{
	return CUpAndDownloader_Common::IsFailed();
} // IsFailed


tbool CDownloader::GetLatestError(tchar* pszErrBuff, tint32 iErrBuffSize)
{
	return CUpAndDownloader_Common::GetLatestError(pszErrBuff, iErrBuffSize);
}
