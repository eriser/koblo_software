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


/*! \class CFFTOSX
 * \brief OSX implementation of IFFT
*/
class CFFTOSX : public virtual IFFT
{
public:
	//! Constructor
	CFFTOSX();
	//! Destructor
	virtual ~CFFTOSX();

	//! IDestructable override
	virtual void Destroy();

	//! IFFT override
	virtual void Init(tuint uiOrder);
	//! IFFT override
	virtual void GetMagnitude(sample* pfSamples) throw (IException*);

protected:
	//! FFT order
	tuint muiOrder;

	//! Size of FFT (2 ^ order)
	tuint32 muiFFTSize;

	//! FFT "setup" (used by fft library)
	FFTSetup mFFT;

	COMPLEX_SPLIT mA;
};

