#include <math.h>
#include <assert.h>
#include <shader.h>

#include "PtexColor.h"
#include "Ptexture.h"

#include <map>
#include <string>

char* miaux_tag_to_string( miTag tag, char *default_value )
{
	char *result = default_value;
	if (tag != 0)
	{
		result = (char*)mi_db_access(tag);
		mi_db_unpin(tag);
	}
	return result;
}

static miLock mr_lock;

struct PtexColorData
{
	std::string ptex_file_path;
	PtexCache * ptex_cache;
	PtexTexture * ptex_texture;
	int ptex_num_channels;
	std::map< int, PtexFilter * > ptex_filters;
};

std::map< std::string , PtexColorData * > ptex_datas;

extern "C" DLLEXPORT int PtexColor_version(void){ return 1; }

extern "C" DLLEXPORT void PtexColor_init( miState * state, PtexColor_params * in_params, miBoolean * inst_req )
{
	if ( !in_params )
	{
		*inst_req = miFALSE;
	}

	mi_init_lock( &mr_lock );
}

extern "C" DLLEXPORT void PtexColor_exit( miState * state, PtexColor_params * in_params )
{
	mi_delete_lock( &mr_lock );
}

extern "C" DLLEXPORT miBoolean PtexColor( miColor * out_result, miState * state, PtexColor_params * in_params )
{
	int thread_id = (int)state->thread;
	PtexFilter * ptex_filter = 0;

	miTag ptex_file_path_tag = *mi_eval_tag( &in_params->ptex_file_path );
	miInteger uv_index = *mi_eval_integer( &in_params->uv_index );
	miInteger filter_id = *mi_eval_integer( &in_params->filter_type );
	miScalar filter_size = *mi_eval_scalar( &in_params->filter_size );

	uv_index = 0;

	std::string ptex_file_path = miaux_tag_to_string( ptex_file_path_tag, "c:\\Temp" );

	std::map< std::string, PtexColorData * >::iterator it_data = ptex_datas.find( ptex_file_path );

	PtexColorData * ptex_color_data = 0;

	if ( it_data == ptex_datas.end() )
	{
		// It's possible that multiple threads enter this if statement at the same time, so
		// we double check inside the critical section to make sure that only one data is 
		// created for this file_path.

		mi_lock( mr_lock );

		std::map< std::string, PtexColorData * >::iterator it_data_2 = ptex_datas.find( ptex_file_path );

		if ( it_data_2 == ptex_datas.end() )
		{
			Ptex::String error;

			PtexCache * ptex_cache = PtexCache::create( 0, 1024 * 1024 );

			PtexTexture * ptex_texture = ptex_cache->get( ptex_file_path.c_str(), error );

			if ( ptex_texture )
			{
				ptex_color_data = new PtexColorData;

				ptex_color_data->ptex_file_path = ptex_file_path;
				ptex_color_data->ptex_cache = ptex_cache;
				ptex_color_data->ptex_texture = ptex_texture;
				ptex_color_data->ptex_num_channels = ptex_texture->numChannels();

				ptex_datas.insert( std::pair< std::string, PtexColorData * >( ptex_file_path, ptex_color_data ) );
			}
		}

		mi_unlock( mr_lock );

		std::map< std::string, PtexColorData * >::iterator it_data_3 = ptex_datas.find( ptex_file_path );

		ptex_color_data = it_data_3->second;
	}
	else
	{
		ptex_color_data = it_data->second;
	}

	std::map< int, PtexFilter * >::iterator it_filter = ptex_color_data->ptex_filters.find( thread_id );

	if ( it_filter == ptex_color_data->ptex_filters.end() )
	{
		// It's possible that multiple threads enter this if statement at the same time, so
		// we double check inside the critical section to make sure that only one filter is 
		// created for this texture.

		mi_lock( mr_lock );

		std::map< int, PtexFilter * >::iterator it_filter_2 = ptex_color_data->ptex_filters.find( thread_id );

		if ( it_filter_2 == ptex_color_data->ptex_filters.end() )
		{
			PtexFilter::FilterType filter_type = PtexFilter::f_point;

			switch ( filter_id )
			{
				case 0: filter_type = PtexFilter::f_point;      break;
				case 1: filter_type = PtexFilter::f_bilinear;   break;
				case 2: filter_type = PtexFilter::f_box;        break;
				case 3: filter_type = PtexFilter::f_gaussian;   break;
				case 4: filter_type = PtexFilter::f_bicubic;    break;
				case 5: filter_type = PtexFilter::f_bspline;    break;
				case 6: filter_type = PtexFilter::f_catmullrom; break;
				case 7: filter_type = PtexFilter::f_mitchell;   break;
			}

			PtexFilter::Options opts( filter_type, 0, filter_size );
			ptex_filter = PtexFilter::getFilter( ptex_color_data->ptex_texture, opts );

			ptex_color_data->ptex_filters.insert( std::pair< int, PtexFilter * >( thread_id, ptex_filter ) );

			//

			float u = state->tex_list[ uv_index ].x;
			float v = state->tex_list[ uv_index ].y;
			float w = state->tex_list[ uv_index ].z;

			miVector deriv = state->derivs[ uv_index ];

			float du = deriv.x;
			float dv = deriv.y;
			float dw = deriv.z;
		}

		mi_unlock( mr_lock );

		// And if the second thread was stopped by the second if, make sure to grab a filter...

		std::map< int, PtexFilter * >::iterator it_filter_3 = ptex_color_data->ptex_filters.find( thread_id );

		ptex_filter = it_filter_3->second;
	}
	else
	{
		ptex_filter = it_filter->second;
	}

	float u = state->tex_list[ uv_index ].x;
	float v = state->tex_list[ uv_index ].y;
	float w = state->tex_list[ uv_index ].z;

	float du = 0.0f;
	float dv = 0.0f;

	int face_id = (int)( w + 0.5f );

	float result[4];
	ptex_filter->eval( result, 0, ptex_color_data->ptex_num_channels, face_id, u, v, du, 0, 0, dv );

	miColor out_color;

	out_color.r = result[ 0 ];
	out_color.g = result[ 1 ];
	out_color.b = result[ 2 ];
	out_color.a = 1.0;

	*out_result = out_color;

	return miTRUE;
}

