SET ( PROJECT_NAME_MAYA_2010 "Maya2010" )
SET ( PROJECT_NAME_MAYA_2011 "Maya2011" )

SET ( MAYA_2010_HEADERS_DIR "C:/Program Files (x86)/Autodesk/Maya2010/include" )
SET ( MAYA_2010_LIBRARY_DIR "C:/Program Files (x86)/Autodesk/Maya2010/lib" )

SET ( MAYA_2011_HEADERS_DIR "C:/Program Files (x86)/Autodesk/Maya2011/include" )
SET ( MAYA_2011_LIBRARY_DIR "C:/Program Files (x86)/Autodesk/Maya2011/lib" )

SET ( PROGRAM_FILES "c:\\Program Files (x86)" )

IF ( CMAKE_CL_64 ) # 64 BITS

	SET ( MAYA_2010_HEADERS_DIR "C:/Program Files/Autodesk/Maya2010/include" )
	SET ( MAYA_2010_LIBRARY_DIR "C:/Program Files/Autodesk/Maya2010/lib" )	
	
	SET ( MAYA_2011_HEADERS_DIR "C:/Program Files/Autodesk/Maya2011/include" )
	SET ( MAYA_2011_LIBRARY_DIR "C:/Program Files/Autodesk/Maya2011/lib" )
	
	SET ( PROGRAM_FILES "c:\\Program Files (x86)" )
	
ENDIF ()

SET ( PTEX_HEADERS_DIR "${CMAKE_SOURCE_DIR}/zlib" "${CMAKE_SOURCE_DIR}/PtexLib" )

SET ( SOURCE_FILES "../CmpMeshModifierCmd.cpp" 
                   "../CmpMeshModifierNode.cpp" 
				   "../PtexUVCmd.cpp" 
				   "../PtexUVNode.cpp" 
				   "../PtexColorNode.cpp" 
				   "../PluginMain.cpp"
)

SET ( HEADER_FILES "../CmpMeshModifierCmd.h"   
                   "../CmpMeshModifierNode.h"   
				   "../PtexUVCmd.h"   
				   "../PtexUVNode.h"   
				   "../PtexColorNode.h"
)

SET( MAYA_LIBRARIES "Foundation.lib" "OpenMaya.lib" "OpenMayaAnim.lib" "OpenMayaRender.lib" )

SET ( CUSTOM_DEFINITIONS "_AFXDLL,_MBCS,NT_PLUGIN,REQUIRE_IOSTREAM" )

ADD_SUBDIRECTORY( Maya2010 )
ADD_SUBDIRECTORY( Maya2011 )
