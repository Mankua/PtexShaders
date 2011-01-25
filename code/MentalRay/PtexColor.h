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
