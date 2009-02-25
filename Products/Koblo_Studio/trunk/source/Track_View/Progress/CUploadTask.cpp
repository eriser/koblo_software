
#include "KSOS.h"


// Private enum for state machine
enum EUploadOrder {
	geUpload_Uninitialized,

	geUpload_Branch_PreVerify_Before,
	geUpload_Branch_PreVerify_Action,
	geUpload_Branch_PreVerify_After,

	geUpload_NewProject_Before,
	geUpload_NewProject_Action,
	geUpload_NewProject_After,

	geUpload_NewBranch_Before,
	geUpload_NewBranch_Action,
	geUpload_NewBranch_After,

	geUpload_Take_PreVerify_Before,
	geUpload_Take_PreVerify_Action,
	geUpload_Take_PreVerify_After,

	geUpload_Take_Upload_Before,
	geUpload_Take_Upload_Action,
	geUpload_Take_Upload_After,

	geUpload_PresetSettings_PreVerify_Before,
	geUpload_PresetSettings_PreVerify_Action,
	geUpload_PresetSettings_PreVerify_After,

	geUpload_PresetSettings_Upload_Before,
	geUpload_PresetSettings_Upload_Action,
	geUpload_PresetSettings_Upload_After,

	geUpload_Commit_PreVerify_Before,
	geUpload_Commit_PreVerify_Action,
	geUpload_Commit_PreVerify_After,

	geUpload_Commit_Upload_Before,
	geUpload_Commit_Upload_Action,
	geUpload_Commit_Upload_After,

	geUpload_Done
}; // EUploadOrder


CUploadTask::CUploadTask()
{
	miActionOrder = 0;

	mpfileCommitXML = NULL;
	mpfilePresetData = NULL;

	mpDownloader_VerifyMisc = NULL;
	mpfileReply_VerifyMisc = NULL;

	mpDownloader_VerifySample = NULL;
	mpfileReply_VerifySample = NULL;

	mpDownloader_VerifyTake = NULL;
	mpfileReply_VerifyTake = NULL;

	mpUploader = NULL;
	mpfileReply_Uploader = NULL;

	mpTakeCurr = NULL;
	mpfileMp3 = NULL;
	mpfileOgg = NULL;
} // constructor


CUploadTask::~CUploadTask()
{
	if (mpUploader) {
		mpUploader->Destroy();
		mpUploader = NULL;
	}
	if (mpfileReply_Uploader) {
		mpfileReply_Uploader->Destroy();
		mpfileReply_Uploader = NULL;
	}
	//
	if (mpDownloader_VerifySample) {
		mpDownloader_VerifySample->Destroy();
		mpDownloader_VerifySample = NULL;
	}
	if (mpfileReply_VerifySample) {
		mpfileReply_VerifySample->Destroy();
		mpfileReply_VerifySample = NULL;
	}
	//
	if (mpDownloader_VerifyTake) {
		mpDownloader_VerifyTake->Destroy();
		mpDownloader_VerifyTake = NULL;
	}
	if (mpfileReply_VerifyTake) {
		mpfileReply_VerifyTake->Destroy();
		mpfileReply_VerifyTake = NULL;
	}
	//
	if (mpDownloader_VerifyMisc) {
		mpDownloader_VerifyMisc->Destroy();
		mpDownloader_VerifyMisc = NULL;
	}
	if (mpfileReply_VerifyMisc) {
		mpfileReply_VerifyMisc->Destroy();
		mpfileReply_VerifyMisc = NULL;
	}
	//
	std::list<CTake_Data*>::iterator it = mlistpTakes.begin();
	for ( ; it != mlistpTakes.end(); it++) {
		CTake_Data* pTake = *it;
		delete pTake;
		mlistpTakes.pop_back();
	}
	if (mpTakeCurr) {
		delete mpTakeCurr;
		mpTakeCurr = NULL;
	}
	if (mpfileMp3) {
		mpfileMp3->Destroy();
		mpfileMp3 = NULL;
	}
	if (mpfileOgg) {
		mpfileOgg->Destroy();
		mpfileOgg = NULL;
	}
	if (mpfileCommitXML) {
		mpfileCommitXML->Destroy();
		mpfileCommitXML = NULL;
	}
	if (mpfilePresetData) {
		mpfilePresetData->Destroy();
		mpfilePresetData = NULL;
	}
} // destructor


void CUploadTask::Destroy()
{
	delete dynamic_cast<CUploadTask*>(this);
} // Destroy


tbool CUploadTask::Init_NewProject(
								   const tchar* pszUser, 
								   const tchar* pszPassword,
								   const tchar* pszProjUUID, 
								   const tchar* pszProjName, 
								   const tchar* pszProjDesc, 
								   const tchar* pszProjLicenseCode,
								   const tchar* pszBranchUUID,
								   const tchar* pszCommitUUID, 
								   const tchar* pszfileCommit,
								   std::list<CTake_Data*>* plistpTakes)
{
	if (miActionOrder != 0) {
		msExtendedError = "Double initialization";
		return false;
	}

	msUser					= pszUser;
	msPassword				= pszPassword;

	msProjectUUID			= pszProjUUID;
	msProjectName			= pszProjName;
	msProjectDescription	= pszProjDesc;
	msProjectLicense		= pszProjLicenseCode;

	// not used: msBranchUUID_Old		= pBranchUUID_Original->Get_UUID();
	msBranchUUID			= pszBranchUUID;
	msBranchName			= "Trunk";
	// not used: msBranchDescription		= pszBranchDesc;
	// not used: msBranchLicense			= pszBranchLicenseCode;

	msCommitUUID			= pszCommitUUID;
	msFileCommitXML			= pszfileCommit;
	msCommitDescription		= "Initial commit of project xml";

	if (!Init_Helper(plistpTakes))
		return false;

	miActionOrder					= geUpload_Branch_PreVerify_Before;
	miAction_Branch_PreVerify_WhereToGoNow	= geUpload_NewProject_Before;
	mbRequiresWriteAccess = true;
	return true;
} // Init_NewProject


tbool CUploadTask::Init_NewBranch(
								  const tchar* pszUser, const tchar* pszPassword,
								  const tchar* pszProjUUID,
								  const tchar* pszBranchUUID_Original,
								  const tchar* pszBranchUUID, 
								  const tchar* pszBranchName, 
								  const tchar* pszBranchDesc, 
								  const tchar* pszBranchLicenseCode,
								  const tchar* pszCommitUUID, 
								  const tchar* pszfileCommit,
								  std::list<CTake_Data*>* plistpTakes)
{
	if (miActionOrder != 0) {
		msExtendedError = "Double initialization";
		return false;
	}

	msUser					= pszUser;
	msPassword				= pszPassword;

	msProjectUUID			= pszProjUUID;
	// not used: msProjectName			= pszProjName;
	// not used: msProjectDescription	= pszProjDesc;
	// not used: msProjectLicense		= pszProjLicenseCode;

	msBranchUUID_Old		= pszBranchUUID_Original;
	msBranchUUID			= pszBranchUUID;
	msBranchName			= pszBranchName;
	msBranchDescription		= pszBranchDesc;
	msBranchLicense			= pszBranchLicenseCode;

	msCommitUUID			= pszCommitUUID;
	msFileCommitXML			= pszfileCommit;
	msCommitDescription		= "Initial commit of project xml";

	if (!Init_Helper(plistpTakes))
		return false;

	miActionOrder					= geUpload_Branch_PreVerify_Before;
	miAction_Branch_PreVerify_WhereToGoNow	= geUpload_NewBranch_Before;
	mbRequiresWriteAccess = false;
	return true;
} // Init_NewBranch


