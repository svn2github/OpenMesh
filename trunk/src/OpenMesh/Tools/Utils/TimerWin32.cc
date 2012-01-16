/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2011 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------* 
 *  This file is part of OpenMesh.                                           *
 *                                                                           *
 *  OpenMesh is free software: you can redistribute it and/or modify         * 
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenMesh is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenMesh.  If not,                                    *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/ 

/*===========================================================================*\
 *                                                                           *             
 *   $Revision: 485 $                                                         *
 *   $Date: 2012-01-12 10:15:18 +0100 (Do, 12 Jan 2012) $                   *
 *                                                                           *
\*===========================================================================*/

#ifndef DOXY_IGNORE_THIS
// ----------------------------------------------------------------------------
#include <OpenMesh/Core/System/config.h>
#if defined(OM_CC_MIPS)
#  include <math.h>
#  include <stdio.h>
#else
#  include <cmath>
#  include <cstdio>
#endif
#include "Timer.hh"
// ----------------------------------------------------------------------------

// ------------------------------------------------------------- namespace ----

namespace OpenMesh {
namespace Utils {


// ----------------------------------------------------------------------------

using namespace std;

// ------------------------------------------------------------- windows 32 ----
#if defined(WIN32) && (defined(_MSC_VER) || defined(__INTEL_COMPILER))

#ifndef DOXY_IGNORE_THIS
#include <windows.h>
#endif

class TimerImplWin32 : public TimerImpl
{
protected:
   LARGE_INTEGER freq_;
   LARGE_INTEGER count_;
   LARGE_INTEGER start_;

public:
   TimerImplWin32(void);
   ~TimerImplWin32(void) { ; }

   virtual void   reset(void);
   virtual void   start(void);
   virtual void   stop(void);
   virtual void   cont(void);
   virtual double seconds(void) const;
};

TimerImplWin32::TimerImplWin32(void)
{
   if (QueryPerformanceFrequency(&freq_)==FALSE)
     throw std::runtime_error("Performance counter of of stock!");
   reset();
}

void TimerImplWin32::reset(void)
{
   memset(&count_,0,sizeof(count_));
   memset(&start_,0,sizeof(count_));
}

void TimerImplWin32::start(void)
{
   reset();
   QueryPerformanceCounter(&start_);
}

void TimerImplWin32::stop(void)
{
   LARGE_INTEGER stop_;

   QueryPerformanceCounter(&stop_);
   count_.QuadPart += stop_.QuadPart - start_.QuadPart;
}

void TimerImplWin32::cont(void)
{
   QueryPerformanceCounter(&start_);
}

double TimerImplWin32::seconds(void) const
{
   return (double)count_.QuadPart/(double)freq_.QuadPart;
}

#endif

// ============================================================================
} // END_NS_UTILS
} // END_NS_OPENMESH
// ----------------------------------------------------------------------------
#endif // DOXY_IGNORE_THIS
// ============================================================================
// end of file Timer.cc
// ============================================================================

