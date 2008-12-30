// Copyright 2004, 2005,2006,2007,2008 Koblo http://koblo.com
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
// along with the Koblo Stools. If not, see <http://www.gnu.org/licenses/>.

//! Called when a buffer of samples needs to be filled
/*!
 \param iID [in]: ID of callback as when SetCallback was called.
 \param pfOut [out]: Buffer to be filled
 \param iCount [in]: Number of samples to fill in buffer
 */

/*! \class CKSInternet_Features
 * \brief class that handles all Internet features
*/

class CKSPlugIn;


class CKSInternet_Features
{
		
private:
	CKSPlugIn* mpKSPlugIn;
	
	virtual void Open_Project_Edit_Page_On_Koblo();
		
		
	
public:
		
	CKSInternet_Features(CKSPlugIn * pKSPlugIn);
	~CKSInternet_Features();
	
	//! called from the menu and opens the project id dialog 
	virtual void On_Menu_Download_Project();
	
	
	
	//! called from the file menu
	virtual void On_Menu_Update_Project();
	
	//! called from the file menu
	virtual void On_Menu_Upload_Project();
	
	
	
	
	
	
	//! called from the file menu
	virtual void On_Menu_Commit_Project();

	//! clean project and read it from koblo.com
	void Download_Project(tint32 iProjectID);
	//! read project from koblo.com
	void Update_Project(tint32 iProjectID);
	//! upload project to koblo.com
	void Commit_Project();
	
	
	
	
	
	
	
	void Compress_Wave_File(std::string sSample);
	void Upload_Sample(std::string sSample);
	void Download_Take();
	
	//! upload compressed audio file and mp3 preview.
	void Upload_OGG_And_MP3_File();
	
		
		
		
};

