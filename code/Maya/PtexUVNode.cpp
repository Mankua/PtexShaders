
#include "PtexUVNode.h"
#include <maya/MFnMeshData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MItMeshEdge.h>
#include <maya/MMatrix.h>
#include <maya/MDistance.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MItMeshFaceVertex.h>
#include <maya/MFnStringData.h>
#include <maya/MImage.h>

MTypeId PtexUVNode::id( 0x00116A41 );

MStatus PtexUVNode::compute( const MPlug &plug, MDataBlock &data )
{
	MStatus stat;
	bool hasNoEffect = false;
	
	MDataHandle inMeshHnd = data.inputValue( inMesh );
	MDataHandle outMeshHnd = data.outputValue( outMesh );
	 
	MDataHandle stateHnd = data.inputValue( state );
	int state = stateHnd.asInt();

	if( state == 1 ) // No Effect/Pass through
		hasNoEffect = true;
		
	if( !hasNoEffect && plug == outMesh )
	{
	    MObject inMeshData = inMeshHnd.asMesh();
				
		if( !hasNoEffect )
		{
			MFnMeshData meshDataFn;
			MObject newMeshData = meshDataFn.create();
			MFnMesh inMeshFn( inMeshData );
			inMeshFn.copy( inMeshData, newMeshData );
			
			MFnMesh meshFn( newMeshData );
			MPointArray pts;
			meshFn.getPoints( pts );

			MStringArray uvSetNames;
			meshFn.getUVSetNames( uvSetNames );
			unsigned int defaultUvSetCount = (unsigned int)uvSetNames.length();

			int num_faces = meshFn.numPolygons();

			MIntArray uvCounts;
			uvCounts.setLength( num_faces );

			for ( int i_f = 0; i_f < num_faces; i_f++ )
			{
				int deg = meshFn.polygonVertexCount( i_f );
				uvCounts[ i_f ] = deg;

				if ( deg != 4 )
				{
					return MS::kFailure;
				}
			}

			MIntArray uvIds;
			uvIds.setLength( 4 * num_faces );

			if ( defaultUvSetCount == 1 )
			{
				int currentUVCount = meshFn.numUVs( uvSetNames[0] );

				MFloatArray us, vs; 
				us.setLength( 4 * num_faces ); 
				vs.setLength( 4 * num_faces );

				for ( int i_f = 0; i_f < num_faces; i_f++ )
				{
					float f = (float)i_f;

					uvIds[ 4 * i_f + 0 ] = 4 * i_f + 0;
					uvIds[ 4 * i_f + 1 ] = 4 * i_f + 1;
					uvIds[ 4 * i_f + 2 ] = 4 * i_f + 2;
					uvIds[ 4 * i_f + 3 ] = 4 * i_f + 3;

					us[ 4 * i_f + 0 ] = (float)i_f;         vs[ 4 * i_f + 0 ] = 0.0f;
					us[ 4 * i_f + 1 ] = (float)i_f + 1.0f;  vs[ 4 * i_f + 1 ] = 0.0f;
					us[ 4 * i_f + 2 ] = (float)i_f + 1.0f;  vs[ 4 * i_f + 2 ] = 1.0f;
					us[ 4 * i_f + 3 ] = (float)i_f;         vs[ 4 * i_f + 3 ] = 1.0f;
				}

				stat = meshFn.setUVs( us, vs, &uvSetNames[0] );
				stat = meshFn.assignUVs( uvCounts, uvIds, &uvSetNames[0] );
			}

			meshFn.updateSurface();
			meshFn.syncObject();

			outMeshHnd.set( newMeshData );
		}	
	}
	else 
		return MS::kUnknownParameter;

	if( hasNoEffect )
		outMeshHnd.set( inMeshHnd.asMesh() );
	
	data.setClean( plug );

	return stat;
}

void *PtexUVNode::creator()
{
	return new PtexUVNode();
}

MStatus PtexUVNode::initialize()
{
	// Call super class's initialize function to setup
	// inMesh and outMesh attributes
	//
	CmpMeshModifierNode::initialize();
	
	return MS::kSuccess;
}