tbool CUploadTask::Init_Commit(
							   const tchar* pszUser, 
							   const tchar* pszPassword,
							   const tchar* pszProjUUID,
							   const tchar* pszBranchUUID,
							   const tchar* pszCommitUUID, 
							   const tchar* pszfileCommit,
							   const tchar* pszOnline_fileCommit,
							   const tchar* pszCommitDesc,
							   std::list<CTake_Data*>* plistpTakes)
{
	if (miActionOrder != 0) {
		msExtendedError = "Double initialization";
		return false;
	}

	msUser					= pszUser;
	msPassword				= pszPassword;

	msProjectUUID			= pszProjUUID;
	// not used: msProjectName			= pszProjName;
	// not used: msProjectDescription	= pszProjDesc;
	// not used: msProjectLicense		= pszProjLicenseCode;

	// not used: msBranchUUID_Old		= pBranchUUID_Original->Get_UUID();
	msBranchUUID						= pszBranchUUID;
	// not used: msBranchName			= pszBranchName;
	// not used: msBranchDescription		= pszBranchDesc;
	// not used: msBranchLicense			= pszBranchLicenseCode;

	msCommitUUID			= pszCommitUUID;
	msFileCommitXML			= pszfileCommit;
	msOnline_FileCommitXML	= pszOnline_fileCommit;
	
//	printf("++++++++++++++++++++\n %s \n++++++++++++++++++++++\n",msFileCommitXML.c_str() );
//	printf("++++++++++++++++++++\n %s \n++++++++++++++++++++++\n",msOnline_FileCommitXML.c_str() );
	
	msCommitDescription		= pszCommitDesc;

	if (!Init_Helper(plistpTakes))
		return false;

	miActionOrder = geUpload_Branch_PreVerify_Before;
	if (mlistpTakes.size() > 0) {
		// Upload takes and then commit preset data and project xml
		miAction_Branch_PreVerify_WhereToGoNow = geUpload_Take_PreVerify_Before;
	}
	else {
		// No takes - go directly to commit preset data and project xml
		miAction_Branch_PreVerify_WhereToGoNow = geUpload_PresetSettings_PreVerify_Before;
	}
	mbRequiresWriteAccess = true;
	return true;
} // Init_Commit


tbool CUploadTask::Add_PresetData(const tchar* pszPresetDataUUID, const tchar* pszFilePresetData)
{
	//TODO: Some sort of checking that we're ok to change this now
	// if (bad) return false;

	msPresetDataUUID = pszPresetDataUUID;
	mpfilePresetData = IFile::Create();
	if (!mpfilePresetData->Open(pszFilePresetData, IFile::FileRead)) {
		msExtendedError = "Unable to read-open file:\n ";
		msExtendedError += pszFilePresetData;
		msPresetDataUUID = "";
		mpfilePresetData = NULL;
		return false;
	}

	// Success
	return true;
} // Add_PresetData


tbool CUploadTask::Init_Helper(std::list<CTake_Data*>* plistpTakes)
{
	// Note: Unfortunately we NEED TO COPY the takes!!!
	// Can't use the originals as that won't be thread safe
	if (plistpTakes) {
		std::list<CTake_Data*>::iterator it = plistpTakes->begin();
		for ( ; it != plistpTakes->end(); it++) {
			CTake_Data* pTake = *it;
			CTake_Data* pTake_Copy = new CTake_Data(*pTake);
			mlistpTakes.push_back(pTake_Copy);
		}
	}

	if (mpUploader == NULL) {
		mpUploader = ine::IUploader::Create();
	}

	return (mpUploader != NULL);
} // Init_Helper


