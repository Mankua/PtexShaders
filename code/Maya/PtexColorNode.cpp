// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================

#include <math.h>

#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFloatVector.h>

#include "PtexColorNode.h"

#include "Ptexture.h"

// static data
MTypeId PtexColorNode::id( 0x8100d );

// Attributes
MObject PtexColorNode::aPtexFilename;
MObject PtexColorNode::aUVCoord;
MObject PtexColorNode::aFilterSize;

MObject PtexColorNode::aOutColor;

#define MAKE_INPUT(attr)	\
    CHECK_MSTATUS( attr.setKeyable(true) );		\
    CHECK_MSTATUS( attr.setStorable(true) );	\
    CHECK_MSTATUS( attr.setReadable(true) );	\
    CHECK_MSTATUS( attr.setWritable(true) );

#define MAKE_OUTPUT(attr)			\
    CHECK_MSTATUS( attr.setKeyable(false) );	\
    CHECK_MSTATUS( attr.setStorable(false) );	\
    CHECK_MSTATUS( attr.setReadable(true) );	\
    CHECK_MSTATUS( attr.setWritable(false) );

// DESCRIPTION:
//
PtexColorNode::PtexColorNode()
{
	InitializeCriticalSection( &m_critical_section );
}

// DESCRIPTION:
//
PtexColorNode::~PtexColorNode()
{
	DeleteCriticalSection(&m_critical_section);
	MRenderCallback::removeCallback( this );
}

void PtexColorNode::postConstructor( )
{
	m_ptex_cache = 0;
	m_ptex_texture = 0;
	m_ptex_filter = 0;

	MRenderCallback::addCallback( this, 255 );

	setMPSafe(true);
}

// DESCRIPTION:
// creates an instance of the node
void * PtexColorNode::creator()
{
    return new PtexColorNode();
}

// DESCRIPTION:
//
MStatus PtexColorNode::initialize()
{
    MFnNumericAttribute nAttr; 

    // Input attributes

	MFnTypedAttribute tAttr;
	aPtexFilename = tAttr.create( "ptexFilename", "ptex", MFnData::kString );
	MAKE_INPUT(tAttr);

	// Implicit shading network attributes

    MObject child1 = nAttr.create( "uCoord", "u", MFnNumericData::kFloat);
    MObject child2 = nAttr.create( "vCoord", "v", MFnNumericData::kFloat);
    aUVCoord = nAttr.create( "uvCoord", "uv", child1, child2);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setHidden(true) );

    child1 = nAttr.create( "uvFilterSizeX", "fsx", MFnNumericData::kFloat);
    child2 = nAttr.create( "uvFilterSizeY", "fsy", MFnNumericData::kFloat);
    aFilterSize = nAttr.create("uvFilterSize", "fs", child1, child2);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setHidden(true) );

	// Output attributes
    aOutColor = nAttr.createColor("outColor", "oc");
	MAKE_OUTPUT(nAttr);

	// Add attributes to the node database.
    CHECK_MSTATUS( addAttribute(aPtexFilename) );
    CHECK_MSTATUS( addAttribute(aFilterSize) );
    CHECK_MSTATUS( addAttribute(aUVCoord) );

    CHECK_MSTATUS( addAttribute(aOutColor) );

	// All input affect the output color
    CHECK_MSTATUS( attributeAffects(aPtexFilename,  aOutColor) );
    CHECK_MSTATUS( attributeAffects(aFilterSize,	aOutColor) );
    CHECK_MSTATUS( attributeAffects(aUVCoord,		aOutColor) );

    return MS::kSuccess;
}

MStatus PtexColorNode::compute(const MPlug& plug, MDataBlock& block) 
{
    if( ( plug != aOutColor ) && ( plug.parent() != aOutColor ) )
	{
		return MS::kUnknownParameter;
	}

	if ( m_ptex_texture == 0 )
	{
		MDataHandle ptexFilenameHnd = block.inputValue( aPtexFilename );
		MString ptexFilenameStr = ptexFilenameHnd.asString();

		int len;
		const char * ptexFilename = ptexFilenameStr.asChar( len );

		Ptex::String error;

		const char * ptex_filename = ptexFilenameStr.asChar();

		m_ptex_cache = PtexCache::create( 0, 1024 * 1024 );

		m_ptex_texture = m_ptex_cache->get( ptexFilename, error );

		if ( m_ptex_texture == 0 )
		{
			MFloatVector resultColor( 1.0f, 0.0f, 1.0f );

			MDataHandle outColorHandle = block.outputValue( aOutColor );
			MFloatVector& outColor = outColorHandle.asFloatVector();
			outColor = resultColor;
			return MS::kSuccess;
		}

		m_ptex_num_channels = m_ptex_texture->numChannels();

		PtexFilter::Options opts( PtexFilter::f_point, 0, 0.0f );
		m_ptex_filter = PtexFilter::getFilter( m_ptex_texture, opts );

		bool stop_here;
		stop_here = true;
	}

    float2 & uv = block.inputValue( aUVCoord ).asFloat2();

	unsigned int thread_id = (unsigned int)GetCurrentThreadId();

	EnterCriticalSection( &m_critical_section );

	int f = (int)uv[ 0 ];

	float u = uv[ 0 ] - (float)f;
	float v = uv[ 1 ];

	float result[4];
	m_ptex_filter->eval( result, 0, m_ptex_num_channels, f, u, v, 0, 0, 0, 0 );

	LeaveCriticalSection( &m_critical_section );

	// set ouput color attribute
	MFloatVector resultColor( result[ 0 ], result[ 1 ], result[ 2 ] );
	MDataHandle outColorHandle = block.outputValue( aOutColor );
    MFloatVector& outColor = outColorHandle.asFloatVector();
    outColor = resultColor;
    outColorHandle.setClean();

    return MS::kSuccess;
}

bool PtexColorNode::renderCallback( const MRenderData &data )
{
	if ( m_ptex_filter )
	{
		m_ptex_filter->release();
		m_ptex_filter = 0;
	}

	if ( m_ptex_texture )
	{
		m_ptex_texture->release();
		m_ptex_texture = 0;
	}

	if ( m_ptex_cache )
	{
		m_ptex_cache->release();
		m_ptex_cache = 0;
	}

	return 0;
}

bool PtexColorNode::shadowCastCallback( const MRenderShadowData &data ) 
{
	return 0;
}

bool PtexColorNode::postProcessCallback( const MRenderData &data ) 
{
	return 0;
}