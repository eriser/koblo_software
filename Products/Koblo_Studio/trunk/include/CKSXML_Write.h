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

/*! \class CKSXML_Write
 * \brief class that parse the XML data in to the internal KS DataEngine
 based on TinyXml www.sourceforge.net/projects/tinyxml
 */

class CKSPlugIn;


class CKSXML_Write //:  public virtual CBasePlugIn, public virtual IBaseDezipperCallback, public virtual ITimerCallback_CanStop
{
	
private:
	CKSPlugIn* mpKSPlugIn;
//	TiXmlDocument mpDoc;
//	TiXmlElement mpProject;
	
	void Add_Comment(TiXmlDocument* doc, std::string str);
	void Add_Comment(TiXmlElement* pParent,std::string str);
	
	
/*! Koblo Studio music project XML format
	most ids are originally returned from the website, and are globally unique (between different projects) 
	ids are always specified as attibutes 
	all times and track positions are specified in sample points
 */
	//! Root of TinyXML DOM
	void Write_Project(TiXmlDocument* doc);
	//! information about the branch
	void Write_Branch(TiXmlElement* pParent);
	//! overall project settings
	void Write_Settings(TiXmlElement* pParent);
	void Write_Signature(TiXmlElement* pParent);
	void Write_License(TiXmlElement* pParent);
	//! editing settings. these are things that would typically not be moved/merged between branches
	void Write_Editing(TiXmlElement* pParent);
	void Write_Tool(TiXmlElement* pParent);
	void Write_Loop(TiXmlElement* pParent);
	void Write_Windows(TiXmlElement* pParent);
	void Write_Window_Edit(TiXmlElement* pParent);
	void Write_Window_Mix(TiXmlElement* pParent);
	void Write_Window_Rack(TiXmlElement* pParent);
	void Write_Window_Position(TiXmlElement* pParent, tuint uiPosX, tuint uiPosY);
	void Write_Window_Size(TiXmlElement* pParent, tuint uiSizeX, tuint uiSizeY);
	//! Tracks
	
	
public:
	
	CKSXML_Write(CKSPlugIn * pKSPlugIn);
	~CKSXML_Write();
	
	
	//! write the XML file to Koblo.com
	void Write_XML(std::string str);
	
};

