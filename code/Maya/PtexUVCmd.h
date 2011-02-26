//
//  This file accompanies the book "Complete Maya Programming (Volume 2)"
//  For further information visit http://www.davidgould.com
// 
//  Copyright (C) 2004 David Gould 
//  All rights reserved.
//
#ifndef _PTEX_UV_CMD_H_
#define _PTEX_UV_CMD_H_

#include "CmpMeshModifierCmd.h"

class PtexUVCmd : public CmpMeshModifierCmd
{
public:
	virtual MStatus doIt( const MArgList &args );
	
	virtual MStatus initModifierNode( MObject &node, MDagModifier &dagMod );

	static void *creator();
	static MSyntax newSyntax();

private:
	double strength;
	MString imageFilename; 
};

#endif