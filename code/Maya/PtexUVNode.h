//
//  This file accompanies the book "Complete Maya Programming (Volume 2)"
//  For further information visit http://www.davidgould.com
// 
//  Copyright (C) 2004 David Gould 
//  All rights reserved.
//
#ifndef _PTEX_UV_NODE_H_
#define _PTEX_UV_NODE_H_

#include "CmpMeshModifierNode.h"

class PtexUVNode : public CmpMeshModifierNode
{
public:	
	virtual MStatus compute( const MPlug &plug, MDataBlock &data );
	static  void *creator();
	static  MStatus initialize();
	
	static MTypeId id;
};

#endif
