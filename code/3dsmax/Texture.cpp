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

#include "Texture.h"
#include "Ptexture.h"
#include "About.h"
#include "AssetManagement/iassetmanager.h"
#include "AssetManagement/AssetType.h"
#include "assetmanagement/AssetUser.h"
#include <IPathConfigMgr.h>

#define SCANLINERENDERER_CLASS_ID				Class_ID(SREND_CLASS_ID,0)

static AColor black( 0.0f, 0.0f, 0.0f, 0.0f );

class TexturePtexClassDesc : public ClassDesc2, public IMtlRender_Compatibility_MtlBase
{
public:

	TexturePtexClassDesc();

	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new TexturePtex(); }
	const TCHAR *	ClassName() { return GetString(IDS_TEXTURE_NAME); }
	SClass_ID		SuperClassID() { return TEXMAP_CLASS_ID; }
	Class_ID		ClassID() { return TEXTURE_PTEX_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("PtexColor"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }					// returns owning module handle

	// -- from IMtlRender_Compability_MtlBase
	virtual bool IsCompatibleWithRenderer(ClassDesc& rendererClassDesc);
};

static TexturePtexClassDesc TexturePtexDesc;
ClassDesc2 * GetTexturePtexDesc() { return &TexturePtexDesc; }

TexturePtexClassDesc::TexturePtexClassDesc() {

	IMtlRender_Compatibility_MtlBase::Init( *this );
}

bool TexturePtexClassDesc::IsCompatibleWithRenderer(ClassDesc& rendererClassDesc)
{
	Class_ID classID = rendererClassDesc.ClassID();

	if ( classID == MRRENDERER_CLASSID || classID == SCANLINERENDERER_CLASS_ID )
	{
		return true;
	}
	else 
	{
		// Return 'true' only if the renderer doesn't implement the compatibility interface.
		// This ensures that we are compatible with all renderers unless they specify the contrary.
		IMtlRender_Compatibility_Renderer* rendererCompatibility = Get_IMtlRender_Compatibility_Renderer(rendererClassDesc);
		return (rendererCompatibility == NULL);
	}
}

ParamDlg * TexturePtex::m_texture_output_dlg = 0;

class TexturePtexPBAccessor : public PBAccessor
{
	void Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t)
	{
		TexturePtex * tex = (TexturePtex*)owner;
		IParamMap2* pmap = tex->m_pblock->GetMap();

		TSTR dir, name, extension, path;

		switch(id)
		{
			case TexturePtex::PTEX_FILE:
			{
			}
			break;

			default:
			break;
		}

		GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime());		//this forces the effect to be re-evaluated
	}
};

TexturePtexPBAccessor pb_accessor;

