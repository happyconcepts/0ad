/* Copyright (C) 2009 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * windows-specific module init and shutdown mechanism
 */

#include "precompiled.h"
#include "winit.h"

#include "win.h"	// GetTickCount for quick'n dirty timing

// see http://blogs.msdn.com/larryosterman/archive/2004/09/27/234840.aspx
// for discussion of a similar mechanism.
//
// note: this module is kept distinct from the CRT's init/shutdown mechanism
// to insulate against changes there. another advantage is that callbacks
// can return LibError instead of int.

// currently (2008-02-17) the init groups are populated as follows:
//   critical : wposix
//   early    : wutil
//   early2   : whrt, wdbg_heap
//   main     : waio, wsock, wtime, wdir_watch
//   late     : wsdl

typedef LibError (*PfnLibError)();

// pointers to start and end of function tables.
// notes:
// - COFF tosses out empty segments, so we have to put in one value
//   (zero, because CallFunctionPointers has to ignore entries =0 anyway).
// - ASCII '$' and 'Z' come before resp. after '0'..'9', so use that to
//   bound the section names.
__declspec(allocate(".WINIT$I$")) PfnLibError initBegin = 0;
__declspec(allocate(".WINIT$IZ")) PfnLibError initEnd = 0;
__declspec(allocate(".WINIT$S$")) PfnLibError shutdownBegin = 0;
__declspec(allocate(".WINIT$SZ")) PfnLibError shutdownEnd = 0;
// note: #pragma comment(linker, "/include") is not necessary since
// these are referenced below.


/**
 * call into a range of function pointers.
 * @param [begin, end): STL-style range
 *
 * note: pointers = 0 are ignored. this is because the above placeholders
 * are initialized to 0 and because the range may be larger than
 * expected due to COFF section padding (with zeroes).
 **/
static void CallFunctionPointers(PfnLibError* begin, PfnLibError* end)
{
	const DWORD t0 = GetTickCount();

	for(PfnLibError* ppfunc = begin; ppfunc < end; ppfunc++)
	{
		if(*ppfunc)
		{
			(*ppfunc)();
		}
	}

	const DWORD t1 = GetTickCount();
	debug_printf(L"WINIT| total elapsed time in callbacks %d ms (+-10)\n", t1-t0);
}


void winit_CallInitFunctions()
{
	CallFunctionPointers(&initBegin, &initEnd);
}

void winit_CallShutdownFunctions()
{
	CallFunctionPointers(&shutdownBegin, &shutdownEnd);
}
