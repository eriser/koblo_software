// Copyright 2004, 2005,2006,2007,2008,2009 Koblo http://koblo.com
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

/*!	\file ine.h
	\author Michael Olsen
	\date 23/May/2005
	\date 23/May/2005
*/

#ifndef _INE_INCLUDED
#define _INE_INCLUDED



// BaseUtilities
#include "bu.h"


namespace k2s {

/*! \namespace
 * \brief Namespace for INet engine
 *
 * This namespace is for accessing a database residing on another server (using tcp/ip)
*/
namespace ine {

#include "IINetUtil.h"
#include "IDownloader.h"

}	// namespace ine

}	// namespace k2s

#endif	// _INE_INCLUDED


