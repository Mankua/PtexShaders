//
//  This file accompanies the book "Complete Maya Programming (Volume 2)"
//  For further information visit http://www.davidgould.com
// 
//  Copyright (C) 2004 David Gould 
//  All rights reserved.
//
#include "CmpMeshModifierCmd.h"
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MFnNumericAttribute.h>

CmpMeshModifierCmd::CmpMeshModifierCmd()
{
	hasHistory = hasInternalTweaks = genHistory = false;
}

MStatus CmpMeshModifierCmd::doIt( const MDagPath &dagPath, const MTypeId &meshModType )
{
	MStatus stat;
	
	meshShapePath = dagPath;
	if( !meshShapePath.isValid() )
	{
		displayError( "Invalid mesh shape path: " + meshShapePath.fullPathName() );
		return MS::kFailure;
	}
	
	meshModifierNodeType = meshModType;
	
	//
	// Get the current state of the history
	//
	MFnDagNode origShapeNodeFn( meshShapePath );
	
	// Determine if the mesh has history
	MPlug inMeshOrigPlug = origShapeNodeFn.findPlug( "inMesh" );
	hasHistory = inMeshOrigPlug.isConnected();
	
	// Determine if the mesh has tweaks
	hasInternalTweaks = false;
	MPlug tweaksPlug = origShapeNodeFn.findPlug( "pnts" );
	if( !tweaksPlug.isNull() )
	{
		MObject obj;
		MPlug tweakPlug;
		MFloatVector tweak;
		unsigned int i;
		unsigned int nTweaks = tweaksPlug.numElements();
		for( i=0; i < nTweaks; i++ )
		{
			tweakPlug = tweaksPlug.elementByPhysicalIndex( i, &stat );
			if( stat && !tweakPlug.isNull() )
			{
				tweakPlug.getValue( obj );
				MFnNumericData numDataFn( obj );
				numDataFn.getData( tweak[0], tweak[1], tweak[2] );
				
				if( tweak[0] != 0.0f || tweak[1] != 0.0f || tweak[2] != 0.0f )
				{
					hasInternalTweaks = true;
					break;
				}
			}
		}
	}
	
	int res;
	MGlobal::executeCommand( "constructionHistory -query -toggle", res );
	genHistory = res != 0;
	
	//MGlobal::displayInfo( MString("resulting: ") + hasHistory + " " + hasInternalTweaks + " " + genHistory + "\n" );
	
	// When there is no existing history
	// cache the mesh data for later undoing
	//
	if( !hasHistory )
	{
		MPlug meshPlug = origShapeNodeFn.findPlug( hasInternalTweaks ? "cachedInMesh" : "outMesh" );
		meshPlug.getValue( origMeshData );
	}
	
	// Create the modifier node
	MObject modNode = dagMods[0].MDGModifier::createNode( meshModifierNodeType, &stat );
	// Create tweak node
	MObject tweakNode = dagMods[0].MDGModifier::createNode( "polyTweak", &stat );
	// Execute DAG modifier to ensure that the nodes actually exist
	dagMods[0].doIt(); 
	
	// Check that the inMesh and outMesh attributes exist in the modifier node
	MFnDependencyNode nodeFn( modNode );
	if( nodeFn.attribute( "inMesh" ).isNull() || nodeFn.attribute( "outMesh" ).isNull() )
	{
		displayError( "Invalid modifier node. It doesn't have inMesh and/or outMesh attributes" );
		return MS::kFailure;
	}

	// Let the derived command class initialize the modifier node
	initModifierNode( modNode, dagMods[1] );
	MFnDependencyNode modNodeFn( modNode );
		
	// Get plug that is the start of the new stream
	MPlug newStreamInMeshPlug = modNodeFn.findPlug( "inMesh" );
		
	// Get the plug connecting into original shape's inMesh
	MPlugArray inPlugs;
	inMeshOrigPlug.connectedTo( inPlugs, true, false );
	MPlug oldStreamOutMeshPlug;
	// N.B. For meshes without construction history
	// there won't be incoming connection
	if( inPlugs.length() )
	{
		oldStreamOutMeshPlug = inPlugs[0];		
		
		// Disconnect the connection into the mesh shape's inMesh attribute.
		// The outMesh of the modifier node will later connect into this.
		dagMods[1].disconnect( oldStreamOutMeshPlug, inMeshOrigPlug );
	}
		
	if( hasInternalTweaks )
	{
		// Transfer tweaks from the mesh shape to the tweak node
		transferTweaks( meshShapePath, tweakNode, dagMods[1] );
		
		MFnDependencyNode tweakNodeFn( tweakNode );
		newStreamInMeshPlug = tweakNodeFn.findPlug( "inputPolymesh" );
		
		// Connect output of tweak node into modifier node		
		MPlug inMeshModPlug = modNodeFn.findPlug( "inMesh" );
		MPlug outMeshTweakPlug = tweakNodeFn.findPlug( "output" );
		dagMods[1].connect( outMeshTweakPlug, inMeshModPlug );
	}

	dagMods[1].doIt();
	
	copyTransform = MObject::kNullObj;
	
	// Generate history for shape that doesn't have one
	if( !hasHistory ) //&& genHistory )
	{		
		// Duplicate the mesh shape node
		copyTransform = origShapeNodeFn.duplicate();
		MFnDagNode copyTransformFn( copyTransform );
		
		MObject copyShapeNode = copyTransformFn.child(0);
		MFnDagNode copyShapeNodeFn( copyShapeNode );
		
		//MGlobal::displayInfo( MString("copy: transform: ") + copyTransformFn.fullPathName() + " shape name: " + copyShapeNodeFn.fullPathName() + "\n" );
				
		// Set it to be an intermediate object
		dagMods[2].commandToExecute( "setAttr " + copyShapeNodeFn.fullPathName() + ".intermediateObject true" );
		
		// Set output plug of old stream to be the outMesh of the duplicated shape
		oldStreamOutMeshPlug = copyShapeNodeFn.findPlug( "outMesh" );
				
		// Rename the duplicate
		dagMods[2].renameNode( copyShapeNode, copyShapeNodeFn.name() + "Orig" );
		
		// Reparent the shape
		MObject origTransform = meshShapePath.transform();
		dagMods[2].reparentNode( copyShapeNode, origTransform );
		
		// Remove the now orphaned transform
		// N.B. calling deleteNode( transformCopy ) causes the shape node to
		// also be deleted, even though it has been reparented to the original mesh.
		// As such, the MEL command "delete" was used instead.
		//
		// The deleteNode() method does some
		// preparation work before it enqueues itself in the MDagModifier list
		// of operations, namely, it looks at it's parents and children and
		// deletes them as well if they are the only parent/child of the node
		// scheduled to be deleted.
		dagMods[2].commandToExecute( "delete " + copyTransformFn.fullPathName() );
	}	
	
	if( !oldStreamOutMeshPlug.isNull() )
		// Connect output mesh of the previous stream the input of the new stream
		dagMods[2].connect( oldStreamOutMeshPlug, newStreamInMeshPlug );
		
	// Connect output of the mesh modifier node to the input of the original mesh shape
	MPlug outMeshModPlug = modNodeFn.findPlug( "outMesh" );
	dagMods[2].connect( outMeshModPlug, inMeshOrigPlug );
	
	if( !hasHistory && !genHistory )
		// Collapse the history
		dagMods[2].commandToExecute( MString("delete -constructionHistory ") + meshShapePath.fullPathName() );
	
	dagMods[2].doIt();
	
	return MS::kSuccess;
}

