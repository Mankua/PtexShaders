//
//  This file accompanies the book "Complete Maya Programming (Volume 2)"
//  For further information visit http://www.davidgould.com
// 
//  Copyright (C) 2004 David Gould 
//  All rights reserved.
//
#ifndef CMPMESHMODIFIERNODE_H
#define CMPMESHMODIFIERNODE_H

#include <maya/MPxNode.h>

class CmpMeshModifierNode : public MPxNode
{
public:		
	static  MStatus initialize();
	
protected:
	static MObject inMesh;
	static MObject outMesh;
};

#endif