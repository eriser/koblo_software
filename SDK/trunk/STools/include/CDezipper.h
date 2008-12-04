// Copyright 2004, 2005,2006,2007,2008 Koblo http://koblo.com
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

/*

Dezipper implementation

We use a simple lp lowpass

*/


class CDezipper : public virtual IDezipper
{
public:
	CDezipper();
	virtual ~CDezipper();

	// IDezipper overrides
	virtual void Destroy();
	virtual void SetSampleRate(float fSampleRate);
	virtual void JumpToTarget(double dTarget);
	virtual void SetTarget(double dTarget);
	virtual double Dezip(long lC);

protected:
	float mfSampleRate;

	float mfFreq;

	double dHistory;
	double dX;

	void CalcFilter();

	virtual void Reset();

	double mdTarget;
};

