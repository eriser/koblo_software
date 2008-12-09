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



class CAAmpGUI : public virtual CBaseGUI, public virtual ge::IEventReceiver
{
public:
	CAAmpGUI(CAAmpPlugIn* pPlugIn, de::IParameterManager* pParmMan);

	virtual ~CAAmpGUI();

	//! CBaseGUI override
	virtual void KSPI_CALL OnTimer();

	//! IDestructable override
	virtual void KSPI_CALL Destroy();

	//! IGUI override
	virtual void KSPI_CALL GetSize(tuint32* piSizeX, tuint32* piSizeY);

	//! IEventReceiver override
	virtual void EventValueChange(ge::IControl *pSender, tint32 iValueNew);
	//! IEventReceiver override
	virtual void EventGeneric(ge::IControl *pSender, void *pEventData);

	//! IContextCallback override
	virtual void GetName(tchar* psz);

	//! Returns the plug-in we're connected to
	CAAmpPlugIn* GetPlugIn() {return dynamic_cast<CAAmpPlugIn*>(mpPlugIn);}

	//! Overload
	const CAAmpPlugIn* GetPlugIn() const {return dynamic_cast<const CAAmpPlugIn*>(mpPlugIn);}

	//! Called by the plug-in when a parameter has changed
	virtual void UpdateEngineData(tint iType, tint iExtra, tint32 iID, tint32 iValue);

	virtual void UpdateAll() {}

protected:
	//! CBaseGUI override
	virtual void Initialize();

	//! Parameter/controls linking
	virtual void ConnectControls();
	
	tuint muiLimiter_Slot;
	tuint muiHarm_Slot;
	tuint muiTube_Slot;
};

