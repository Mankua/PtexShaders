//
//  This file accompanies the book "Complete Maya Programming (Volume 2)"
//  For further information visit http://www.davidgould.com
// 
//  Copyright (C) 2004 David Gould 
//  All rights reserved.
//
#ifndef CMPMESHMODIFIERCMD_H
#define CMPMESHMODIFIERCMD_H

#include <maya/MDagModifier.h>
#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MDagPath.h>
#include <maya/MPoint.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshEdge.h>
#include <maya/MDagModifier.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MDistance.h>
#include <maya/MDagPathArray.h>

// N.B. This an abstract class since the doIt(MArgList &) function hasn't
// been defined. Derived classes must define this function.
//
class CmpMeshModifierCmd : public MPxCommand
{
public:
	CmpMeshModifierCmd();
	
	MStatus doIt( const MDagPath &dagPath,
				  const MTypeId &meshModType );
	virtual MStatus	redoIt();
	virtual MStatus	undoIt();
	virtual bool isUndoable() const { return true; }
	
	virtual MStatus initModifierNode( MObject &node, MDagModifier &dagMod ) = 0;
	
private:
	MDagPath meshShapePath;
	MTypeId meshModifierNodeType;
	
	bool hasHistory;
	bool hasInternalTweaks;
	bool genHistory;

	enum { N_DAGMODIFIERS=3 };
	MDagModifier dagMods[N_DAGMODIFIERS];
	
	MStatus	transferTweaks( const MDagPath &shapePath,
							MObject &tweakNode, 
							MDagModifier &dagMod );


	MObject copyTransform; // Parent transform of duplicated shape
	MObject origMeshData; // Copy of outMesh data when shape has no history, so that it can be undone
};

#endif