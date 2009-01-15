// Copyright 2004, 2005,2006,2007,2008,2009 Koblo http://koblo.com
//
// This file is part of the Koblo Stools.
//
// the Koblo Stools is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// the Koblo Stools is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Koblo Stools. If not, see <http://www.gnu.org/licenses/>.


//! Truncates a floating point number and converts it to int
#define TRUNC(a) \
	(unsigned int)((a))

//! Linear interpolation for floating point. a: Accumulator [in]. b: Integer index [out]. c: Fraction [out]
#define INDEX_DELTA(a, b, c) \
		b = TRUNC(a); \
		c = a - b

class CLFOLOT : public virtual ILFOLOT
{
public:
	CLFOLOT();
	virtual ~CLFOLOT();

	// IOscillatorLOT overrides
	virtual void Destroy();
	virtual void SetSampleRate(tfloat fSampleRate);
	virtual void SetAttack(tfloat fMs);
	virtual void SetLOT(tfloat* pfLOT, tint iLOTSize);
	virtual void Reset();
	virtual tfloat Process(const tfloat32 pfRate, tint lC);

protected:
	float mfSampleRate;
	float mfOneDivSampleRate;
	float mfThirtyTwoDivSampleRate;
	tint  miPhase;
	float mfAttack;
	float mfEnvAmp;

	double* mpdBuffer;
	long mlBufferSize;
	long mlBufferIndex;

	float* mpfLOT;
	int	miLOTSize;

	float mfItrFactor;

};

