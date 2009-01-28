

class CTake_Data:
public virtual CKSUUID
{
public:
	//! Constructor
	CTake_Data();

	//! Destructor
	virtual ~CTake_Data();

	
	
	//! Name of the left side (or mono) wave file for clip
	std::string sWaveNameL;
	//! Name of the right side wave file for clip
	std::string sWaveNameR;
	
	//!!! obsolete
	/*
	//! Name of the original from which the clip was imported/decompressed
	std::string sOriginalName;
	//! Extension of the original from which the clip was imported/decompressed
	std::string sOriginalExt;
	
	//! This is relevant if unpacking a compressed project - must decompress both mono clips at same time
	tbool bIsOriginalStereo;
	
	//! Is the original lossy compressed?
	tbool bIsOriginalLossy;
	 */
	
	//! Which stereo channels to map: 1 = left, 2 = right, 3 = both
	tint32 iOriginalChannelMask;
	
	//! Is this displayed as a stereo clip in list?
	tbool bIsStereoInList;
	
	// -------------
	// Variables below here aren't saved but regenerated upon load
	std::string sWavePathNameL;
	std::string sWavePathNameR;
	
	//! get the Sample UUID
	void Set_Description(std::string sDescription ){msDescription =  sDescription;};
	
	//! get the description
	std::string Get_Description(){ return msDescription;};
	
	//! get the Sample UUID
	void Set_URL(std::string sURL ){ msURL = sURL;};
	
	//! get the description
	std::string Get_URL(){ return msURL;};
	
	//! path to the left peak file
	std::string sLeft_Peak_File_Path;
	
	//! path to the right peak file
	std::string sRight_Peak_File_Path;

	
	
protected:
	

	//! description
	std::string msDescription;
	//! url
	std::string msURL;
	
	
	
};






