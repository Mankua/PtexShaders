#include "PtexUVCmd.h"
#include "PtexUVNode.h"
#include "PtexColorNode.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
{
	MFnPlugin plugin( obj, "Diego A. Castano", "1.0" );

	MStatus stat;

	stat = plugin.registerCommand( "ptexUV", PtexUVCmd::creator, PtexUVCmd::newSyntax );
	if( !stat )
	{
		MGlobal::displayError( MString( "registerCommand ptexUV failed: " ) + stat.errorString() );
		return stat;
	}

	stat = plugin.registerNode( "ptexUV", PtexUVNode::id, PtexUVNode::creator, PtexUVNode::initialize );
	if( !stat )
	{
		MGlobal::displayError( MString( "registerNode ptexUV failed: " ) + stat.errorString() );
		return stat;
	}

	const MString UserClassify( "texture/2d" );
	stat = plugin.registerNode( "ptex", PtexColorNode::id, &PtexColorNode::creator, &PtexColorNode::initialize, MPxNode::kDependNode, &UserClassify );
	if( !stat )
	{
		MGlobal::displayError( MString( "registerNode ptex failed: " ) + stat.errorString() );
		return stat;
	}

	return stat;
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	MStatus	stat;

	stat = plugin.deregisterCommand( "ptexUV" );
	if ( !stat )
	{
		MGlobal::displayError( MString( "deregisterCommand failed: " ) + stat.errorString() );
		return stat;
	}

	stat = plugin.deregisterNode( PtexUVNode::id );
	if ( !stat )
	{
		MGlobal::displayError( MString( "deregisterNode failed: " ) + stat.errorString() );
		return stat;
	}

	stat = plugin.deregisterNode( PtexColorNode::id );
	if ( !stat )
	{
		MGlobal::displayError( MString( "deregisterNode failed: " ) + stat.errorString() );
		return stat;
	}

	return stat;
}