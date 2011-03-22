#ifndef _PTEX_COLOR_NODE_H_
#define _PTEX_COLOR_NODE_H_

#include <map>

#include <maya/MPxNode.h>
#include <maya/MRenderCallback.h>

class PtexCache;
class PtexTexture;
class PtexFilter;

class PtexColorNode : public MPxNode, public MRenderCallback 
{
public:

	PtexColorNode();
	virtual         ~PtexColorNode();

	virtual MStatus compute( const MPlug&, MDataBlock& );
	virtual void    postConstructor();

	static  void *  creator();
	static  MStatus initialize();

	//  Id tag for use with binary file format
	static  MTypeId id;

	virtual bool shadowCastCallback( const MRenderShadowData &data );
	virtual bool renderCallback( const MRenderData &data );
	virtual bool postProcessCallback( const MRenderData &data );

private:

	// Input attributes
	static MObject aPtexFileName;
	static MObject aPtexFilterType;
	static MObject aPtexFilterSize;

	static MObject aUVPos;
	static MObject aUVSize;

	// Output attributes
	static MObject aOutColor;

	PtexCache * m_ptex_cache;
	PtexTexture * m_ptex_texture;
	int m_ptex_num_channels;
	PtexFilter * m_ptex_filter;

	CRITICAL_SECTION m_critical_section;
};

#endif
