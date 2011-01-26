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

#include "Modifier.h"
#include "About.h"

IObjParam* ModifierPtex::ip = NULL;

/*===========================================================================*\
 |	Class Descriptor OSM
\*===========================================================================*/

class ModifierPtexClassDesc:public ClassDesc2
{
	public:
	int 			IsPublic()					{ return TRUE; }
	void *			Create( BOOL loading )		{ return new ModifierPtex; }
	const TCHAR *	ClassName()					{ return GetString(IDS_MODIFIER_NAME); }
	SClass_ID		SuperClassID()				{ return OSM_CLASS_ID; }
	Class_ID 		ClassID()					{ return MODIFIER_PTEX_CLASS_ID; }
	const TCHAR* 	Category()					{ return GetString(IDS_CATEGORY);  }

	// Hardwired name, used by MAX Script as unique identifier
	const TCHAR*	InternalName()				{ return _T("ModifierPtex"); }
	HINSTANCE		HInstance()					{ return hInstance; }
};

static ModifierPtexClassDesc modifier_ptex_class_desc;
ClassDesc2 * GetModifierPtexClassDesc() {return &modifier_ptex_class_desc;}

/*===========================================================================*\
 |	Basic implimentation of a dialog handler
\*===========================================================================*/

// BOOL
INT_PTR ModifierPtexDlgProc::DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int id = LOWORD(wParam);
	switch (msg) 
	{
		case WM_INITDIALOG:
			break;
		case WM_DESTROY:
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) 
			{
				case IDC_MOD_ABOUT:
				{
					DialogBoxParam(	hInstance, MAKEINTRESOURCE(IDD_ABOUT), GetCOREInterface()->GetMAXHWnd(), AboutDlgProc, 0);
				}
				break;

				case IDC_MOD_HELP:
				{
					ShellExecute( NULL, "open", "http://www.mankua.com/Stripes/Stripes.php", NULL, NULL, SW_SHOWNORMAL );
				}
				break;
			}
			break;
	}
	return FALSE;
}

/*===========================================================================*\
 |	Paramblock2 Descriptor
\*===========================================================================*/

class ModifierPtexPBAccessor : public PBAccessor
{
public:
	void Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t) {
	}
};

static ModifierPtexPBAccessor   ModifierPtexPBAccessor;

static ParamBlockDesc2 skpurem_param_blk ( ModifierPtex::PARAMS, _T("ModifierPtexParams"),  0, &modifier_ptex_class_desc, P_AUTO_CONSTRUCT + P_AUTO_UI, 0, 
	//rollout
	IDD_MODIFIER, IDS_PARAMS, 0, 0, NULL, 

	// params
	ModifierPtex::UVW_TYPE,		_T("channel"),		TYPE_INT,	0,	IDS_SPIN,
		p_default,		0,
		p_range,		0,	1,
		p_ui,			TYPE_RADIO, 2, IDC_MOD_UVW_TYPE_UVW, IDC_MOD_UVW_TYPE_VCOL,
		end,

	ModifierPtex::UVW_CHANNEL,		_T("mapChanel"),	TYPE_INT,	0,	IDS_SPIN,
		p_default,		1,
		p_range, 		1, 99, 
		p_ui,			TYPE_SPINNER, EDITTYPE_INT, IDC_MOD_UVW_CHANNEL, IDC_MOD_UVW_CHANNEL_SPIN, 1.0,
		end,

	end
 );

/*===========================================================================*\
 |	Constructor
 |  Ask the ClassDesc2 to make the AUTO_CONSTRUCT paramblocks and wire them in
\*===========================================================================*/

ModifierPtex::ModifierPtex()
{
	m_pblock = 0;

	modifier_ptex_class_desc.MakeAutoParamBlocks(this);
	assert(m_pblock);
}

ModifierPtex::~ModifierPtex()
{
}

/*===========================================================================*\
 |	Invalidate our UI (or the recently changed parameter)
\*===========================================================================*/

void ModifierPtex::InvalidateUI()
{
	skpurem_param_blk.InvalidateUI(m_pblock->LastNotifyParamID());
}

/*===========================================================================*\
 |	Open and Close dialog UIs
 |	We ask the ClassDesc2 to handle Beginning and Ending EditParams for us
\*===========================================================================*/

void ModifierPtex::BeginEditParams( IObjParam *ip, ULONG flags,Animatable *prev )
{
	this->ip = ip;

	modifier_ptex_class_desc.BeginEditParams(ip, this, flags, prev);

	skpurem_param_blk.SetUserDlgProc(new ModifierPtexDlgProc(this));
}

