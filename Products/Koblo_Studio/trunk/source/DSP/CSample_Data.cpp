
#include "KSOS.h"

CSample_Data::CSample_Data()
{
	mpTake_Data = new CTake_Data(this);
	muiTakes	=	1;
} 

CSample_Data::~CSample_Data()
{
	delete mpTake_Data;
} 

CSample_Data::CSample_Data(const CSample_Data& Data_Old)
{


	msName			=	Data_Old.msName;
	muiTakes		=	Data_Old.muiTakes;

	mpTake_Data		=	new CTake_Data(this);
	*mpTake_Data	=	*(Data_Old.mpTake_Data);
}






CTake_Data* CSample_Data::Get_Take_Data(std::string sUUID)
{ 
	
	//!!! TO DO iterate list of takes here when multiply takes are supported
	if(stricmp(sUUID.c_str(), mpTake_Data->Get_UUID().c_str()) == 0)
		return mpTake_Data;
	
	return NULL;
	
}


CTake_Data* CSample_Data::Get_Take_Data(tuint32 uiTake)
{ 
	//!!! TO DO iterate list of takes here when multiply takes are supported
	return mpTake_Data;
	

	
}

