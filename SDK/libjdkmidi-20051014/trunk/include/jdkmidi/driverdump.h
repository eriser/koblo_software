/*
 *  libjdkmidi-2004 C++ Class Library for MIDI
 *
 *  Copyright (C) 2004  J.D. Koftinoff Software, Ltd.
 *  www.jdkoftinoff.com
 *  jeffk@jdkoftinoff.com
 *
 *  *** RELEASED UNDER THE GNU GENERAL PUBLIC LICENSE (GPL) April 27, 2004 ***
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef JDKMIDI_DRIVERDUMP_H
#define JDKMIDI_DRIVERDUMP_H

#include "jdkmidi/driver.h"

namespace jdkmidi
{

  class MIDIDriverDump : public MIDIDriver
  {
    public:
      MIDIDriverDump ( int queue_size, FILE *outfile );
      virtual ~MIDIDriverDump();
      
      virtual bool HardwareMsgOut ( const MIDITimedBigMessage &msg );
      
      virtual void TimeTick ( unsigned long sys_time );
      
    protected:
    
      FILE *f;
  };
  
}

#endif