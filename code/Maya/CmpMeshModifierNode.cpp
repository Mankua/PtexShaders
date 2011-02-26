//
//  This file accompanies the book "Complete Maya Programming (Volume 2)"
//  For further information visit http://www.davidgould.com
// 
//  Copyright (C) 2004 David Gould 
//  All rights reserved.
//
#include "CmpMeshModifierNode.h"
#include <maya/MFnTypedAttribute.h>

MObject CmpMeshModifierNode::inMesh;
MObject CmpMeshModifierNode::outMesh;

MStatus CmpMeshModifierNode::initialize()
{	
	MFnTypedAttribute tAttr;
	inMesh = tAttr.create( "inMesh", "im", MFnData::kMesh );
	
	outMesh = tAttr.create( "outMesh", "om", MFnData::kMesh );
	tAttr.setStorable( false );
	
	addAttribute( inMesh );
	addAttribute( outMesh );
	
	attributeAffects( inMesh, outMesh );
	
	return MS::kSuccess;
}