static ParamBlockDesc2 texture_ptex_param_blk ( TexturePtex::PBLOCK, _T("params"),  0, &TexturePtexDesc, 
	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
	//rollout
	IDD_TEXTURE, IDS_PARAMS, 0, 0, NULL,
	// params

	TexturePtex::PTEX_FILE,		_T("ptexFile"),		TYPE_FILENAME,	0,				IDS_SPIN,
		p_assetTypeID,	MaxSDK::AssetManagement::AssetType::kOtherAsset,
		p_accessor, &pb_accessor,
		end,

	TexturePtex::UVW_TYPE,		_T("uvwSource"),	TYPE_INT,		0,				IDS_SPIN,
		end,

	TexturePtex::UVW_CHANNEL, 	_T("uvwChannel"), 	TYPE_INT, 		0, 				IDS_SPIN, 
		p_default, 		1, 
		p_range, 		1,					100, 
		p_ui, 			TYPE_SPINNER,		EDITTYPE_INT,	IDC_TEX_UVW_CHANNEL,	IDC_TEX_UVW_CHANNEL_SPIN,	1.0f, 
		end,

	TexturePtex::FILTER_SIZE, 	_T("filterSize"), 	TYPE_FLOAT, 	0, 				IDS_SPIN, 
		p_default, 		1.0f, 
		p_range, 		0.0f,				1.0f, 
		p_ui, 			TYPE_SPINNER,		EDITTYPE_FLOAT, IDC_TEX_FITER_SIZE,		IDC_TEX_FITER_SIZE_SPIN,	0.001f,
		end,

	TexturePtex::FILTER_TYPE,	_T("filterType"),	TYPE_INT,		0,				IDS_SPIN,
		p_default, 		1, 
		end,

	//TexturePtex::BUMP_AMOUNT, 	_T("bumpAmount"), 	TYPE_FLOAT, 	P_ANIMATABLE,	IDS_SPIN, 
	//	p_default, 		1.0f, 
	//	p_range, 		0.0f,				9999.9f,
	//	p_ui, 			TYPE_SPINNER,		EDITTYPE_FLOAT, IDC_TEX_BUMP_AMOUNT,	IDC_TEX_BUMP_AMOUNT_SPIN,	0.01f,
	//	end,

	TexturePtex::OUTPUT_REF,	_T("output"),		TYPE_REFTARG,	P_OWNERS_REF,	IDS_SPIN,
		p_refno,		TexturePtex::TEXTURE_OUTPUT,
		end,

	TexturePtex::PREMULTIPLIED_ALPHA, 	_T("premultipliedAlpha"), 	TYPE_BOOL, 	0, 				IDS_SPIN, 
		p_default, 		FALSE, 
		p_ui,			TYPE_SINGLECHEKBOX,		IDC_TEX_PREMULTIPLIED_ALPHA,
		end,

	end
);

class PtexFileAccessor : public IAssetAccessor
{
public:

	PtexFileAccessor( TexturePtex * texture_ptex ) : m_texture_ptex( texture_ptex ) {}

	virtual MaxSDK::AssetManagement::AssetType GetAssetType() const	{ return MaxSDK::AssetManagement::kBitmapAsset; }

	// path accessor functions
	virtual MaxSDK::AssetManagement::AssetUser GetAsset() const
	{
		return m_texture_ptex->GetPtexFile();
	}

	virtual bool SetAsset( const MaxSDK::AssetManagement::AssetUser& aNewAsset )
	{
		m_texture_ptex->SetPtexFile( aNewAsset );
		return true;
	}

protected:

	TexturePtex * m_texture_ptex;
};

//

class TexturePtexDlgProc : public ParamMap2UserDlgProc
{
public:

	HWND m_hwnd;
	TexturePtex * m_tex;	
	ICustButton * m_file_button;

	TexturePtexDlgProc( TexturePtex * tex ) 
	{
		m_tex = tex;

		m_file_button = NULL;
	}		

	~TexturePtexDlgProc()
	{
		Destroy();
	}

	void Destroy() 
	{
		if ( m_file_button )
		{
			ReleaseICustButton( m_file_button );
			m_file_button = NULL; 
		}
	}

