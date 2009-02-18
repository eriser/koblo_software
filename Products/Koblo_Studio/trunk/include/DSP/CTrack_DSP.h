

class CTrack_DSP: public virtual CKSUUID
{
public:
	//! Constructor
	CTrack_DSP(CDSP* pDSP, tint32 iChannelNumber, tbool bIsBusOrMix, CTrack_DSP** ppAUXes);

	//! Destructor
	virtual ~CTrack_DSP();

	//! Initializw
	virtual void Initialize();
	//! deinitialize
	virtual void DeInitialize();
	//! process track
	virtual void Process(tint32 iSamples);

	//! Set volume 0.0-0.1f
	virtual void Set_Volume(tfloat32 fVolume);
	//! set aux send
	virtual void SetAUXVolume(tint32 iAUX, tfloat32 fVolume);
	//! clear all effect tails
	virtual void ResetAllEffectsTails();
	//! set left right panning
	virtual void SetPanningLeftRight(tfloat32 fLeftRight);
	//! set front back panning !not used
	virtual void SetPanningFrontBack(tfloat32 fFrontBack);
	//! Set input channel count
	virtual void SetInputChannelCount(tuint32 iChannels);
	//! set first input
	virtual void SetFirstInput(tuint32 uiFirstInput);
	//! set track mode (curently always stereo)
	virtual void SetTrackMode(tuint32 iChannels);
	//! out put destination
	virtual void SetOutputDestination(tuint32 uiDest, tint32 iDestNumberOfChannels);
	//! get track mode
	tuint32 GetChannelMode() const {return miModeChannelCount;};

	//! IMixerChannel override
	virtual tuint32 GetOutputDestination() const {return miDestination;};

	virtual tint32 GetNumberOfChannelsForPanner() { return miNumberOfChannelsForPanner; };
	virtual tint32 GetNumberOfInputChannels() { return miInputChannelCount; };

	CBuffer* GetBuffer() {return mpBuffer;}

	CRegion_DSP* CreateRegion(tint32 iUniqueID, 
								const std::string& sSoundListItemName, 
								tuint64 uiTrack_Pos, 
								tuint64 uiSamplePosStart, 
								tuint64& ruiSamplePosEnd,
								tuint64 uiFadeInLength = 0,
								tuint64 uiFadeOutLength = 0,
								tfloat32 fRegionVolume = 1.0f);
	
	
	void Update_Regions_For_Playback();
	
	void DeleteRegion(tint32 iID);

	CRegion_DSP* GetRegion_DSP(tuint32 uiID);
	
	//! Get region track pos
	tuint64 Get_Region_Pos(tuint32 uiID);
	//! Update the size of a region;
	void Resize_Region(tuint32 uiID, tuint64 iTrackStartPos, tuint64 iSoundStartPos, tint64 iSoundEndPos);

	void SetSongPosition(tuint64 uiPosNew);
	
	tbool HasRegions() const {return mRegionInfoList.size() != 0;}

	void IncSongPos(tint32 iSamples) {muiSongPos += iSamples;}

	struct SChannelRegionInfo {
		CRegion_DSP* pRegion;
		tuint64 uiTrack_Pos;
		tuint32 uiRegionID;
	};

	void Insert_Region_Info( SChannelRegionInfo* pInfo);
	
	const std::list<SChannelRegionInfo*>& GetRegionList() {return mRegionInfoList;}

	void AddInsert(tint32 iInsert, tuint32 uiCompanyID, tuint32 uiProductID, tuint32 uiProductID2);

	void SetInsertBypass(tint32 iINsert, tbool bBypass);

	void LimitDataFromStream(tbool bEnableLimiting, tint64 iFirstSampleIx, tint64 iFinalSampleIx)
	{ mbLimitDataFromStream = bEnableLimiting; miFirstStreamIx_Limited = iFirstSampleIx; miFinalStreamIx_Limited = iFinalSampleIx; };


	void GetChannelRegionInfo(tuint32 iRegion, SChannelRegionInfo info);
	
//	void Edit_Selection_On_Track(tuint64 uiSelection_Pos, tuint64  uiSelection_Duration);
	// Fade selection in
	void Delete_Selection(tint32 iCmd, tuint64 uiSelection_Pos, tuint64  uiSelection_Duration);

/*	
	//! Render fade region and insert it on track
	tint32 Create_Fade_Region(	tint32 iCmd, 
								const std::string& sSoundClipName, 
								tuint64 uiTrack_Pos, 
								tuint64 uiSample_Start, 
								tuint64 uiSamplePosEnd,
								tuint64 uiSelection_Duration);
*/
	kspi::IPlugIn* GetInsert(tint32 iIndex) {return mppInsert[iIndex];}

	void SetArmed(tbool bNew) {mbArmed = bNew;}

	tbool IsArmed() const {return mbArmed;}

	void SetInputChannel(tint32 iNew) {miInputChannel = iNew;}

	void Start();

	void Stop();
	
	CKSUUID mUUID;

protected:
	tbool mbArmed;

	tint32 miInputChannel;

	std::list<SChannelRegionInfo*> mRegionInfoList;

	std::list<SChannelRegionInfo*>::const_iterator mitRegionsInfo;

	tint32 miRecordingChannels;

	tint64 miRecordingSongPos;

	CBuffer* mpBuffer;
	// Parrent pointer
	CDSP* mpDSP;
	
	st::IDSPTools* mpDSPTools;

	CTrack_DSP** mppAUXes;

	tfloat32 mfVolumeSlider;
	tfloat32 mfPanningLeft;//, 
	tfloat32 mfPanningFrontBack;
	tfloat32 mfPanningRight;
	tfloat32 mpfAUXVolume[2];

	CBaseDezipper2* mapDezipperOutAmp[8];
	CBaseDezipper2* mpDezipperPreAmp;
	
	tbool mbMeterBeforeEffects;
	tint32 miNumberOfChannelsForMeter;

	tint32 miTrack;

	tfloat32 mpfBufferTmp1[giAudioMaxBufferSize];
	tfloat32 mpfBufferTmp2[giAudioMaxBufferSize];

	CPlugInManager::PlugInHandle mhInserts[4];
	kspi::IPlugIn* mppInsert[4];
	tbool mpbInsertBypass[4];

	tuint32 miInputChannelCount;
	tuint32 muiFirstInput;
	tuint32 miModeChannelCount;
	tuint32 miDestination;
	tint32 miDestinationNumberOfChannels;
	tint32 miNumberOfChannelsForPanner;
	tbool mbIsBusOrMix;
	tbool mbKillDecay;


	//! True if we only play back a certain range of the stream data
	tbool mbLimitDataFromStream;
	tint64 miFirstStreamIx_Limited;
	tint64 miFinalStreamIx_Limited;

	//! Internal use only (doesn't lock mutex) - calculates and sets how many channels should exist for panners and peak meters
	virtual void _CalcSetNumberOfChannelsForPanner();
	
	//! Internal use only (doesn't lock mutex) - calculates and sets the volume dezippers for the channels
	virtual void _CalcSetVolumeDezippers();
	
	//! Internal use only (doesn't lock mutex) - avoid hanging peak volumes when sliding volume and panners
	virtual void _KillPeakVolume();

	tuint64 muiSongPos;

	// fade volume
	tfloat32 mfFade_Vol;
	// Fade position
	tint64 miFade_Pos;

	IFile* mpFileRecording;
	tfloat32 mfPeak;
	tfloat32 mfPeakMono;

	std::string msRecordingName;
	std::string msFile_Name_Path_On_Disk;
	
	
	
};