tbool CUploadTask::DoWork()
{
	tbool bSuccess = false;
	tbool bActionDone = false;
	tbool bAlreadyThere = false;
	tbool bNoMoreTakes = false;

	switch (miActionOrder) {
		case geUpload_Uninitialized:
			msExtendedError = "Internal error: Uninitialized";
			break;

		case geUpload_Branch_PreVerify_Before:
			bSuccess = DoBranch_PreVerify_Before();
			miActionOrder++;
			break;
		case geUpload_Branch_PreVerify_Action:
			bSuccess = DoBranch_PreVerify_Action(&bActionDone);
			if (bActionDone) miActionOrder++;
			break;
		case geUpload_Branch_PreVerify_After:
			bSuccess = DoBranch_PreVerify_After(&bAlreadyThere, &bNoMoreTakes);
			// Where to go?
			if (!bAlreadyThere) {
				// Brand new project or branch - go ahead
				miActionOrder = miAction_Branch_PreVerify_WhereToGoNow;
			}
			else {
				// Skip create project/branch
				if (bNoMoreTakes) {
					// No takes - go to commit preset data and project xml
					miActionOrder = geUpload_PresetSettings_PreVerify_Before;
				}
				else {
					// Go to upload takes and then commit project xml
					miActionOrder = geUpload_Take_PreVerify_Before;
				}
			}
			break;

		case geUpload_NewProject_Before:
			bSuccess = DoNewProject_Before();
			miActionOrder++;
			break;
		case geUpload_NewProject_Action:
			bSuccess = DoNewProject_Action(&bActionDone);
			if (bActionDone) miActionOrder++;
			break;
		case geUpload_NewProject_After:
			bSuccess = DoNewProject_After(&bNoMoreTakes);
			// Where to go?
			if (bNoMoreTakes) {
				// Jump to commit preset data
				miActionOrder = geUpload_PresetSettings_PreVerify_Before;
			}
			else {
				// Upload takes
				miActionOrder = geUpload_Take_PreVerify_Before;
			}
			break;

		case geUpload_NewBranch_Before:
			bSuccess = DoNewBranch_Before();
			miActionOrder++;
			break;
		case geUpload_NewBranch_Action:
			bSuccess = DoNewBranch_Action(&bActionDone);
			if (bActionDone) miActionOrder++;
			break;
		case geUpload_NewBranch_After:
			bSuccess = DoNewBranch_After(&bNoMoreTakes);
			// Where to go?
			if (bNoMoreTakes) {
				// Jump to commit preset data
				miActionOrder = geUpload_PresetSettings_PreVerify_Before;
			}
			else {
				// Upload takes
				miActionOrder = geUpload_Take_PreVerify_Before;
			}
			break;

		case geUpload_Take_PreVerify_Before:
			bSuccess = DoTake_PreVerify_Before();
			miActionOrder++;
			break;
		case geUpload_Take_PreVerify_Action:
			bSuccess = DoTake_PreVerify_Action(&bActionDone);
			if (bActionDone) miActionOrder++;
			break;
		case geUpload_Take_PreVerify_After:
			bSuccess = DoTake_PreVerify_After(&bAlreadyThere, &bNoMoreTakes);
			// Where to go?
			if (!bAlreadyThere) {
				// This take is brand new - continue upload
				miActionOrder++;
			}
			else {
				// Take is already there
				if (bNoMoreTakes) {
					// No more takes - go to commit preset data
					miActionOrder = geUpload_PresetSettings_PreVerify_Before;
				}
				else {
					// Go to next take
					miActionOrder = geUpload_Take_PreVerify_Before;
				}
			}
			break;

		case geUpload_Take_Upload_Before:
			bSuccess = DoTake_Upload_Before();
			miActionOrder++;
			break;
		case geUpload_Take_Upload_Action:
			bSuccess = DoTake_Upload_Action(&bActionDone);
			if (bActionDone) miActionOrder++;
			break;
		case geUpload_Take_Upload_After:
			bSuccess = DoTake_Upload_After(&bNoMoreTakes);
			// Where to go?
			if (bNoMoreTakes) {
				// Done - go to commit project xml
				miActionOrder++;
			}
			else {
				// Go to next take
				miActionOrder = geUpload_Take_PreVerify_Before;
			}
			break;

		case geUpload_PresetSettings_PreVerify_Before:
			bSuccess = DoPresetSettings_PreVerify_Before(&bAlreadyThere);
			// Where to go?
			if (!bAlreadyThere) {
				// Brand new preset data - go ahead
				miActionOrder++;
			}
			else {
				// Skip upload preset data - go to upload project xml
				miActionOrder = geUpload_Commit_PreVerify_Before;
			}
			break;
		case geUpload_PresetSettings_PreVerify_Action:
			bSuccess = DoPresetSettings_PreVerify_Action(&bActionDone);
			if (bActionDone) miActionOrder++;
			break;
		case geUpload_PresetSettings_PreVerify_After:
			bSuccess = DoPresetSettings_PreVerify_After(&bAlreadyThere);
			// Where to go?
			if (!bAlreadyThere) {
				// Brand new preset data - go ahead
				miActionOrder++;
			}
			else {
				// Skip upload preset data - go to upload project xml
				miActionOrder = geUpload_Commit_PreVerify_Before;
			}
			break;

		case geUpload_PresetSettings_Upload_Before:
			bSuccess = DoPresetSettings_Upload_Before();
			miActionOrder++;
			break;
		case geUpload_PresetSettings_Upload_Action:
			bSuccess = DoPresetSettings_Upload_Action(&bActionDone);
			if (bActionDone) miActionOrder++;
			break;
		case geUpload_PresetSettings_Upload_After:
			bSuccess = DoPresetSettings_Upload_After();
			miActionOrder++;
			break;

		case geUpload_Commit_PreVerify_Before:
			bSuccess = DoCommit_PreVerify_Before();
			miActionOrder++;
			break;
		case geUpload_Commit_PreVerify_Action:
			bSuccess = DoCommit_PreVerify_Action(&bActionDone);
			if (bActionDone) miActionOrder++;
			break;
		case geUpload_Commit_PreVerify_After:
			bSuccess = DoCommit_PreVerify_After(&bAlreadyThere);
			// Where to go?
			if (!bAlreadyThere) {
				// Brand new commit - go ahead
				miActionOrder++;
			}
			else {
				// Skip upload commit - we're done
				miActionOrder = geUpload_Done;
			}
			break;

		case geUpload_Commit_Upload_Before:
			bSuccess = DoCommit_Upload_Before();
			miActionOrder++;
			break;
		case geUpload_Commit_Upload_Action:
			bSuccess = DoCommit_Upload_Action(&bActionDone);
			if (bActionDone) miActionOrder++;
			break;
		case geUpload_Commit_Upload_After:
			bSuccess = DoCommit_Upload_After();
			miActionOrder++;
			break;

		default:
			// Why are we here?
			break;
	}

	if (!bSuccess) {
		miActionOrder = geUpload_Done;
	}
	return bSuccess;
} // DoWork


tbool CUploadTask::IsDone()
{
	return (miActionOrder >= geUpload_Done);
} // IsDone


tbool CUploadTask::DoBranch_PreVerify_Before()
{
	if (mpfileReply_VerifyMisc) {
		// Release file - it was from previous transfer
		mpfileReply_VerifyMisc->Destroy();
		mpfileReply_VerifyMisc = NULL;
	}
	mpfileReply_VerifyMisc = IFileMemory::Create();
	if (mpfileReply_VerifyMisc == NULL)
		return false;
	mpfileReply_VerifyMisc->Open(IFile::FileCreate);

	if (mpDownloader_VerifyMisc == NULL)
		mpDownloader_VerifyMisc = ine::IDownloader::Create();
	if (mpDownloader_VerifyMisc == NULL)
		return false;

	std::string sURI = std::string("/branches/") + msBranchUUID + "/rights.xml";
	if ((!mpDownloader_VerifyMisc->Init("koblo.com", sURI.c_str(), 80, msUser.c_str(), msPassword.c_str()))
		||
		(!mpDownloader_VerifyMisc->Start(mpfileReply_VerifyMisc))
	) {
		tchar pszErr[1024];
		mpDownloader_VerifyMisc->GetError(pszErr, 1024);
		msExtendedError = std::string("Init+start of verify-branch failed:\n") + pszErr;
		return false;
	}

	muiProgressIx = 0;
	muiProgressTarget = 1;
	msProgress = std::string("Looking up branch");

	return true;
} // DoBranch_PreVerify_Before

tbool CUploadTask::DoBranch_PreVerify_Action(tbool* pbActionDone)
{
	tint64 iVerifyBranch_Progress = 0;
	tint64 iVerifyBranch_Total = 1;
	if (!mpDownloader_VerifyMisc->GetProgress(&iVerifyBranch_Progress, &iVerifyBranch_Total)) {
		tchar pszErr[1024];
		mpDownloader_VerifyMisc->GetError(pszErr, 1024);
		msExtendedError = pszErr;
		return false;
	}

	muiProgressIx = (tuint64)(iVerifyBranch_Progress);
	muiProgressTarget = (tuint64)(iVerifyBranch_Total);

	// We don't care for the actual status of the download - we test that later
	*pbActionDone = mpDownloader_VerifyMisc->IsFinished();

	return true;
} // DoBranch_PreVerify_Action

