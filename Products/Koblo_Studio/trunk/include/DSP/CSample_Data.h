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
// along with the Koblo Stools. If not, see <http://www.gnu.org/licenses/>.



/*! \brief CSample data object \
 *  holds sample info 
 */

class CSample_Data{
	
public:
	//! Constructor
	CSample_Data();
	
	//! Destructor
	virtual ~CSample_Data();
	
	//! Name of clip as seen in the list
	std::string sName;
	
	//! Set sample name
	void Set_Name( std::string Name){ sName = Name;};
	//! get sample name
	std::string Get_Name(){ return sName;};
	//! get a pointer to the takes
	CTake_Data* Get_Take_Data(){ return mpTake_Data;};
	//! get the Sample UUID
	std::string Get_UUID(){ return msUUID;};
	//! set uuid
	void Generate_UUID();
	
	
	
protected:
	
	//! UUID
	std::string msUUID;
	
	
	//!!! TO DO turn this in to a stdlist of takes later
	CTake_Data msTake_Data;
	CTake_Data* mpTake_Data;
	
};