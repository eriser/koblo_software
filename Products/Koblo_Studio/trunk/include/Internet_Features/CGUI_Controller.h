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

//! Called when a buffer of samples needs to be filled
/*!
 \param iID [in]: ID of callback as when SetCallback was called.
 \param pfOut [out]: Buffer to be filled
 \param iCount [in]: Number of samples to fill in buffer
 */

/*! \class CGUI_Controller
 * \brief class that handles all gui logic 
 */


class CGUI_Controller
{
	


	

	
public:
	
	CGUI_Controller();
	~CGUI_Controller();
	
	void Open_Close_Mix_Window();
	void Open_Close_Rack_Window();
	void Open_Close_Import_Audio_Window();
	void Select_All_Regions();
	void Close_Window();
	void Toggle_Window();
	void Show_All_Plugins(tbool bShow);
	virtual void Update_Windows();
	
protected:

};

