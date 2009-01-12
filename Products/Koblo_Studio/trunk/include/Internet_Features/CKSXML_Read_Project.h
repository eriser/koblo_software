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

/*! \class CKSXML_Read_Project
 * \brief class that parse the XML data in to the internal KS DataEngine
 based on TinyXml www.sourceforge.net/projects/tinyxml
 */

class CKSApplication;
//class TiXmlDocument;


class CKSXML_Read_Project 
{
	



public:
	
	CKSXML_Read_Project();
	~CKSXML_Read_Project();
	
	//! reset project
	void Reset_Project();
	
	//! read a project XML file from Koblo.comin to the tiny xml DOM
	void Read_Project_XML_To_DOM(tint32 iProjectID);
	
	//! pass the DOM in to the KS data structure
	void CKSXML_Parse_DOM_To_Preset();

	//! itterate true the project XML file from the root level
	void Parse_Project( TiXmlNode* pParent );
	
	//! handle unknown tags
	void Ignore_Tag(TiXmlNode* pParent);
	
	//! set projet parameters from XML tag <project> in to KS
	/*!
	 \param TiXmlNode [in]: TiXmlNode.
	 */
	void Read_Project(TiXmlNode* pParent);
	
	//! handle the different ellements at the root level of the xml file sort out comments and the like
	/*!
	 \param TiXmlNode [in]: TiXmlNode.
	 */
	void Parse_Project_Childs(TiXmlNode* pParent);
	
	//! set branch parameters from XML tag <branch> in to KS
	/*!
	 \param TiXmlElement [in]: TiXmlElement.
	 */
	void Read_Branch(TiXmlElement* pElement);
	void Read_Settings(TiXmlElement* pElement);
	void Read_Signature(TiXmlElement* pElement);
	void Update_Signature(TiXmlElement* pElement);
	void Read_Edditing(TiXmlElement* pElement);
	void Read_Sample(TiXmlElement* pElement);
	void Read_Sample_Take(TiXmlElement* pElement);
	void Read_Loop(TiXmlElement* pElement);
	void Read_Windows(TiXmlElement* pElement);
	void Read_Window(TiXmlNode* pChild, tint32 iId);
	void Read_Window_Pos(TiXmlNode* pChild, tint32 iId);
	void Read_Window_Size(TiXmlNode* pChild, tint32 iId);
	void Read_Dialogs(TiXmlElement* pElement);
	void Read_Export_Dialog(TiXmlElement* pElement);
	void Read_Insert(TiXmlElement* pElement);
	void Read_Insert_Settings(TiXmlElement* pElement);
	void Read_Insert_Parameter(TiXmlElement* pElement);
	void Read_Track(TiXmlElement* pElement);
	void Read_Track_Name(TiXmlElement* pElement, tint32 iTrackID);
	void Read_Track_In(TiXmlElement* pElement, tint32 iTrackID);
	void Read_Track_Out(TiXmlElement* pElement, tint32 iTrackID);
	void Read_Track_Regions(TiXmlElement* pElement, tint32 iTrackID);
	
	void Read_Track_Solo(TiXmlNode* pParent, tint32 iTrackID);
	void Read_Track_Mute(TiXmlNode* pParent, tint32 iTrackID);
	void Read_Track_Arm(TiXmlNode* pParent, tint32 iTrackID);
//	void Read_Track_Aux(TiXmlNode* pParent, tint32 iTrackID, tint32 iAux);
	
	//void Read_Track_Aux(TiXmlElement* pElement, tint32 iTrackID);
	void Read_Track_Insert(TiXmlElement* pElement, tint32 iTrackID);
	void Read_Track_Region(TiXmlElement* pElement, tint32 iTrackID);
	void Read_Track_Region_Fade(TiXmlElement* pElement, tint32 iTrackID);
	
	void Read_Bus(TiXmlElement* pElement);
	void Read_Bus_Out(TiXmlElement* pElement);
	void Read_Bus_Aux(TiXmlElement* pElement);
	void Read_Bus_Insert(TiXmlElement* pElement);
	
	void Read_Master(TiXmlElement* pElement);
	void Read_Master_Out(TiXmlElement* pElement);
	void Read_Master_Insert(TiXmlElement* pElement);
	void Read_Master_Aux_Return(TiXmlElement* pElement);

	
	void Set_Param( TiXmlNode* pParent, tuint uiType, tuint32 uiID, tint32 Section, tfloat fFactor = 1.0f );
	
	tbool Check_For_Newer_Revision(tint32 iProject_ID);
	//! itterate true the project XML file from the root level
	void Check_Project( TiXmlNode* pParent );
	
	void Check_Project_Childs(TiXmlNode* pParent);
	
	void Check_Branch(TiXmlNode* pParent);
	
	void Check_Revision(TiXmlElement* pElement);

private:
	
	
	tint32 miTrack_ID;
	
	// create TinyXml document
	TiXmlDocument* mpDoc;
	
	tbool mbNew_Revision;
	
};