	INT_PTR DlgProc( TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	void DeleteThis() { delete this; }
};

INT_PTR TexturePtexDlgProc::DlgProc( TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	m_hwnd = hWnd;

	switch (msg) 
	{
		case WM_INITDIALOG:
		{
			// Init File Button
			m_file_button = GetICustButton( GetDlgItem( hWnd, IDC_TEX_PTEX_FILE ) );

			MSTR ptex_filename = m_tex->m_pblock->GetStr( TexturePtex::PTEX_FILE );

			if ( ptex_filename.length() == 0 )
			{
				m_file_button->SetText( _T("None") );
			}
			else
			{
				m_file_button->SetTooltip( true, ptex_filename.data() );

				if ( ptex_filename.length() > 42 )
				{
					ptex_filename.remove( 0, ptex_filename.length() - 42 );
				}

				m_file_button->SetText( ptex_filename );
			}

			// Init UVW Source

			int uvw_type = m_tex->m_pblock->GetInt( TexturePtex::UVW_TYPE );

			SendDlgItemMessage( hWnd, IDC_TEX_UVW_TYPE, CB_RESETCONTENT, 0, 0 );
			SendDlgItemMessage( hWnd, IDC_TEX_UVW_TYPE, CB_ADDSTRING, 0, (LPARAM)(const TCHAR*)_T("Explicit Map Channel"));
			SendDlgItemMessage( hWnd, IDC_TEX_UVW_TYPE, CB_ADDSTRING, 0, (LPARAM)(const TCHAR*)_T("Vertex Color Channel"));
			SendDlgItemMessage( hWnd, IDC_TEX_UVW_TYPE, CB_SETCURSEL, uvw_type, 0 );

			// Init Ptex filter

			int filter_type = m_tex->m_pblock->GetInt( TexturePtex::FILTER_TYPE );

			SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_RESETCONTENT, 0, 0 );
			SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_ADDSTRING, 0, (LPARAM)(const TCHAR*)_T("Point"));
			SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_ADDSTRING, 0, (LPARAM)(const TCHAR*)_T("Bilinear"));
			SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_ADDSTRING, 0, (LPARAM)(const TCHAR*)_T("Box"));
			SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_ADDSTRING, 0, (LPARAM)(const TCHAR*)_T("Gaussian"));
			SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_ADDSTRING, 0, (LPARAM)(const TCHAR*)_T("Bicubic"));
			SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_ADDSTRING, 0, (LPARAM)(const TCHAR*)_T("Bspline"));
			SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_ADDSTRING, 0, (LPARAM)(const TCHAR*)_T("Catmull Rom"));
			SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_ADDSTRING, 0, (LPARAM)(const TCHAR*)_T("Mitchell"));
			SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_SETCURSEL, filter_type, 0 );
		}
		break;

		case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) ) 
			{
				case IDC_TEX_PTEX_FILE:
				{
					FilterList filter_list;
					filter_list.Append( _M("Ptex files(*.ptx)"));
					filter_list.Append( _M("*.ptx"));

					MSTR ptex_filename;
					MSTR initial_dir( _M( "c://Temp" ) );

					if ( GetCOREInterface8()->DoMaxOpenDialog( hWnd, "Open Ptex file", ptex_filename, initial_dir, filter_list ) == true )
					{
						m_tex->m_pblock->SetValue( TexturePtex::PTEX_FILE, 0, ptex_filename );
						m_file_button->SetText( ptex_filename );
					}
				}
				break;

				case IDC_TEX_UVW_TYPE:
				{
					int uvw_type = SendDlgItemMessage( hWnd, IDC_TEX_UVW_TYPE, CB_GETCURSEL, 0, 0 );
					m_tex->m_pblock->SetValue( TexturePtex::UVW_TYPE, 0, uvw_type );
				}
				break;

				case IDC_TEX_FILTER_TYPE:
				{
					int filter_id = SendDlgItemMessage( hWnd, IDC_TEX_FILTER_TYPE, CB_GETCURSEL, 0, 0 );
					m_tex->m_pblock->SetValue( TexturePtex::FILTER_TYPE, 0, filter_id );
				}
				break;

				case IDC_TEX_ABOUT:
				{
					DialogBoxParam(	hInstance, MAKEINTRESOURCE(IDD_ABOUT), GetCOREInterface()->GetMAXHWnd(), AboutDlgProc, 0);
				}
				break;

				case IDC_TEX_HELP:
				{
					ShellExecute( NULL, "open", "http://www.mankua.com/Stripes/Stripes.php", NULL, NULL, SW_SHOWNORMAL );
				}
				break;
			}
		}
		break;

		//case CC_SPINNER_CHANGE:
		//{
		//	switch( LOWORD(wParam) )
		//	{
		//		case IDC_TEX_FITER_SIZE_SPIN:
		//		{
		//		}
		//		break;
		//	}
		//}
		//break;

		case WM_DESTROY:
		{
			Destroy();
		}
		break;
	}

	return 0;
}