MStatus	CmpMeshModifierCmd::transferTweaks( const MDagPath &shapePath,
										    MObject &tweakNode, 
											MDagModifier &dagMod )
{
	// Get the tweaks from the mesh shape and apply them to the 
	// to the tweak node
	MFnDagNode shapeNodeFn( shapePath );
	MPlug srcTweaksPlug = shapeNodeFn.findPlug( "pnts" );
	
	MFnDependencyNode tweakNodeFn( tweakNode );
	MPlug dstTweaksPlug = tweakNodeFn.findPlug( "tweak" );
		
	//MGlobal::displayInfo( MString( "storing tweaks from " ) + shapePath.fullPathName() + "\n" );
	
	MPlugArray plugs;
	MPlug srcTweakPlug;
	MPlug dstTweakPlug;
	MObject dataObj;
	MFloatVector tweak;
	unsigned int nTweaks = srcTweaksPlug.numElements();
	unsigned int i, j, ci, logicalIndex;
	for( i=0; i < nTweaks; i++ )
	{
		srcTweakPlug = srcTweaksPlug.elementByPhysicalIndex( i );
		if( !srcTweakPlug.isNull() )
		{
			logicalIndex = srcTweakPlug.logicalIndex();
			
			// Set tweak node tweak element
			srcTweakPlug.getValue( dataObj );			
			MFnNumericData numDataFn( dataObj );
			numDataFn.getData( tweak[0], tweak[1], tweak[2] );
			
			dagMod.commandToExecute( MString( "setAttr " ) + tweakNodeFn.name() + ".tweak[" + logicalIndex + "] " + 
									 tweak[0] + " " + tweak[1] + " " + tweak[2] );

			// Handle transfer of incoming and outgoing connections to "pnts" elements
			dstTweakPlug = dstTweaksPlug.elementByLogicalIndex(logicalIndex);

			if( srcTweakPlug.isConnected() )
			{
				// As source, transfer source to tweak node tweak
				srcTweakPlug.connectedTo( plugs, false, true );
				for( j=0; j < plugs.length(); j++ )
				{
					dagMod.disconnect( srcTweakPlug, plugs[j] );
					dagMod.connect( dstTweakPlug, plugs[j] );
				}
				
				// As destination, transfer destination to tweak node tweak
				srcTweakPlug.connectedTo( plugs, true, false );
				if( plugs.length() == 1 ) // There can only be one input connection
				{
					dagMod.disconnect( plugs[0], srcTweakPlug );
					dagMod.connect( plugs[0], dstTweakPlug );
				}	
			}
			else // Check children
			{
				MPlug srcTweakChildPlug;
				MPlug dstTweakChildPlug;
				
				for( ci=0; ci < srcTweakPlug.numChildren(); ci++ )
				{
					srcTweakChildPlug = srcTweakPlug.child(ci);
					dstTweakChildPlug = dstTweakPlug.child(ci);
					if( srcTweakChildPlug.isConnected() )
					{
						// As souce, transfer source to tweak node tweak
						srcTweakChildPlug.connectedTo( plugs, false, true );
						for( j=0; j < plugs.length(); j++ )
						{
							dagMod.disconnect( srcTweakChildPlug, plugs[j] );
							dagMod.connect( dstTweakChildPlug, plugs[j] );
						}
						
						// As destination, transfer destination to tweak node tweak
						srcTweakChildPlug.connectedTo( plugs, true, false );
						if( plugs.length() == 1 ) // There can only be one input connection
						{
							dagMod.disconnect( plugs[0], srcTweakChildPlug );
							dagMod.connect( plugs[0], dstTweakChildPlug );
						}
					}	
				}				
			}
									
			// With the tweak values and any connections now transferred to
			// the tweak node's tweak element, this source element can be reset
			dagMod.commandToExecute( MString( "setAttr " ) + shapePath.fullPathName() + ".pnts[" + logicalIndex + "] 0 0 0" );									
			
			//MGlobal::displayInfo( MString(" tweak: ") + tweakIndices[i] + ": " + tweaks[i].x + ", " + tweaks[i].y + ", " + tweaks[i].z + "\n" );
		}
	}

	return MS::kSuccess;
}

MStatus	CmpMeshModifierCmd::redoIt()
{
		
	dagMods[0].doIt();
	dagMods[1].doIt();
	// Manually duplicate the original mesh shape
	// Since there is no DGModifier or DAGModifier function
	// for duplicating, this must be done manually
	if( !hasHistory )
	{
		MFnDagNode origShapeNodeFn( meshShapePath );
		copyTransform = origShapeNodeFn.duplicate();		
	}
	dagMods[2].doIt();
		
	return MS::kSuccess;
}

MStatus	CmpMeshModifierCmd::undoIt()
{
	dagMods[2].undoIt();

	// Manually delete the duplicated shape
	if( !copyTransform.isNull() )
		MGlobal::deleteNode( copyTransform );

	dagMods[1].undoIt();
	dagMods[0].undoIt();

	if( !hasHistory )
	// Manually restore the original mesh data since
	// it would have been destroyed when the construction
	// history was applied
	{
		MFnDagNode origShapeNodeFn( meshShapePath );
		MPlug meshPlug = origShapeNodeFn.findPlug( hasInternalTweaks ? "cachedInMesh" : "outMesh" );
		meshPlug.setValue( origMeshData );
	}
		
	return MS::kSuccess;
}