tbool CUploadTask::DoBranch_PreVerify_After(tbool* pbAlreadyThere, tbool* pbNoTakes)
{
	tint32 iReplySize = (tint32)mpfileReply_VerifyMisc->GetMemorySize();
	tbool bBranchThere = mpDownloader_VerifyMisc->IsDone();
	if (!bBranchThere) {
		// Status 404 means "page not there" - that's ok ..
		if (mpDownloader_VerifyMisc->GetHttpStatusCode() != 404) {
			// .. but this on the other hand is an unexpected error
			tchar pszErr[1024];
			mpDownloader_VerifyMisc->GetError(pszErr, 1024);
			msExtendedError = std::string("Verify branch failed:\n") + pszErr;
			if (iReplySize > 1) {
				msExtendedError += "\n\n";
				msExtendedError += std::string((tchar*)(mpfileReply_VerifyMisc->GetMemoryPtr()), iReplySize);
			}
			return false;
		}
	}

	if (bBranchThere) {
		// Branch has already been uploaded
		*pbAlreadyThere = true;

		// Maybe we came here because we didn't know branch existed - fix that
		if (msBranchName.length() > 0) {
			gpApplication->Set_Branch_Name(msBranchName);
		}

		// Find out if we have sufficient access
		std::string sRights((tchar*)(mpfileReply_VerifyMisc->GetMemoryPtr()), iReplySize);
		if (mbRequiresWriteAccess) {
			tbool bWrite = gpApplication->Write_Permission(sRights);
			if (!bWrite) {
				msExtendedError = "You don't have upload rights for project/branch.";
				return false;
			}
		}
		else {
			tbool bRead = gpApplication->Read_Permission(sRights);
			if (!bRead) {
				msExtendedError = "You don't have read access to project/branch.";
				return false;
			}
		}

		if (mlistpTakes.size() == 0) {
			// No takes to upload
			*pbNoTakes = true;
		}
	}

	return true;
} // DoBranch_PreVerify_After


tbool CUploadTask::DoNewProject_Before()
{
	if (mpfileReply_Uploader) {
		// Previously used - close
		mpfileReply_Uploader->Destroy();
		mpfileReply_Uploader = NULL;
	}
	mpfileReply_Uploader = IFileMemory::Create();
	mpfileReply_Uploader->Open(IFile::FileCreate);

	if (
		(!mpUploader->Init("koblo.com", "/projects.xml", 80, msUser.c_str(), msPassword.c_str()))
		||
		(!mpUploader->AddParam("project[uuid]", msProjectUUID.c_str(), -1))
		||
		(!mpUploader->AddParam("project[name]", msProjectName.c_str(), -1))
		||
		(!mpUploader->AddParam("project[description]", msProjectDescription.c_str(), -1))
		||
		(!mpUploader->AddParam("project[license]", msProjectLicense.c_str(), -1))
		||
		(!mpUploader->AddParam("branch[uuid]", msBranchUUID.c_str(), -1))
		||
		(!mpUploader->Start(mpfileReply_Uploader))
	) {
		tchar pszErr[1024];
		mpUploader->GetError(pszErr, 1024);
		msExtendedError = "Init+start of create project failed:\n";
		msExtendedError += pszErr;
		return false;
	}

	muiProgressIx = 0;
	muiProgressTarget = 1;
	msProgress = std::string("Creating project");

	return true;
} // DoNewProject_Before

tbool CUploadTask::DoNewProject_Action(tbool* pbActionDone)
{
	if (mpUploader->IsFailed()) {
		tchar pszErr[1024];
		mpUploader->GetError(pszErr, 1024);
		msExtendedError = std::string("Create project failed:\n") + pszErr;
		tint32 iReplySize = (tint32)mpfileReply_Uploader->GetMemorySize();
		if (iReplySize > 1) {
			msExtendedError += "\n\n";
			msExtendedError += std::string((tchar*)(mpfileReply_Uploader->GetMemoryPtr()), iReplySize);
		}

		// We must clear UUID for project as it didn't work - otherwise we'll
		// think it succeeded and next commit will fail when opening project
		// in koblo studio later...
		gpApplication->Online_Project_Upload_Failed();

		return false;
	}

	tint64 iUp_Progress = 0;
	tint64 iUp_Total = 1;
	tint64 iReply_Progress = 0;
	tint64 iReply_Total = 1;
	mpUploader->GetProgress(&iUp_Progress, &iUp_Total, &iReply_Progress, &iReply_Total);
	muiProgressIx = (tuint64)(iUp_Progress + iReply_Progress);
	muiProgressTarget = (tuint64)(iUp_Total + iReply_Total);

	if (mpUploader->IsDone())
		*pbActionDone = true;

	return true;
} // DoNewProject_Action

tbool CUploadTask::DoNewProject_After(tbool* pbNoTakes)
{
	// Remember that we've created project!
	// We do this by setting branch-name of application to "trunk"
	gpApplication->Set_Branch_Name(msBranchName);

	if (mlistpTakes.size() == 0) {
		// No takes to upload - we're done
		*pbNoTakes = true;
	}

	return true;
} // DoNewProject_After


tbool CUploadTask::DoNewBranch_Before()
{
	if (mpfileReply_Uploader) {
		// Previously used - close
		mpfileReply_Uploader->Destroy();
		mpfileReply_Uploader = NULL;
	}
	mpfileReply_Uploader = IFileMemory::Create();
	mpfileReply_Uploader->Open(IFile::FileCreate);

	std::string sURI = std::string("/branches/") + msBranchUUID_Old + "/branch.xml";
	if (
		(!mpUploader->Init("koblo.com", sURI.c_str(), 80, msUser.c_str(), msPassword.c_str()))
		||
		(!mpUploader->AddParam("branch[uuid]", msBranchUUID.c_str(), -1))
		||
		(!mpUploader->AddParam("branch[name]", msBranchName.c_str(), -1))
		||
		(!mpUploader->AddParam("branch[description]", msBranchDescription.c_str(), -1))
		||
		(!mpUploader->AddParam("branch[license]", msBranchLicense.c_str(), -1))
		||
		(!mpUploader->Start(mpfileReply_Uploader))
	) {
		tchar pszErr[1024];
		mpUploader->GetError(pszErr, 1024);
		msExtendedError = "Init+start of create branch failed:\n";
		msExtendedError += pszErr;
		return false;
	}

	muiProgressIx = 0;
	muiProgressTarget = 1;
	msProgress = std::string("Creating branch");

	return true;
} // DoNewBranch_Before

tbool CUploadTask::DoNewBranch_Action(tbool* pbActionDone)
{
	if (mpUploader->IsFailed()) {
		tchar pszErr[1024];
		mpUploader->GetError(pszErr, 1024);
		msExtendedError = std::string("Create branch failed:\n") + pszErr;
		tint32 iReplySize = (tint32)mpfileReply_Uploader->GetMemorySize();
		if (iReplySize > 1) {
			msExtendedError += "\n\n";
			msExtendedError += std::string((tchar*)(mpfileReply_Uploader->GetMemoryPtr()), iReplySize);
		}
		return false;
	}

	tint64 iUp_Progress = 0;
	tint64 iUp_Total = 1;
	tint64 iReply_Progress = 0;
	tint64 iReply_Total = 1;
	mpUploader->GetProgress(&iUp_Progress, &iUp_Total, &iReply_Progress, &iReply_Total);
	muiProgressIx = (tuint64)(iUp_Progress + iReply_Progress);
	muiProgressTarget = (tuint64)(iUp_Total + iReply_Total);

	if (mpUploader->IsDone())
		*pbActionDone = true;

	return true;
} // DoNewBranch_Action

