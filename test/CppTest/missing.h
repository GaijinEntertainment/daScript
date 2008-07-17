// ---
//
// $Id: missing.h,v 1.4 2008/07/15 20:33:31 hartwork Exp $
//
// CppTest - A C++ Unit Testing Framework
// Copyright (c) 2003 Niklas Lundell
//
// ---
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// ---

#ifndef CPPTEST_MISSING_H
#define CPPTEST_MISSING_H

#if (defined(__WIN32__) || defined(WIN32))
# include "winconfig.h"
#else
# include "config.h"
#endif 

namespace Test
{
#ifndef HAVE_GETTIMEOFDAY

	struct timeval
	{
		long tv_sec;
		long tv_usec;
	};
		
	extern int gettimeofday(timeval* tv, void*);
		
#endif // #ifndef HAVE_GETTIMEOFDAY
		
#ifndef HAVE_ROUND
	extern double round(double d);
#endif	
	
} // namespace Test

#endif // #ifndef CPPTEST_MISSING_H
