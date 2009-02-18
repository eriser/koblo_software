// Copyright 2004, 2005,2006,2007,2008,2009 Koblo http://koblo.com
//
// This file is part of the Koblo SDK.
//
// the Koblo SDK is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// the Koblo SDK is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Koblo SDK. If not, see <http://www.gnu.org/licenses/>.


class CDownloadTask : public CProgressTask {
public:
	tbool bSuccess;

	// State machine "enum" (it's an int to allow for ++ )
	tint32 miActionOrder;

	CDownloadTask();
	virtual ~CDownloadTask();
	virtual void Destroy();

	//! For downloading an entirely new project
	/*!
		\param pszUser [in]: Login user
		\param pszPassword [in]: Login password
		\param pszBranchUUID [in]: UUID of branch to download (Note! Not project, branch!)
	*/
	tbool Init_DownloadXML(
		const tchar* pszUser, const tchar* pszPassword,
		const tchar* pszBranchUUID);

	//! For updating an already downloaded project
	/*!
		\param pszUser [in]: Login user
		\param pszPassword [in]: Login password
		\param plistpTakes [in]: A list of takes to download
	*/
	tbool Init_Update(
		const tchar* pszUser,
		const tchar* pszPassword,
		std::list<CTake_Data*>* plistpTakes);

	virtual tbool DoWork();
	virtual tbool IsDone();

protected:
	tbool DoGetBranchXml_Before();
	tbool DoGetBranchXml_Action(tbool* pbActionDone);
	tbool DoGetBranchXml_After(tbool* pbNoTakes);

	tbool DoTake_Download_Before();
	tbool DoTake_Download_Action(tbool* pbActionDone);
	tbool DoTake_Download_After(tbool* pbNoMoreTakes);

	std::string msUser;
	std::string msPassword;

	std::string msBranchUUID;

	std::list<CTake_Data*> mlistpTakes;
	tbool Init_Helper(std::list<CTake_Data*>* plistpTakes);
	CTake_Data* mpTakeCurr;
	IFile* mpfileOgg;
	IFileMemory* mpfileXML;

	ine::IDownloader* mpDownloader;

}; // CExportClipTask