tbool CUploadTask::DoNewBranch_After(tbool* pbNoTakes)
{
	// Remember that we've created project!
	// We do this by setting branch-name of application to input branch name
	gpApplication->Set_Branch_Name(msBranchName);

	if (mlistpTakes.size() == 0) {
		// No takes to upload - we're done
		*pbNoTakes = true;
	}

	return true;
} // DoNewBranch_After


tbool CUploadTask::DoTake_PreVerify_Before()
{
	if (mlistpTakes.size() == 0) {
		msExtendedError = "Internal error: Where did upload takes in list go?";
		return false;
	}
	if (mpTakeCurr) {
		// Release this take - it has already been handled
		delete mpTakeCurr;
		mpTakeCurr = NULL;
	}
	mpTakeCurr = mlistpTakes.front();
	mlistpTakes.pop_front();

	if (mpfileReply_VerifySample) {
		// Release file - it was from previous transfer
		mpfileReply_VerifySample->Destroy();
		mpfileReply_VerifySample = NULL;
	}
	if (mpfileReply_VerifyTake) {
		// Release file - it was from previous transfer
		mpfileReply_VerifyTake->Destroy();
		mpfileReply_VerifyTake = NULL;
	}
	mpfileReply_VerifySample = IFileMemory::Create();
	mpfileReply_VerifyTake = IFileMemory::Create();
	if ((mpfileReply_VerifySample == NULL) || (mpfileReply_VerifyTake == NULL))
		return false;
	mpfileReply_VerifySample->Open(IFile::FileCreate);
	mpfileReply_VerifyTake->Open(IFile::FileCreate);

	if (mpDownloader_VerifySample == NULL)
		mpDownloader_VerifySample = ine::IDownloader::Create();
	if (mpDownloader_VerifyTake == NULL)
		mpDownloader_VerifyTake = ine::IDownloader::Create();
	if ((mpDownloader_VerifySample == NULL) || (mpDownloader_VerifyTake == NULL))
		return false;

	std::string sURI_VerifySample = std::string("/samples/") + mpTakeCurr->GetUUID_ForOwningSample();
	sURI_VerifySample += ".xml";
	if ((!mpDownloader_VerifySample->Init("koblo.com", sURI_VerifySample.c_str(), 80, msUser.c_str(), msPassword.c_str()))
		||
		(!mpDownloader_VerifySample->Start(mpfileReply_VerifySample))
	) {
		tchar pszErr[1024];
		mpDownloader_VerifySample->GetError(pszErr, 1024);
		msExtendedError = std::string("Init+start of verify-sample failed:\n") + pszErr;
		return false;
	}
	std::string sURI_VerifyTake = std::string("/takes/") + mpTakeCurr->Get_UUID();
	sURI_VerifyTake += ".xml";
	if ((!mpDownloader_VerifyTake->Init("koblo.com", sURI_VerifyTake.c_str(), 80, msUser.c_str(), msPassword.c_str()))
		||
		(!mpDownloader_VerifyTake->Start(mpfileReply_VerifyTake))
	) {
		tchar pszErr[1024];
		mpDownloader_VerifyTake->GetError(pszErr, 1024);
		msExtendedError = std::string("Init+start of verify-take failed:\n") + pszErr;
		return false;
	}

	muiProgressIx = 0;
	muiProgressTarget = 1;
	msProgress = std::string("Looking up ") + mpTakeCurr->Screen_Name();

	return true;
} // DoTake_PreVerify_Before

tbool CUploadTask::DoTake_PreVerify_Action(tbool* pbActionDone)
{
	tint64 iVerifySample_Progress = 0;
	tint64 iVerifySample_Total = 1;
	if (!mpDownloader_VerifySample->GetProgress(&iVerifySample_Progress, &iVerifySample_Total)) {
		tchar pszErr[1024];
		mpDownloader_VerifySample->GetError(pszErr, 1024);
		msExtendedError = pszErr;
		return false;
	}

	tint64 iVerifyTake_Progress = 0;
	tint64 iVerifyTake_Total = 1;
	if (!mpDownloader_VerifyTake->GetProgress(&iVerifyTake_Progress, &iVerifyTake_Total)) {
		tchar pszErr[1024];
		mpDownloader_VerifyTake->GetError(pszErr, 1024);
		msExtendedError = pszErr;
		return false;
	}

	muiProgressIx = (tuint64)(iVerifySample_Progress + iVerifyTake_Progress);
	muiProgressTarget = (tuint64)(iVerifySample_Total + iVerifyTake_Total);

	// We don't care for the actual status of the downloads - we test that later
	tint32 iFinished = 0;
	if (mpDownloader_VerifySample->IsFinished()) {
		iFinished++;
	}
	if (mpDownloader_VerifyTake->IsFinished()) {
		iFinished++;
	}

	*pbActionDone = (iFinished == 2);
	return true;
} // DoTake_PreVerify_Action

tbool CUploadTask::DoTake_PreVerify_After(tbool* pbSkipThisTake, tbool* pbNoMoreTakes)
{
	tbool bTakeThere = mpDownloader_VerifyTake->IsDone();
	if (!bTakeThere) {
		// Status 404 means "page not there" - that's ok ..
		if (mpDownloader_VerifyTake->GetHttpStatusCode() != 404) {
			// .. but this on the other hand is an unexpected error
			tchar pszErr[1024];
			mpDownloader_VerifyTake->GetError(pszErr, 1024);
			msExtendedError = std::string("Verify take failed:\n") + pszErr;
			tint32 iReplySize = (tint32)mpfileReply_VerifyTake->GetMemorySize();
			if (iReplySize > 1) {
				msExtendedError += "\n\n";
				msExtendedError += std::string((tchar*)(mpfileReply_VerifyTake->GetMemoryPtr()), iReplySize);
			}
			return false;
		}
	}

	// (lasse) no! if (bTakeThere && mpfileReply_Uploader) {
	if (bTakeThere) {
		// Take has already been uploaded
		*pbSkipThisTake = true;

		// We came here because we didn't have URL for take - fix that
		if (!SetTakeURL(mpfileReply_VerifyTake))
			return false;

		if (mlistpTakes.size() == 0) {
			// No more takes
			*pbNoMoreTakes = true;
		}
	}

	return true;
} // DoTake_PreVerify_After


