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

//!!! TO ASK is there always only one channel in a sound?
//#define MAX_CHANNELS_IN_SOUND 0

/*! \brief CSample process on CWaveFile \
 *  trim start and end \
 */

class CSample_DSP
{
public:
	CSample_DSP(const std::string& sPathName);

	tuint64 GetLength() {return muiLength;}

	void GetSamples(tfloat32* pfData, tint32 iSamples) {mppStream->GetSamples(pfData, iSamples); muiPos += iSamples;}

	const std::string& GetPathName() const {return msPathName;}

	void SetPos(tuint64 uiPos) {mppStream->SetPosition(uiPos); muiPos = uiPos;}

	tuint64 GetPos() const {return muiPos;}

protected:
	tuint64 muiPos;

	std::string msPathName;

	tuint64 muiLength;
	
	st::IStream* mppStream;
};