//--- TexturePtex -------------------------------------------------------

TexturePtex::TexturePtex()
{
	m_ptex_cache = 0;
	m_ptex_texture = 0;

	InitializeCriticalSection(&m_critical_section);

	m_pblock = NULL;
	m_texture_output = NULL;
	TexturePtexDesc.MakeAutoParamBlocks(this);
	Reset();
}

TexturePtex::~TexturePtex()
{
	DeleteCriticalSection(&m_critical_section);
}

//MSTR TexturePtex::GetFullName( TSTR& s )
//{
//	MSTR class_name;
//	GetClassName( class_name );
//
//	MSTR ptex_filename = m_pblock->GetStr( PTEX_FILE );
//
//	if ( ptex_filename.length() < 3 ) return class_name;
//
//	MSTR dir, name, extension, path;
//	SplitFilename( ptex_filename, &dir, &name, &extension );
//	
//	MSTR filename = name + extension;
//
//	return filename;
//}

//From MtlBase
void TexturePtex::Reset() 
{
	if ( m_texture_output )
	{
		m_texture_output->Reset();
	}
	else
	{
		ReplaceReference( TEXTURE_OUTPUT, GetNewDefaultTextureOutput() );
	}

	m_interval.SetEmpty();
}

Interval TexturePtex::Validity(TimeValue t)
{
	//TODO: Update m_interval here
	return m_interval;
}

ParamDlg* TexturePtex::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp) 
{
	IAutoMParamDlg* masterDlg = TexturePtexDesc.CreateParamDlgs( hwMtlEdit, imp, this );

	texture_ptex_param_blk.SetUserDlgProc( new TexturePtexDlgProc( this ) );

	m_texture_output_dlg = m_texture_output->CreateParamDlg( hwMtlEdit, imp );

	masterDlg->AddDlg( m_texture_output_dlg );

	return masterDlg;	
}