tbool CUploadTask::DoTake_Upload_Before()
{
	tbool bSampleThere = mpDownloader_VerifySample->IsDone();
	if (!bSampleThere) {
		// Status 404 means "page not there" - that's ok ..
		if (mpDownloader_VerifySample->GetHttpStatusCode() != 404) {
			// .. but this on the other hand is an unexpected error
			tchar pszErr[1024];
			mpDownloader_VerifySample->GetError(pszErr, 1024);
			msExtendedError = std::string("Verify sample failed:\n") + pszErr;
			tint32 iReplySize = (tint32)mpfileReply_VerifySample->GetMemorySize();
			if (iReplySize > 1) {
				msExtendedError += "\n\n";
				msExtendedError += std::string((tchar*)(mpfileReply_VerifySample->GetMemoryPtr()), iReplySize);
			}
			return false;
		}
	}

	if (mpfileOgg) {
		mpfileOgg->Destroy();
		mpfileOgg = NULL;
	}
	if (mpfileMp3) {
		mpfileMp3->Destroy();
		mpfileMp3 = NULL;
	}
	mpfileOgg = IFile::Create();
	mpfileMp3 = IFile::Create();
	if (mpfileMp3 == NULL || mpfileOgg == NULL) {
		return false;
	}
	std::string sOgg = mpTakeCurr->GetOggFilePath();
	std::string sMp3 = mpTakeCurr->GetMp3FilePath();
	if (!mpfileOgg->Open(sOgg.c_str(), IFile::FileRead)) {
		msExtendedError = std::string("Can't read-open file:\n") + sOgg;
		return false;
	}
	if (!mpfileMp3->Open(sMp3.c_str(), IFile::FileRead)) {
		msExtendedError = std::string("Can't read-open file:\n") + sMp3;
		return false;
	}

	if (mpfileReply_Uploader) {
		// Previously used - close
		mpfileReply_Uploader->Destroy();
		mpfileReply_Uploader = NULL;
	}
	mpfileReply_Uploader = IFileMemory::Create();
	mpfileReply_Uploader->Open(IFile::FileCreate);

	tbool bInitError = false;
	if (bSampleThere) {
		// Previously uploaded sample - just upload a new take onto it
		std::string sURI = std::string("/samples/");
		sURI += mpTakeCurr->GetUUID_ForOwningSample();
		sURI += "/takes.xml";
		if (
			(!mpUploader->Init("koblo.com", sURI.c_str(), 80, msUser.c_str(), msPassword.c_str()))
			||
			(!mpUploader->AddParam("take[uuid]", mpTakeCurr->Get_UUID().c_str(), -1))
			||
			(!mpUploader->AddParam("take[description]", mpTakeCurr->Get_Description().c_str(), -1))
			||
			(!mpUploader->AddFileParam("audio[uploaded_data]", mpfileOgg))
			||
			(!mpUploader->AddFileParam("mp3[uploaded_data]", mpfileMp3))
		) {
			bInitError = true;
		}
	}
	else {
		// All new sample - create sample + upload take
		std::string sURI = std::string("/projects/");
		sURI += msProjectUUID;
		sURI += "/samples.xml";
		std::string sDescription = mpTakeCurr->Get_Description();
		if (sDescription.length() == 0) sDescription = "No description";
		if (
			(!mpUploader->Init("koblo.com", sURI.c_str(), 80, msUser.c_str(), msPassword.c_str()))
			||
			(!mpUploader->AddParam("sample[uuid]", mpTakeCurr->GetUUID_ForOwningSample().c_str(), -1))
			||
			(!mpUploader->AddParam("sample[name]", mpTakeCurr->GetName_ForOwningSample().c_str(), -1))
			||
			(!mpUploader->AddParam("take[uuid]", mpTakeCurr->Get_UUID().c_str(), -1))
			||
			(!mpUploader->AddParam("take[description]", sDescription.c_str(), -1))
			||
			(!mpUploader->AddFileParam("audio[uploaded_data]", mpfileOgg))
			||
			(!mpUploader->AddFileParam("mp3[uploaded_data]", mpfileMp3))
		) {
			bInitError = true;
		}
	}

	if (!bInitError) {
		if (!mpUploader->Start(mpfileReply_Uploader))
			bInitError = true;
	}

	if (bInitError) {
		tchar pszErr[1024];
		mpUploader->GetError(pszErr, 1024);
		msExtendedError = "Create branch failed:\n";
		msExtendedError += pszErr;
		return false;
	}

	muiProgressIx = 0;
	muiProgressTarget = 1;
	msProgress = std::string("Uploading ") + mpTakeCurr->Screen_Name();

	return true;
} // DoTake_Upload_Before

tbool CUploadTask::DoTake_Upload_Action(tbool* pbActionDone)
{
	if (mpUploader->IsFailed()) {
		tchar pszErr[1024];
		mpUploader->GetError(pszErr, 1024);
		msExtendedError = std::string("Upload of take failed:\n") + pszErr;
		tint32 iReplySize = (tint32)mpfileReply_Uploader->GetMemorySize();
		if (iReplySize > 1) {
			msExtendedError += "\n\n";
			msExtendedError += std::string((tchar*)(mpfileReply_Uploader->GetMemoryPtr()), iReplySize);
		}
		return false;
	}

	tint64 iUp_Progress = 0;
	tint64 iUp_Total = 1;
	tint64 iReply_Progress = 0;
	tint64 iReply_Total = 1;
	mpUploader->GetProgress(&iUp_Progress, &iUp_Total, &iReply_Progress, &iReply_Total);
	muiProgressIx = (tuint64)(iUp_Progress + iReply_Progress);
	muiProgressTarget = (tuint64)(iUp_Total + iReply_Total);

	if (mpUploader->IsDone())
		*pbActionDone = true;

	return true;
} // DoTake_Upload_Action

tbool CUploadTask::DoTake_Upload_After(tbool* pbNoMoreTakes)
{
	if (!SetTakeURL(mpfileReply_Uploader))
		return false;

	if (mlistpTakes.size() == 0) {
		// No more takes
		*pbNoMoreTakes = true;
	}
	return true;
} // DoTake_Upload_After


tbool CUploadTask::DoPresetSettings_PreVerify_Before(tbool* pbAlreadyThere)
{
	if (msPresetDataUUID.length() == 0) {
		// We don't need to upload preset data
		*pbAlreadyThere = true;
		return true;
	}

	if (mpfileReply_VerifyMisc) {
		// Release file - it was from previous transfer
		mpfileReply_VerifyMisc->Destroy();
		mpfileReply_VerifyMisc = NULL;
	}
	mpfileReply_VerifyMisc = IFileMemory::Create();
	if (mpfileReply_VerifyMisc == NULL)
		return false;
	mpfileReply_VerifyMisc->Open(IFile::FileCreate);

	if (mpDownloader_VerifyMisc == NULL)
		mpDownloader_VerifyMisc = ine::IDownloader::Create();
	if (mpDownloader_VerifyMisc == NULL)
		return false;

	std::string sURI = std::string("/plugindata/") + msPresetDataUUID + ".xml";
	if ((!mpDownloader_VerifyMisc->Init("koblo.com", sURI.c_str(), 80, msUser.c_str(), msPassword.c_str()))
		||
		(!mpDownloader_VerifyMisc->Start(mpfileReply_VerifyMisc))
	) {
		tchar pszErr[1024];
		mpDownloader_VerifyMisc->GetError(pszErr, 1024);
		msExtendedError = std::string("Init+start of verify preset data failed:\n") + pszErr;
		return false;
	}

	muiProgressIx = 0;
	muiProgressTarget = 1;
	msProgress = std::string("Looking up preset data");

	return true;
} // DoPresetSettings_PreVerify_Before

