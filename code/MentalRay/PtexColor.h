/*-------------------------------------------------------------------------*\
-----------------------------------------------------------------------------
Ptex Shaders
http://www.mankua.com/Ptex/Ptex.php

Author : Diego A. Castaño
Copyright : (c) 2004-2011 Mankua Software Inc.

Licence : ZLib licence :

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
-----------------------------------------------------------------------------
\*-------------------------------------------------------------------------*/

#ifndef __PTEX_COLOR_H_
#define __PTEX_COLOR_H_

#include <math.h>
#include <shader.h>

typedef struct
{
	miTag ptex_file_path;
	miInteger uv_index;
	miInteger filter_type;
	miScalar filter_size;
} 
PtexColor_params;

extern "C"
{
	DLLEXPORT int PtexColor_version(void);
	DLLEXPORT miBoolean PtexColor( miColor *out_result, miState *state, PtexColor_params *in_params );
	DLLEXPORT void PtexColor_init( miState * state, PtexColor_params * in_params, miBoolean * inst_req );
	DLLEXPORT void PtexColor_exit( miState * state, PtexColor_params * in_params );
}
#endif
