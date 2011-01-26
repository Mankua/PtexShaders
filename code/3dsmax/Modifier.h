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

#ifndef _MODIFIER_PTEX_H
#define _MODIFIER_PTEX_H

#include "max.h"
#include "iparamm2.h"
#include "istdplug.h"
#include "meshadj.h"
#include "modstack.h"
#include "macrorec.h"
#include "resource.h"

#define	MODIFIER_PTEX_CLASS_ID Class_ID(0x71c55045, 0x730a23df)

TCHAR *GetString(int id);

/*===========================================================================*\
 |	ModifierPtex class defn
\*===========================================================================*/

class ModifierPtex : public Modifier
{
public:

	enum 
	{ 
		PARAMS, 
	}; 

	enum 
	{	
		UVW_TYPE,
		UVW_CHANNEL,
	};

	// Access to the interface
	static IObjParam *ip;

	// Global parameter block
	IParamBlock2	*m_pblock;

	//Constructor/Destructor
	ModifierPtex();
	~ModifierPtex();
	void DeleteThis() { delete this; }

	// Plugin identification
	void GetClassName(TSTR& s) { s= TSTR(GetString(IDS_MODIFIER_NAME)); }  
	virtual Class_ID ClassID() { return MODIFIER_PTEX_CLASS_ID;}		
	TCHAR *GetObjectName() { return GetString(IDS_MODIFIER_NAME); }

	// Defines the behavior for this modifier
	// This is currently setup to be basic geometry 
	// modification of deformable objects
	ChannelMask ChannelsUsed()  { return PART_GEOM|PART_TOPO|PART_TEXMAP|PART_VERTCOLOR; }
	ChannelMask ChannelsChanged() { return PART_GEOM|PART_TOPO|PART_TEXMAP|PART_VERTCOLOR; }
	Class_ID InputType() { return defObjectClassID; }
	BOOL ChangeTopology() {return FALSE;}

	// Calculate the local validity from the parameters
	Interval LocalValidity(TimeValue t);
	Interval GetValidity(TimeValue t);

	// Object modification and notification of change
	void ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node);
	void NotifyInputChanged(Interval changeInt, PartID partID, RefMessage message, ModContext *mc);

	// Reference support
	int NumRefs() { return 1; }
	RefTargetHandle GetReference(int i);
	void SetReference(int i, RefTargetHandle rtarg);
	RefTargetHandle Clone(RemapDir& remap = DefaultRemapDir());
	RefResult NotifyRefChanged( Interval changeInt,RefTargetHandle hTarget, 
	   PartID& partID, RefMessage message);
	void LocalDataChanged();

	
	// SubAnim support
	int NumSubs() { return 1; }
	Animatable* SubAnim(int i);
	TSTR SubAnimName(int i);

	// Direct paramblock access
	int	NumParamBlocks() { return 1; }	
	IParamBlock2* GetParamBlock(int i) { return m_pblock; }
	IParamBlock2* GetParamBlockByID(BlockID id) { return (m_pblock->ID() == id) ? m_pblock : NULL; }
	int GetParamBlockIndex(int id) {return id;}

	// Does not use createmouse callbacks
	CreateMouseCallBack* GetCreateMouseCallBack() {return NULL;}

	// Load and unload our UI
	void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
	void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);
	void InvalidateUI();
};


/*===========================================================================*\
 |	Dialog Processor
\*===========================================================================*/

class ModifierPtexDlgProc : public ParamMap2UserDlgProc 
{
	public:
		ModifierPtex *spm;

		ModifierPtexDlgProc() {}
		ModifierPtexDlgProc(ModifierPtex *spm_in) { spm = spm_in; }

		// BOOL
		INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void DeleteThis() { }

		void SetThing(ReferenceTarget *m) {
			spm = (ModifierPtex*)m;
			}

};


#endif
