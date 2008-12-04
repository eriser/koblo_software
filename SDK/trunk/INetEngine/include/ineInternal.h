/*!	\file ineInternal.h
	\author Michael Olsen
	\date 23/May/2005
	\date 23/May/2005
*/

#include "ine.h"

// Inheritance via dominance
#pragma warning(disable: 4250)
// Identifier truncated to 255 characters
#pragma warning(disable: 4786)

#include <memory.h>
#include <stdexcept>
#include <assert.h>

#include "Base64UrlEncode.h"

namespace k2s {

namespace ine {

#include "CyoDecode.h"
#include "CyoEncode.h"

#ifdef WIN32
#include <wininet.h>
#endif	// WIN32

#ifdef _Mac
#endif	// _Mac

#include "CINetUtil.h"
#include "CINet.h"


}	// namespace ine

}	// namespace k2s


using namespace k2s;

using namespace ine;