void ModifierPtex::EndEditParams( IObjParam *ip, ULONG flags,Animatable *next )
{	
	modifier_ptex_class_desc.EndEditParams(ip, this, flags, next);

	this->ip = NULL;
}

RefTargetHandle ModifierPtex::Clone(RemapDir& remap) 
{	
	ModifierPtex* newmod = new ModifierPtex();	
	newmod->ReplaceReference(0,m_pblock->Clone(remap));
	return(newmod);
}

Animatable* ModifierPtex::SubAnim(int i)
{
	switch (i)
	{
		case 0: return m_pblock;
		default: return NULL;
	}
}

TSTR ModifierPtex::SubAnimName(int i) 
{
	switch (i)
	{
		case 0: return GetString(IDS_PARAMS);
		default: return _T("");
	}
}

RefTargetHandle ModifierPtex::GetReference(int i)
{
	switch (i) 
	{
		case 0: return m_pblock;
		default: return NULL;
	}
}

void ModifierPtex::SetReference(int i, RefTargetHandle rtarg) 
{
	switch (i) 
	{
		case 0: m_pblock = (IParamBlock2*)rtarg; break;
	}
}

RefResult ModifierPtex::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID,  RefMessage message) 
{
	switch (message)
	{
		case REFMSG_CHANGE:
			skpurem_param_blk.InvalidateUI();
		break;
	}
	return REF_SUCCEED;
}

Interval ModifierPtex::GetValidity(TimeValue t) 
{
	Interval valid = FOREVER;
	return valid;
}

Interval ModifierPtex::LocalValidity(TimeValue t) 
{
	return GetValidity(t);
}

void ModifierPtex::LocalDataChanged() 
{
	NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
}

void ModifierPtex::ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node) 
{
	Interval valid = FOREVER;

	int uvw_channel = 0;

	if ( m_pblock->GetInt( UVW_TYPE ) == 0 ) 
	{
		uvw_channel = m_pblock->GetInt( UVW_CHANNEL );
	}

	if ( os->obj->IsSubClassOf( polyObjectClassID ) )
	{ 
		PolyObject *polyObj = (PolyObject*)os->obj;
		MNMesh &mnMesh = polyObj->GetMesh();

		bool has_map;
		if ( uvw_channel >= mnMesh.MNum() )
		{
			mnMesh.SetMapNum( uvw_channel + 1 );
			has_map = false;
		} 
		else 
		{
			has_map = mnMesh.M( uvw_channel )->GetFlag( MN_DEAD ) == false;
		}

		MNMap *mc = mnMesh.M( uvw_channel );

		if ( has_map == false )
		{
			mc->setNumFaces( mnMesh.numf );

			for ( int i_f = 0; i_f < mnMesh.numf; i_f++ )
			{
				mc->f[ i_f ].SetSize( mnMesh.f[ i_f ].deg );
			}
		}

		if ( mc->GetFlag( MN_DEAD ) ) mc->ClearFlag( MN_DEAD );

		unsigned int num_tverts = 0;

		for ( int i_f = 0; i_f < mnMesh.numf; i_f++ )
		{
			num_tverts += mc->f[ i_f ].deg;
		}

		mc->setNumVerts( num_tverts );

		unsigned int tvert_id = 0;

		for ( int i_f = 0; i_f < mnMesh.numf; i_f++ )
		{
			unsigned int deg = mc->f[ i_f ].deg;

			for ( unsigned int i_v = 0; i_v < deg; i_v++ )
			{
				mc->f[ i_f ].tv[ i_v ] = tvert_id;

				Point3 tv;

				if      ( i_v == 0 ) tv = Point3( 0.0f, 0.0f, 0.0f );
				else if ( i_v == 1 ) tv = Point3( 1.0f, 0.0f, 0.0f );
				else if ( i_v == 2 ) tv = Point3( 1.0f, 1.0f, 0.0f );
				else                 tv = Point3( 0.0f, 1.0f, 0.0f );

				tv.z = (float)i_f;

				mc->v[ tvert_id ] = tv;
			
				tvert_id++;
			}
		}
	}

	// Update all the caches etc
	Interval iv = LocalValidity(t);
	iv = iv & os->obj->ChannelValidity( t, GEOM_CHAN_NUM );
	iv = iv & os->obj->ChannelValidity( t, TOPO_CHAN_NUM );
	os->obj->UpdateValidity( TEXMAP_CHAN_NUM, iv );
}

/*===========================================================================*\
 |	NotifyInputChanged is called each time the input object is changed in some way
 |	We can find out how it was changed by checking partID and message
\*===========================================================================*/

void ModifierPtex::NotifyInputChanged(Interval changeInt, PartID partID, RefMessage message, ModContext *mc)
{

}

