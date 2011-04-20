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
#include <maya/MFnEnumAttribute.h>
#include <maya/MFloatVector.h>

#include "PtexColorNode.h"

#include "Ptexture.h"

/////////////////////////////////////////////////////////////////////

#define MCHECKERROR(STAT,MSG)       \
	if ( MS::kSuccess != STAT ) {   \
	cerr << MSG << endl;			\
	return MS::kFailure;			\
	}

#define MAKE_INPUT(attr)	\
	CHECK_MSTATUS(attr.setKeyable(true) );		\
	CHECK_MSTATUS( attr.setStorable(true) );	\
	CHECK_MSTATUS( attr.setReadable(true) );	\
	CHECK_MSTATUS( attr.setWritable(true) );

#define MAKE_OUTPUT(attr)			\
	CHECK_MSTATUS( attr.setKeyable(false) );	\
	CHECK_MSTATUS( attr.setStorable(false) );	\
	CHECK_MSTATUS( attr.setReadable(true) );	\
	CHECK_MSTATUS( attr.setWritable(false) );

// static data
MTypeId PtexColorNode::id( 0x00116A40 );

// Attributes
MObject PtexColorNode::aPtexFileName;
MObject PtexColorNode::aPtexFilterType;
MObject PtexColorNode::aPtexFilterSize;

MObject PtexColorNode::aUVPos;
MObject PtexColorNode::aUVSize;

MObject PtexColorNode::aOutColor;


// DESCRIPTION:
//
PtexColorNode::PtexColorNode()
{
}

// DESCRIPTION:
//
PtexColorNode::~PtexColorNode()
{
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
	MStatus status;

	MFnNumericAttribute numericAttribute;

	// Input attributes

	MFnTypedAttribute fileNameAttribute;
	aPtexFileName = fileNameAttribute.create( "ptexFileName", "f", MFnData::kString );
	MAKE_INPUT( fileNameAttribute );
	fileNameAttribute.setConnectable(false);

	MFnEnumAttribute enumAttribute;
	aPtexFilterType = enumAttribute.create( "ptexFilterType", "t", 0, &status );
	MCHECKERROR( status, "create filterType attribute" );
	enumAttribute.addField( "Point",      0 );
	enumAttribute.addField( "Bilinear",   1 );
	enumAttribute.addField( "Box",        2 );
	enumAttribute.addField( "Gaussian",   3 );
	enumAttribute.addField( "Bicubic",    4 );
	enumAttribute.addField( "BSpline",    5 );
	enumAttribute.addField( "CatmullRom", 6 );
	enumAttribute.addField( "Mitchell",   7 );
	enumAttribute.setHidden( false );
	MAKE_INPUT( enumAttribute );
	enumAttribute.setConnectable(false);
	MCHECKERROR( status, "Error adding shapeType attribute." );

	MFnNumericAttribute filterSizeAttribute;
	aPtexFilterSize = filterSizeAttribute.create( "ptexFilterSize", "s", MFnNumericData::kFloat, 1.0 );
	MAKE_INPUT( filterSizeAttribute );
	filterSizeAttribute.setConnectable(false);

	// Implicit shading network attributes

	MObject child1 = numericAttribute.create( "uCoord", "u", MFnNumericData::kFloat);
	MObject child2 = numericAttribute.create( "vCoord", "v", MFnNumericData::kFloat);
	aUVPos = numericAttribute.create( "uvCoord", "uv", child1, child2);
	MAKE_INPUT( numericAttribute );
	CHECK_MSTATUS( numericAttribute.setHidden(true) );

	child1 = numericAttribute.create( "uvFilterSizeX", "fsx", MFnNumericData::kFloat);
	child2 = numericAttribute.create( "uvFilterSizeY", "fsy", MFnNumericData::kFloat);
	aUVSize = numericAttribute.create( "uvFilterSize", "fs", child1, child2 );
	MAKE_INPUT( numericAttribute );
	CHECK_MSTATUS( numericAttribute.setHidden(true) );

	// Output attributes
	aOutColor = numericAttribute.createColor("outColor", "oc");
	MAKE_OUTPUT(numericAttribute);

	// Add attributes to the node database.
	CHECK_MSTATUS( addAttribute(aPtexFileName) );
	CHECK_MSTATUS( addAttribute(aPtexFilterType) );
	CHECK_MSTATUS( addAttribute(aPtexFilterSize) );
	CHECK_MSTATUS( addAttribute(aUVPos) );
	CHECK_MSTATUS( addAttribute(aUVSize) );

	CHECK_MSTATUS( addAttribute(aOutColor) );

	// All input affect the output color
	CHECK_MSTATUS( attributeAffects( aPtexFileName,   aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aPtexFilterSize, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aPtexFilterType, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aUVPos,          aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aUVSize,         aOutColor ) );

	return MS::kSuccess;
}

MStatus PtexColorNode::compute(const MPlug& plug, MDataBlock& block) 
{
    if( ( plug != aOutColor ) && ( plug.parent() != aOutColor ) )
	{
		return MS::kUnknownParameter;
	}
	
	if ( m_ptex_cache == NULL )
	{
		m_ptex_cache = PtexCache::create( 0, 1024 * 1024 );
	}

	if ( m_ptex_cache && m_ptex_texture == 0 )
	{
		MDataHandle fileNameHnd = block.inputValue( aPtexFileName );
		MDataHandle filterTypeHnd = block.inputValue( aPtexFilterType );

		MString fileNameStr = fileNameHnd.asString();
		int filterTypeValue = filterTypeHnd.asInt();

		const float &filterSize = block.inputValue( aPtexFilterSize ).asFloat();

		if ( fileNameStr.length() )
		{
			Ptex::String error;
			m_ptex_texture = m_ptex_cache->get( fileNameStr.asChar(), error );
		}
		
		if ( m_ptex_texture == 0 )
		{
			MDataHandle outColorHandle = block.outputValue( aOutColor );
			MFloatVector& outColor = outColorHandle.asFloatVector();
			outColor.x = 1.0f;
			outColor.y = 0.0f;
			outColor.z = 1.0f;
			return MS::kSuccess;
		}

		m_ptex_num_channels = m_ptex_texture->numChannels();

		PtexFilter::FilterType ptexFilterType = PtexFilter::f_point;

		switch ( filterTypeValue )
		{
			case 0:   ptexFilterType = PtexFilter::f_point;        break;
			case 1:   ptexFilterType = PtexFilter::f_bilinear;     break;
			case 2:   ptexFilterType = PtexFilter::f_box;          break;
			case 3:   ptexFilterType = PtexFilter::f_gaussian;     break;
			case 4:   ptexFilterType = PtexFilter::f_bicubic;      break;
			case 5:   ptexFilterType = PtexFilter::f_bspline;      break;
			case 6:   ptexFilterType = PtexFilter::f_catmullrom;   break;
			case 7:   ptexFilterType = PtexFilter::f_mitchell;     break;
		}

		PtexFilter::Options opts( ptexFilterType, 0, filterSize );
		m_ptex_filter = PtexFilter::getFilter( m_ptex_texture, opts );
	}

	const float2 &uv  = block.inputValue( aUVPos  ).asFloat2();
	const float2 &duv = block.inputValue( aUVSize ).asFloat2();

	int f = (int)uv[ 0 ];

	float u = uv[ 0 ] - (float)f;
	float v = uv[ 1 ];

	float result[4];
	
	m_critical_section.lock();
	m_ptex_filter->eval( result, 0, m_ptex_num_channels, f, u, v, duv[ 0 ], 0, 0, duv[ 1 ] );
	m_critical_section.unlock();
	
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