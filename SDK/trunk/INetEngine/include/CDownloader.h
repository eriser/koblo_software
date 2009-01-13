class CDownloader : public virtual IDownloader, public CUpAndDownloader_Common
{
public:
	CDownloader();
	virtual ~CDownloader();
	
	//! IDestructable override
	virtual void Destroy();

	//! IDownloader implementation
	virtual tbool Init(const tchar* pszHost, const tchar* pszPage, tint32 iPort = 80, const tchar* pszUser = NULL, const tchar* pszPassword = NULL, tint32 iTimeOutSecs = 10);
	//! IDownloader implementation
	virtual tbool SetDesiredMIMEType(E_MIME_Type eType);
	//! IDownloader implementation
	virtual tbool SetSpecificVerb(EVerbType eVerb);
	//! IDownloader implementation
	virtual tbool AddParam(const tchar* pszParamName, const tchar* pcParamData, tint32 iParamDataLen);
	//! IDownloader implementation
	virtual tbool DownloadPortion(tchar* pszBuffer, tint32 iBufferSize, tint32* piPortionSize, tuint64* puiTotalSize);
	//! IDownloader implementation
	virtual tbool Abort();

	//! IDownloader implementation
	virtual tbool IsDone();
	//! IDownloader implementation
	virtual tbool IsFailed();

	//! IDownloader implementation
	virtual tbool GetLatestError(tchar* pszErrBuff, tint32 iErrBuffSize);

protected:
	void Constructor_OSSpecific();
	void Destructor_OSSpecific();

	CMutex mMutex_Connection;
	tbool OpenConnection();
	tbool OpenConnection_OSSpecific();
	void CloseConnection();
	void CloseConnection_OSSpecific();
	
	tbool DownloadPortion_OSSpecific(tchar* pszBuffer, tint32 iBufferSize, tint32* piPortionSize, tuint64* puiTotalSize);
	

#ifdef _WIN32
	#include "CDownloaderWin.h"
#endif // _WIN32
#ifdef _Mac
	#include "CDownloaderOSX.h"
#endif // _Mac

};