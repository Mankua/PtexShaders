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

#ifndef _TEXTURE_PTEX_H_
#define _TEXTURE_PTEX_H_

#include <map>

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

#include "stdmat.h"
#include "imtl.h"
#include "macrorec.h"

#include <IMtlRender_Compatibility.h>
#include <mentalray\imrShader.h>
#include <mentalray\imrShaderTranslation.h>
#include <mentalray\imrAdvancedTranslation.h>
#include <mentalray\mentalrayInMax.h>

class PtexCache;
class PtexTexture;
class PtexFilter;

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define TEXTURE_PTEX_CLASS_ID Class_ID(0x37916a9b, 0x297c164e)

#define PBLOCK_REF	0

class TexturePtex : public Texmap, private imrShaderTranslation
{
public:

	enum 
	{
		PBLOCK,
		TEXTURE_OUTPUT,
		NUM_REFS,
	};

	enum 
	{ 
		PTEX_FILE,
		UVW_TYPE,
		UVW_CHANNEL,
		FILTER_SIZE,
		FILTER_TYPE,
		BUMP_AMOUNT,
		OUTPUT_REF,
		PREMULTIPLIED_ALPHA,
	};

	// Parameter block
	IParamBlock2 * m_pblock;		// ref 0 : PARAMS
	TextureOutput * m_texture_output;		// ref 1 : OUTPUT

	static ParamDlg * m_texture_output_dlg;

	Interval		m_interval;

	PtexCache * m_ptex_cache;
	PtexTexture * m_ptex_texture;
	int m_ptex_num_channels;
	bool m_premultiplied_alpha;

	std::map< unsigned int, PtexFilter * > m_ptex_filters;

	// This plugin needs to be Thread Safe, so...
	CRITICAL_SECTION m_critical_section;
	
	// Temp

	MSTR m_ptex_file_path;
	int m_uv_channel;
	int m_filter_id;
	float m_filter_size;

	//Constructor/Destructor
	TexturePtex();
	~TexturePtex();

	//From MtlBase
	ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);
	BOOL SetDlgThing(ParamDlg* dlg);
	void Update(TimeValue t, Interval& valid);
	void Reset();
	Interval Validity(TimeValue t);
	ULONG LocalRequirements( int subMtlNum);
	void LocalMappingsRequired( int  subMtlNum, BitArray &mapreq, BitArray &bumpreq );

	int NumSubTexmaps() { return 0; }
	Texmap* GetSubTexmap(int i) { return 0; }
	void SetSubTexmap(int i, Texmap *m) {}
	TSTR GetSubTexmapSlotName(int i) { return TSTR(_T("WhazzThis")); }

	//From Texmap
	RGBA EvalColor(ShadeContext& sc);
	float EvalMono(ShadeContext& sc);
	Point3 EvalNormalPerturb(ShadeContext& sc);

	int SubNumToRefNum( int subNum ) { return subNum; }

	//TODO: If your class is derived from Tex3D then you should also 
	//implement ReadSXPData for 3D Studio/DOS SXP texture compatibility
	void ReadSXPData(TCHAR *name, void *sxpdata) { }

	// Loading/Saving
	IOResult Load(ILoad *iload);
	IOResult Save(ISave *isave);

	//From Animatable
	Class_ID ClassID() {return TEXTURE_PTEX_CLASS_ID;}		
	SClass_ID SuperClassID() { return TEXMAP_CLASS_ID; }
	void GetClassName( TSTR& s ) {s = GetString(IDS_TEXTURE_NAME);}
	//MSTR GetFullName( TSTR& s );

	RefTargetHandle Clone( RemapDir &remap );
	RefResult NotifyRefChanged( Interval changeInt, RefTargetHandle hTarget, PartID& partID,  RefMessage message );

	int NumSubs() { return NUM_REFS; }
	Animatable* SubAnim( int i ); 
	TSTR SubAnimName( int i );

	// TODO: Maintain the number or references here 
	int NumRefs() { return NUM_REFS; }
	RefTargetHandle GetReference( int i );
	void SetReference( int i, RefTargetHandle rtarg );

	MaxSDK::AssetManagement::AssetUser GetPtexFile() const;
	void SetPtexFile( const MaxSDK::AssetManagement::AssetUser& file );

	int	NumParamBlocks() { return 1; }
	IParamBlock2* GetParamBlock( int i ) { return m_pblock; } 
	IParamBlock2* GetParamBlockByID( BlockID id ) { return ( m_pblock->ID() == id ) ? m_pblock : NULL; }

	void DeleteThis() { delete this; }		

	void EnumAuxFiles(AssetEnumCallback& nameEnum, DWORD flags);

	int RenderBegin( TimeValue t, ULONG flags=0 );
	int RenderEnd( TimeValue t );

	// -- from InterfaceServer
	virtual BaseInterface* GetInterface(Interface_ID id);

private:

	// -- from imrShaderTranslation
	virtual imrShader* GetMRShader( imrShaderCreation& shaderCreation );
	virtual void ReleaseMRShader();
	virtual bool NeedsAutomaticParamBlock2Translation();
	virtual void TranslateParameters(imrTranslation& translationInterface, imrShader* shader, TimeValue t, Interval& valid);
	virtual void GetAdditionalReferenceDependencies(AdditionalDependencyTable& refTargets);
};


#endif