tbool CUploadTask::DoPresetSettings_PreVerify_Action(tbool* pbActionDone)
{
	tint64 iVerify_Progress = 0;
	tint64 iVerify_Total = 1;
	if (!mpDownloader_VerifyMisc->GetProgress(&iVerify_Progress, &iVerify_Total)) {
		tchar pszErr[1024];
		mpDownloader_VerifyMisc->GetError(pszErr, 1024);
		msExtendedError = pszErr;
		return false;
	}

	muiProgressIx = (tuint64)(iVerify_Progress);
	muiProgressTarget = (tuint64)(iVerify_Total);

	// We don't care for the actual status of the download - we test that later
	*pbActionDone = mpDownloader_VerifyMisc->IsFinished();

	return true;
} // DoPresetSettings_PreVerify_Action

tbool CUploadTask::DoPresetSettings_PreVerify_After(tbool* pbAlreadyThere)
{
	tint32 iReplySize = (tint32)mpfileReply_VerifyMisc->GetMemorySize();
	tbool bIsThere = mpDownloader_VerifyMisc->IsDone();
	if (!bIsThere) {
		// Status 404 means "page not there" - that's ok ..
		if (mpDownloader_VerifyMisc->GetHttpStatusCode() != 404) {
			// .. but this on the other hand is an unexpected error
			tchar pszErr[1024];
			mpDownloader_VerifyMisc->GetError(pszErr, 1024);
			msExtendedError = std::string("Verify preset data failed:\n") + pszErr;
			if (iReplySize > 1) {
				msExtendedError += "\n\n";
				msExtendedError += std::string((tchar*)(mpfileReply_VerifyMisc->GetMemoryPtr()), iReplySize);
			}
			return false;
		}
	}

	if (bIsThere) {
		// preset data has already been uploaded
		*pbAlreadyThere = true;

		// We came here because we didn't know preset data existed - fix that
		std::string sXML((tchar*)(mpfileReply_VerifyMisc->GetMemoryPtr()), iReplySize);
		//!!! TODO: Important!!! We must save info that preset data exists!
		// How do we do that? Maybe it's not important?
		// ..... something here
	}

	return true;
} // DoPresetSettings_PreVerify_After


tbool CUploadTask::DoPresetSettings_Upload_Before()
{
	if (mpfileReply_Uploader) {
		// Previously used - close
		mpfileReply_Uploader->Destroy();
		mpfileReply_Uploader = NULL;
	}
	mpfileReply_Uploader = IFileMemory::Create();
	mpfileReply_Uploader->Open(IFile::FileCreate);

	std::string sURI = std::string("/branches/") + msBranchUUID + "/plugindata.xml";
	if (
		(!mpUploader->Init("koblo.com", sURI.c_str(), 80, msUser.c_str(), msPassword.c_str()))
		||
		(!mpUploader->AddParam("plugindata[uuid]", msPresetDataUUID.c_str(), -1))
		||
		(!mpUploader->AddFileParam(
			"plugindata[uploaded_data]",
			mpfilePresetData,
			NULL,
			ine::MIME_TYPE_KOBLO_PLUGINDATA)
		)
		||
		(!mpUploader->Start(mpfileReply_Uploader))
	) {
		tchar pszErr[1024];
		mpUploader->GetError(pszErr, 1024);
		msExtendedError = "Init+start of upload preset data failed:\n";
		msExtendedError += pszErr;
		return false;
	}

	muiProgressIx = 0;
	muiProgressTarget = 1;
	msProgress = std::string("Committing preset data");

	return true;
} // DoPresetSettings_Upload_Before

tbool CUploadTask::DoPresetSettings_Upload_Action(tbool* pbActionDone)
{
	if (mpUploader->IsFailed()) {
		tchar pszErr[1024];
		mpUploader->GetError(pszErr, 1024);
		msExtendedError = std::string("Commit of preset data failed:\n") + pszErr;
		tint32 iReplySize = (tint32)mpfileReply_Uploader->GetMemorySize();
		if (iReplySize > 1) {
			msExtendedError += "\n\n";
			msExtendedError += std::string((tchar*)(mpfileReply_Uploader->GetMemoryPtr()), iReplySize);
		}
		return false;
	}

	tint64 iUp_Progress = 0;
	tint64 iUp_Total = 1;
	tint64 iReply_Progress = 0;
	tint64 iReply_Total = 1;
	mpUploader->GetProgress(&iUp_Progress, &iUp_Total, &iReply_Progress, &iReply_Total);
	muiProgressIx = (tuint64)(iUp_Progress + iReply_Progress);
	muiProgressTarget = (tuint64)(iUp_Total + iReply_Total);

	if (mpUploader->IsDone())
		*pbActionDone = true;

	return true;
} // DoPresetSettings_Upload_Action

tbool CUploadTask::DoPresetSettings_Upload_After()
{
	// Get commit number
	tint32 iSize = (tint32)mpfileReply_Uploader->GetMemorySize();
	if (iSize > 1) {
		std::string sUrlReplyXML(
			(const tchar*)mpfileReply_Uploader->GetMemoryPtr(),
			iSize);
		
		//!!! TODO: Set URL in project xml, so it'll know where to get preset data
		//gpApplication->Pass_Branch_Revision(sCommitReply);
	}

	return true;
} // DoPresetSettings_Upload_After


tbool CUploadTask::DoCommit_PreVerify_Before()
{
	if (mpfileReply_VerifyMisc) {
		// Release file - it was from previous transfer
		mpfileReply_VerifyMisc->Destroy();
		mpfileReply_VerifyMisc = NULL;
	}
	mpfileReply_VerifyMisc = IFileMemory::Create();
	if (mpfileReply_VerifyMisc == NULL)
		return false;
	mpfileReply_VerifyMisc->Open(IFile::FileCreate);

	if (mpDownloader_VerifyMisc == NULL)
		mpDownloader_VerifyMisc = ine::IDownloader::Create();
	if (mpDownloader_VerifyMisc == NULL)
		return false;

	std::string sURI = std::string("/commits/") + msCommitUUID + ".xml";
	if ((!mpDownloader_VerifyMisc->Init("koblo.com", sURI.c_str(), 80, msUser.c_str(), msPassword.c_str()))
		||
		(!mpDownloader_VerifyMisc->Start(mpfileReply_VerifyMisc))
	) {
		tchar pszErr[1024];
		mpDownloader_VerifyMisc->GetError(pszErr, 1024);
		msExtendedError = std::string("Init+start of verify-commit failed:\n") + pszErr;
		return false;
	}

	muiProgressIx = 0;
	muiProgressTarget = 1;
	msProgress = std::string("Looking up commit");

	return true;
} // DoCommit_PreVerify_Before

