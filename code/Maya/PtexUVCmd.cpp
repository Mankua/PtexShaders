//
//  This file accompanies the book "Complete Maya Programming (Volume 2)"
//  For further information visit http://www.davidgould.com
// 
//  Copyright (C) 2004 David Gould 
//  All rights reserved.
//
#include "PtexUVCmd.h"
#include "PtexUVNode.h"
#include <maya/MSelectionList.h>

MSyntax PtexUVCmd::newSyntax()
{
    MSyntax syntax;
	
	syntax.enableQuery( false );
	syntax.enableEdit( false );
		
	return syntax;
}

MStatus PtexUVCmd::doIt( const MArgList &args )
{
	// Get the options from the command line
	MStatus stat;
	MArgDatabase argData( syntax(), args, &stat );

	if( !stat )	return stat;

	// Iterate over the meshes
	// Get a list of currently selected objects
	MSelectionList selection;
	MGlobal::getActiveSelectionList( selection );

	int nSelMeshes = 0;
	MDagPath dagPath;
	MItSelectionList iter( selection, MFn::kMesh );
	for ( ; !iter.isDone(); iter.next() )
	{
		nSelMeshes++;								

		// Get DAG path to mesh shape node
		iter.getDagPath( dagPath );		
		dagPath.extendToShape();
		
		break;
	}
	
	if( nSelMeshes == 0 )
	{
		MGlobal::displayWarning( "Select one or more meshes" );
		return MS::kFailure;
	}	
	
	CmpMeshModifierCmd::doIt( dagPath, PtexUVNode::id );
	
	return MS::kSuccess;
}

void *PtexUVCmd::creator()
{ 
	return new PtexUVCmd; 
}

// This function takes the newly created mesh modifier node and intializes it. 
// It should record all the changes it makes to the node in the supplied dagMod object.
// This ensures that the changes can be undone.
//
MStatus PtexUVCmd::initModifierNode( MObject &node, MDagModifier &dagMod )
{
	MFnDependencyNode depFn( node );
	MString name = depFn.name();
		
	return MS::kSuccess;
}