BOOL TexturePtex::SetDlgThing(ParamDlg* dlg)
{
	if ( ( m_texture_output_dlg != NULL ) && ( dlg == m_texture_output_dlg ) )
	{
		m_texture_output_dlg->SetThing( m_texture_output );
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//From ReferenceMaker
RefTargetHandle TexturePtex::GetReference(int i) 
{
	switch (i) 
	{
		case PBLOCK: return m_pblock;
		case TEXTURE_OUTPUT: return m_texture_output;
		default: return 0;
	}
}

void TexturePtex::SetReference(int i, RefTargetHandle rtarg) 
{
	switch(i) 
	{
		case PBLOCK:
			m_pblock = (IParamBlock2 *)rtarg;
		break;

		case TEXTURE_OUTPUT:
			m_texture_output = (TextureOutput *)rtarg;
		break;
	}
}

MaxSDK::AssetManagement::AssetUser TexturePtex::GetPtexFile()const
{
	return m_pblock->GetAssetUser( PTEX_FILE, 0 );
}

void TexturePtex::SetPtexFile( const MaxSDK::AssetManagement::AssetUser& file )
{
	m_pblock->SetValue( PTEX_FILE, 0, file );
}

//From ReferenceTarget 
RefTargetHandle TexturePtex::Clone(RemapDir &remap) 
{
	TexturePtex * mnew = new TexturePtex();
	*((MtlBase*)mnew) = *((MtlBase*)this);

	mnew->ReplaceReference( PBLOCK, remap.CloneRef( m_pblock ) );
	mnew->ReplaceReference( TEXTURE_OUTPUT, remap.CloneRef( m_texture_output ) );

	BaseClone( this, mnew, remap );
	return (RefTargetHandle)mnew;
}

Animatable* TexturePtex::SubAnim(int i) 
{
	switch (i)
	{
		case PBLOCK: return m_pblock;
		case TEXTURE_OUTPUT: return m_texture_output;
		default: return 0;
	}
}

TSTR TexturePtex::SubAnimName(int i) 
{
	switch (i)
	{
		case PBLOCK: return GetString(IDS_PARAMS);		
		case TEXTURE_OUTPUT: return GetString(IDS_TEXTURE_OUTPUT);		
		default: return TSTR( _T("") );
	}
}

RefResult TexturePtex::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message ) 
{
	return(REF_SUCCEED);
}

IOResult TexturePtex::Save(ISave *isave) 
{
	return IO_OK;
}

IOResult TexturePtex::Load(ILoad *iload) 
{
	return IO_OK;
}

void TexturePtex::EnumAuxFiles(AssetEnumCallback& nameEnum, DWORD flags)
{
	if ( ( flags & FILE_ENUM_CHECK_AWORK1 ) && TestAFlag( A_WORK1 ) ) return; 
	if ( ( flags & FILE_ENUM_INACTIVE ) == false ) return; // not needed by renderer

	if(flags & FILE_ENUM_ACCESSOR_INTERFACE)
	{
		PtexFileAccessor accessor( this );

		if( accessor.GetAsset().GetId() != MaxSDK::AssetManagement::kInvalidId )
		{
			IEnumAuxAssetsCallback* callback = static_cast<IEnumAuxAssetsCallback*>(&nameEnum);
			callback->DeclareAsset( accessor );
		}
	}
	else
	{
		if ( GetPtexFile().GetId() != MaxSDK::AssetManagement::kInvalidId ) 
		{
			//TO DO: should RecordInput take an asset id?
			IPathConfigMgr::GetPathConfigMgr()->RecordInputAsset( GetPtexFile(), nameEnum, flags );
		}

		ReferenceTarget::EnumAuxFiles( nameEnum, flags ); // LAM - 4/21/03
	}

	ReferenceTarget::EnumAuxFiles(nameEnum, flags);
}

ULONG TexturePtex::LocalRequirements( int subMtlNum )
{
	return 1;
}

void TexturePtex::LocalMappingsRequired( int  subMtlNum, BitArray &mapreq, BitArray &bumpreq )
{
	if   ( m_pblock->GetInt( UVW_TYPE ) == 0 ) m_uv_channel = m_pblock->GetInt( UVW_CHANNEL );
	else                                       m_uv_channel = 0;

	mapreq.Set( m_uv_channel );
	bumpreq.Set( m_uv_channel );
}

void TexturePtex::Update(TimeValue t, Interval& valid) 
{
	if   ( m_pblock->GetInt( UVW_TYPE ) == 0 ) m_uv_channel = m_pblock->GetInt( UVW_CHANNEL );
	else                                       m_uv_channel = 0;

	m_ptex_file_path = m_pblock->GetStr( PTEX_FILE );
	m_filter_id = m_pblock->GetInt( FILTER_TYPE );
	m_filter_size = m_pblock->GetFloat( FILTER_SIZE );
	m_premultiplied_alpha = m_pblock->GetInt( PREMULTIPLIED_ALPHA ) > 0;
}

int TexturePtex::RenderBegin(TimeValue t, ULONG flags) 
{
	Ptex::String error;

	MSTR ptex_filename = m_pblock->GetStr( PTEX_FILE );

	if ( ptex_filename.length() == 0 ) return 0;

	m_ptex_cache = PtexCache::create( 0, 1024 * 1024 );

	m_ptex_texture = m_ptex_cache->get( ptex_filename.data(), error );

	if ( m_ptex_texture == 0 ) return 0;

	m_ptex_num_channels = m_ptex_texture->numChannels();

	return 1;
}

AColor TexturePtex::EvalColor(ShadeContext& sc)
{
	if ( sc.doMaps == 0 ) return black;

	if ( m_ptex_texture == 0 ) return black;

	unsigned int thread_id = (unsigned int)GetCurrentThreadId();
	PtexFilter * ptex_filter = 0;

	std::map< unsigned int, PtexFilter * >::iterator it = m_ptex_filters.find( thread_id );

	if ( it == m_ptex_filters.end() )
	{
		EnterCriticalSection( &m_critical_section );

		// It's possible that multiple threads enter this if statement at the same time, so
		// we double check inside the critical section to make sure that only one filter is 
		// created for this texture.

		std::map< unsigned int, PtexFilter * >::iterator it2 = m_ptex_filters.find( thread_id );

		if ( it2 == m_ptex_filters.end() )
		{
			PtexFilter::FilterType filter_type = PtexFilter::f_point;

			switch ( m_filter_id )
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

			PtexFilter::Options opts( filter_type, 0, m_filter_size );
			ptex_filter = PtexFilter::getFilter( m_ptex_texture, opts );

			m_ptex_filters.insert( std::pair< unsigned int, PtexFilter * >( thread_id, ptex_filter ) );
		}

		LeaveCriticalSection( &m_critical_section );

		// And if the second thread was stopped by the second if, make sure to grab a filter...

		std::map< unsigned int, PtexFilter * >::iterator it3 = m_ptex_filters.find( thread_id );

		ptex_filter = it3->second;
	}
	else
	{
		ptex_filter = it->second;
	}

	DbgAssert( ptex_filter );

	float result[4];

	result[ 0 ] = 1.0f;
	result[ 1 ] = 0.0f;
	result[ 2 ] = 0.0f;
	result[ 3 ] = 1.0f;

	int face_id = (int)sc.UVW( m_uv_channel ).x;

	float u = sc.UVW( m_uv_channel ).x;
	float v = sc.UVW( m_uv_channel ).y;

	u = u - float( face_id );

	float du = sc.DUVW( m_uv_channel ).x;
	float dv = sc.DUVW( m_uv_channel ).y;

	int x = sc.ScreenCoord().x;
	int y = sc.ScreenCoord().y;
	
	ptex_filter->eval( result, 0, m_ptex_num_channels, face_id, u, v, du, 0, 0, dv );

	AColor color;
	
	if ( m_ptex_num_channels == 1 )
	{
		color.r = color.g = color.b = result[ 0 ];
		color.a = 1.0f;
	}
	else if ( m_ptex_num_channels == 3 )
	{
		color.r = result[ 0 ];
		color.g = result[ 1 ];
		color.b = result[ 2 ];
		color.a = 1.0f;
	}
	else if ( m_ptex_num_channels == 4 )
	{
		color.r = result[ 0 ];
		color.g = result[ 1 ];
		color.b = result[ 2 ];
		color.a = result[ 3 ];
	}

	if ( m_premultiplied_alpha == false )
	{
		color.r *= color.a;
		color.g *= color.a;
		color.b *= color.a;
	}

	color = m_texture_output->Filter( color );

	return color;
}

float TexturePtex::EvalMono(ShadeContext& sc)
{
	if ( sc.doMaps == 0 ) return 0.0f;

	float f = Intens( EvalColor( sc ) );

	f = m_texture_output->Filter( f );

	return f;
}

float inv[ 3 ] = { 0.0f, 1.0f, 0.5f };

Point3 TexturePtex::EvalNormalPerturb( ShadeContext& sc )
{
	if ( sc.doMaps == 0 ) return Point3(0, 0, 0);

	if ( m_ptex_texture == 0 ) return Point3(0, 0, 0);

	unsigned int thread_id = (unsigned int)GetCurrentThreadId();
	PtexFilter * ptex_filter = 0;

	std::map< unsigned int, PtexFilter * >::iterator it = m_ptex_filters.find( thread_id );

	if ( it == m_ptex_filters.end() )
	{
		EnterCriticalSection(&m_critical_section);

		// It's possible that multiple threads enter this if statement at the same time, so
		// we double check inside the critical section to make sure that only one filter is 
		// created for this texture.

		std::map< unsigned int, PtexFilter * >::iterator it2 = m_ptex_filters.find( thread_id );

		if ( it2 == m_ptex_filters.end() )
		{
			PtexFilter::FilterType filter_type = PtexFilter::f_point;

			switch ( m_filter_id )
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

			PtexFilter::Options opts( filter_type, 0, m_filter_size );
			ptex_filter = PtexFilter::getFilter( m_ptex_texture, opts );

			m_ptex_filters.insert( std::pair< unsigned int, PtexFilter * >( thread_id, ptex_filter ) );
		}

		LeaveCriticalSection(&m_critical_section);

		// And if the second thread was stopped by the second if, make sure to grab a filter...

		std::map< unsigned int, PtexFilter * >::iterator it3 = m_ptex_filters.find( thread_id );

		ptex_filter = it3->second;
	}
	else
	{
		ptex_filter = it->second;
	}

	DbgAssert( ptex_filter );

	int x = sc.ScreenCoord().x;
	int y = sc.ScreenCoord().y;

	int face_id = (int)sc.UVW( m_uv_channel ).x;

	Point3 uv = sc.UVW( m_uv_channel );
	Point3 duv = sc.DUVW( m_uv_channel );

	uv.x = uv.x - float( face_id );

	if ( sc.IsSuperSampleOn() && sc.IsTextureSuperSampleOn() && sc.GetSampleSizeScale() > 0.0f )
	{
		duv /= sc.GetSampleSizeScale() / 1.2f;
	}

	Point3 dp[2];
	sc.BumpBasisVectors( dp, AXIS_UV, m_uv_channel );

	Point2 dm( 0.0f, 0.0f );

	float result[4];

	ptex_filter->eval( result, 0, 3, face_id, uv.x, uv.y, duv.x, 0, 0, duv.y );

	float c00 = 0.33333333333f * ( result[ 0 ] + result[ 1 ] + result[ 2 ] );

	int nx = 0;
	float sx = 0.0f;

	if ( uv.x + duv.x <= 1.0f )
	{
		ptex_filter->eval( result, 0, 3, face_id, uv.x + duv.x, uv.y, duv.x, 0, 0, duv.y );

		float cp0 = 0.33333333333f * ( result[ 0 ] + result[ 1 ] + result[ 2 ] );

		nx++;
		sx += ( cp0 - c00 ) / duv.x;
	}

	if ( uv.x - duv.x >= 0.0f )
	{
		ptex_filter->eval( result, 0, 3, face_id, uv.x - duv.x, uv.y, duv.x, 0, 0, duv.y );

		float cn0 = 0.33333333333f * ( result[ 0 ] + result[ 1 ] + result[ 2 ] );

		nx++;
		sx += ( c00 - cn0 ) / duv.x;
	}

	dm.x = - sx * inv[ nx ];

	int ny = 0;
	float sy = 0.0f;

	if ( uv.y + duv.y <= 1.0f )
	{
		ptex_filter->eval( result, 0, 3, face_id, uv.x, uv.y + duv.y, duv.x, 0, 0, duv.y );

		float c0p = 0.33333333333f * ( result[ 0 ] + result[ 1 ] + result[ 2 ] );

		ny++;
		sy += ( c0p - c00 ) / duv.y;
	}

	if ( uv.y - duv.y >= 0.0f )
	{
		ptex_filter->eval( result, 0, 3, face_id, uv.x, uv.y - duv.y, duv.x, 0, 0, duv.y );

		float c0n = 0.33333333333f * ( result[ 0 ] + result[ 1 ] + result[ 2 ] );

		ny++;
		sy += ( c00 - c0n ) / duv.y;
	}

	dm.y = - sy * inv[ ny ];

	dm *= 0.01f; // * m_bump_amount;

	Point3 np = dm.x * dp[ 0 ] + dm.y * dp[ 1 ];

	np = m_texture_output->Filter( np );

	return np;
}

int TexturePtex::RenderEnd( TimeValue t ) 
{
	for ( std::map< unsigned int, PtexFilter* >::iterator it =  m_ptex_filters.begin(); it != m_ptex_filters.end(); it++ )
	{
		unsigned int thread_id = it->first;
		PtexFilter * ptex_filter = it->second;

		ptex_filter->release();
		ptex_filter = 0;
	}

	m_ptex_filters.clear();

	if ( m_ptex_texture )
	{
		m_ptex_texture->release();
		m_ptex_texture = 0;
	}

	if ( m_ptex_cache )
	{
		m_ptex_cache->release();
		m_ptex_cache = 0;
	}

	return 5;
}

// Mental Ray 

bool GetParamIDByName( ParamID& paramID, const TCHAR* name, IParamBlock2* mr_pblock )
{
	DbgAssert( mr_pblock != NULL );

	int count = mr_pblock->NumParams();

	for ( int i = 0; i < count; i++ )
	{
		ParamID id = mr_pblock->IndextoID(i);
		const ParamDef& paramDef = mr_pblock->GetParamDef(id);

		char * mr_name = paramDef.int_name;

		if ( _tcsicmp( name, paramDef.int_name ) == 0 )
		{
			paramID = id;
			return true;
		}
	}

	DbgAssert(false);

	return false;
}

template<typename T>
void TranslateMRShaderParameter( IParamBlock2 * mr_pblock, TCHAR* paramName, T value )
{
	ParamID paramID;
	if ( GetParamIDByName( paramID, paramName, mr_pblock ) )
	{
		mr_pblock->SetValue( paramID, 0, value );
	}
	else 
	{
		DbgAssert(false);
	}
}

imrShader* TexturePtex::GetMRShader( imrShaderCreation& shaderCreation ) 
{
	// Create the shader and return it
	imrShader* shader = shaderCreation.CreateShader( _T("PtexColor"), GetFullName() );
	return shader;
}

void TexturePtex::ReleaseMRShader() 
{
}

bool TexturePtex::NeedsAutomaticParamBlock2Translation()
{
	return false;
}
void TexturePtex::TranslateParameters( imrTranslation& translationInterface, imrShader* shader, TimeValue t, Interval& valid )
{
	// Get the parameter block in which we need to store the parameter values
	IParamBlock2 * shader_pblock = shader->GetParametersParamBlock();
	if ( shader_pblock != NULL ) 
	{
		Interval localValid = FOREVER;

		// Update at the current time 
		Update(t, localValid);

		TranslateMRShaderParameter( shader_pblock, _T("filter_size"), m_filter_size );
		TranslateMRShaderParameter( shader_pblock, _T("ptex_file_path"), m_ptex_file_path );
		TranslateMRShaderParameter( shader_pblock, _T("filter_type"), m_filter_id );
		
		ParamID paramID;
		if ( GetParamIDByName( paramID, _T("uv_index"), shader_pblock ) )
		{
			int uv_index = m_uv_channel;
			if      ( m_uv_channel == 0 ) uv_index = 1;
			else if ( m_uv_channel == 1 ) uv_index = 0;
			shader_pblock->SetValue( paramID, 0, uv_index );
		}

		valid &= localValid;
	}
	else 
	{
		// This shouldn't happen
		DbgAssert(false);
	}
}

void TexturePtex::GetAdditionalReferenceDependencies( AdditionalDependencyTable& refTargets )
{
	// None
}

BaseInterface* TexturePtex::GetInterface(Interface_ID id)
{
	if ( id == IMRSHADERTRANSLATION_INTERFACE_ID)
	{
		return static_cast<imrShaderTranslation*>(this);
	}
	else 
	{
		return Texmap::GetInterface(id);
	}
}