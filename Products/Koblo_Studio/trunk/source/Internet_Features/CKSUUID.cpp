#include "KSOS.h"

CKSUUID::CKSUUID()
{

	msUUID	=	"";

}

CKSUUID::~CKSUUID()
{
	
}
void CKSUUID::Check_UUID()
{
	if(msUUID.size() == 0)
		Generate_UUID();
}


std::string CKSUUID::Get_UUID( )
{
	Check_UUID();
	return msUUID;
}

void CKSUUID::Generate_UUID()
{
	
	tchar* pszUUID = new tchar[128];
	tuint32 uiBuffer_Size;
	
	Gen_UUID(pszUUID, uiBuffer_Size);
	msUUID = pszUUID;
	
}