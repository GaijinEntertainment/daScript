// ---
//
// $Id: utils.cpp,v 1.2 2004/02/04 21:08:23 nilu Exp $
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


#if (defined(__WIN32__) || defined(WIN32))
# include "winconfig.h"
#else
# include "config.h"
#endif
#include "missing.h"
#include "utils.h"

#ifdef HAVE_ROUND
	#include <cmath>
	using namespace std;
#endif

namespace Test
{	
	// Computes the procentage of correct tests.
	//
	int
	correct(int tests, int errors)
	{
		return static_cast<int>(
			round((static_cast<double>(tests - errors) / tests) * 100.0));
	}

} // namespace Test