tbool CUploadTask::DoCommit_PreVerify_Action(tbool* pbActionDone)
{
	tint64 iVerifyCommit_Progress = 0;
	tint64 iVerifyCommit_Total = 1;
	if (!mpDownloader_VerifyMisc->GetProgress(&iVerifyCommit_Progress, &iVerifyCommit_Total)) {
		tchar pszErr[1024];
		mpDownloader_VerifyMisc->GetError(pszErr, 1024);
		msExtendedError = pszErr;
		return false;
	}

	muiProgressIx = (tuint64)(iVerifyCommit_Progress);
	muiProgressTarget = (tuint64)(iVerifyCommit_Total);

	// We don't care for the actual status of the download - we test that later
	*pbActionDone = mpDownloader_VerifyMisc->IsFinished();

	return true;
} // DoCommit_PreVerify_Action

tbool CUploadTask::DoCommit_PreVerify_After(tbool* pbAlreadyThere)
{
	tbool bCommitThere = mpDownloader_VerifyMisc->IsDone();
	if (!bCommitThere) {
		// Status 404 means "page not there" - that's ok ..
		if (mpDownloader_VerifyMisc->GetHttpStatusCode() != 404) {
			// .. but this on the other hand is an unexpected error
			tchar pszErr[1024];
			mpDownloader_VerifyMisc->GetError(pszErr, 1024);
			msExtendedError = std::string("Verify commit failed:\n") + pszErr;
			tint32 iReplySize = (tint32)mpfileReply_VerifyMisc->GetMemorySize();
			if (iReplySize > 1) {
				msExtendedError += "\n\n";
				msExtendedError += std::string((tchar*)(mpfileReply_VerifyMisc->GetMemoryPtr()), iReplySize);
			}
			return false;
		}
	}

	if (bCommitThere) {
		// Commit has already been uploaded
		*pbAlreadyThere = true;

		// We came here because we didn't know commit existed - fix that
		//!!! TODO: Important!!! We must save info that commit exists!
		// How do we do that? Maybe it's not important?
		// ..... something here
	}

	return true;
} // DoCommit_PreVerify_After


tbool CUploadTask::DoCommit_Upload_Before()
{
	if (mpfileCommitXML) {
		// Previously used - close
		mpfileCommitXML->Destroy();
		mpfileCommitXML = NULL;
	}
	mpfileCommitXML = IFile::Create();
	if (!mpfileCommitXML->Open(msFileCommitXML.c_str(), IFile::FileRead)) {
		msExtendedError = std::string("Error read-opening project xml file:\n  ") + msFileCommitXML;
		return false;
	}
	tint32 iSizeXML = (tint32)mpfileCommitXML->GetSizeWhenOpened();
	tchar* pszXml = new tchar[iSizeXML + 1];
	if (pszXml == NULL) {
		// memory
		return false;
	}
	if (mpfileCommitXML->Read(pszXml, iSizeXML) != iSizeXML) {
		msExtendedError = std::string("Error reading file:\n  ") + msFileCommitXML;
		delete[] pszXml;
		return false;
	}
	msCommitXML = std::string(pszXml, iSizeXML);
	delete[] pszXml;

	if (mpfileReply_Uploader) {
		// Previously used - close
		mpfileReply_Uploader->Destroy();
		mpfileReply_Uploader = NULL;
	}
	mpfileReply_Uploader = IFileMemory::Create();
	mpfileReply_Uploader->Open(IFile::FileCreate);

	std::string sURI = std::string("/branches/") + msBranchUUID + "/commits.xml";
	if (
		(!mpUploader->Init("koblo.com", sURI.c_str(), 80, msUser.c_str(), msPassword.c_str()))
		||
		(!mpUploader->AddParam("commit[uuid]", msCommitUUID.c_str(), -1))
		||
		(!mpUploader->AddParam("commit[description]", msCommitDescription.c_str(), -1))
		||
		(!mpUploader->AddParam("commit[markup]", msCommitXML.c_str(), iSizeXML))
		||
		(!mpUploader->Start(mpfileReply_Uploader))
	) {
		tchar pszErr[1024];
		mpUploader->GetError(pszErr, 1024);
		msExtendedError = "Init+start of commit project file failed:\n";
		msExtendedError += pszErr;
		return false;
	}

	muiProgressIx = 0;
	muiProgressTarget = 1;
	msProgress = std::string("Committing project xml");

	return true;
} // DoCommit_Upload_Before

tbool CUploadTask::DoCommit_Upload_Action(tbool* pbActionDone)
{
	if (mpUploader->IsFailed()) {
		tchar pszErr[1024];
		mpUploader->GetError(pszErr, 1024);
		msExtendedError = std::string("Commit of project file failed:\n") + pszErr;
		tint32 iReplySize = (tint32)mpfileReply_Uploader->GetMemorySize();
		if (iReplySize > 1) {
			msExtendedError += "\n\n";
			msExtendedError += std::string((tchar*)(mpfileReply_Uploader->GetMemoryPtr()), iReplySize);
		}
		return false;
	}

	tint64 iUp_Progress = 0;
	tint64 iUp_Total = 1;
	tint64 iReply_Progress = 0;
	tint64 iReply_Total = 1;
	mpUploader->GetProgress(&iUp_Progress, &iUp_Total, &iReply_Progress, &iReply_Total);
	muiProgressIx = (tuint64)(iUp_Progress + iReply_Progress);
	muiProgressTarget = (tuint64)(iUp_Total + iReply_Total);

	if (mpUploader->IsDone())
		*pbActionDone = true;

	return true;
} // DoCommit_Upload_Action

tbool CUploadTask::DoCommit_Upload_After()
{
	// Get commit number
	tint32 iSize = (tint32)mpfileReply_Uploader->GetMemorySize();
	if (iSize > 1) {
		std::string sCommitReply(
			(const tchar*)mpfileReply_Uploader->GetMemoryPtr(),
			iSize);
		
		gpApplication->Pass_Branch_Revision(sCommitReply);

//		tint32 iDummy = 0;
		//!!! TODO: Set commit number by calling a setter function on gpApplication
		// .... something here
	}

	return true;
} // DoCommit_Upload_After


tbool CUploadTask::SetTakeURL(IFileMemory* pfile)
{
	tint32 iSize = (tint32)pfile->GetMemorySize();
	std::string sXML((tchar*)pfile->GetMemoryPtr(), iSize);

	// Call setter function on application
	//!!! TODO: Implement below!
#if (1)
	if (!gpApplication->Set_Take_Handshake(sXML)) {
		msExtendedError = "Error parsing/setting URL for uploaded take";
		return false;
	}
#endif
	return true;
} // SetTakeURL


