// ---
//
// $Id: winconfig.h,v 1.1 2008/07/15 20:33:31 hartwork Exp $
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

#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

// Capabilities
//
#define HAVE_GETTICKCOUNT

// Compiler specific stuff
//
#if _MSC_VER == 1200					// MS Visual C++ 6.0
	#pragma warning (disable: 4786)
#endif
	
#if _MSC_VER > 1300						// MS Visual C++ .NET 2002 and above
	#pragma warning (disable: 4267)
#endif

#endif // #ifndef TEST_CONFIG_H
