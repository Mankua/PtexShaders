

##################
# CONFIGURATION #
################
#set(CMAKE_VERBOSE_MAKEFILE 1)
# all maya versions we support
SET ( MAYA_VERSIONS 2012 2011 2010 2009 2008 )

SET (  CUSTOM_DEFINITIONS "REQUIRE_IOSTREAM;_BOOL" )

IF ( UNIX )
	SET (MAYA_INSTALL_BASE_DEFAULT /usr/autodesk)
ELSEIF ( APPLE )
	SET (MAYA_INSTALL_BASE_DEFAULT /Applications/Autodesk)
ELSE ( UNIX )
	SET ( MAYA_INSTALL_BASE_DEFAULT "c:/Program Files" )
	SET ( CUSTOM_DEFINITIONS "${CUSTOM_DEFINITIONS};_AFXDLL;_MBCS;NT_PLUGIN" )
	
	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )
ENDIF ()



SET( MAYA_INSTALL_BASE_PATH ${MAYA_INSTALL_BASE_DEFAULT} CACHE STRING
    "Path containing all your maya installations, like /usr/autodesk or /Applications/Autodesk/")

SET( MAYA_INSTALL_BASE_SUFFIX "" CACHE STRING
    "Suffix to append to maya installation directories, like -x64 on 64 bit systems on linux")
    
SET ( PTEX_HEADERS_DIR "${CMAKE_SOURCE_DIR}/zlib" "${CMAKE_SOURCE_DIR}/PtexLib" )


################
# SOURCE FILES #
################
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

INCLUDE_DIRECTORIES ( ${PUBLIC_INCLUDE_DIRS} ${PTEX_HEADERS_DIR} )
LINK_DIRECTORIES ( ${PUBLIC_LIBRARY_DIRS} )
SET( MAYA_LIBRARIES Foundation OpenMaya OpenMayaAnim OpenMayaRender )

FUNCTION ( add_maya_build VERSION )
	SET ( _MAYA_LOCATION ${MAYA_INSTALL_BASE_PATH}/maya${VERSION}${MAYA_INSTALL_BASE_SUFFIX} )
	SET ( _PROJECT Maya_${VERSION} )
	
	INCLUDE_DIRECTORIES ( ${_MAYA_LOCATION}/include )
	LINK_DIRECTORIES ( ${PUBLIC_LIBRARY_DIRS} ${_MAYA_LOCATION}/lib )
	
	ADD_LIBRARY ( ${_PROJECT} SHARED ${SOURCE_FILES} ${HEADER_FILES} )
	TARGET_LINK_LIBRARIES( ${_PROJECT} ${MAYA_LIBRARIES} ${PTEX_NAME_LIB} ${ZLIB_NAME_LIB} )
	
	SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES COMPILE_DEFINITIONS "${CUSTOM_DEFINITIONS}" )
	SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES OUTPUT_NAME ptex${VERSION} )
	SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )
	
	IF (WIN32)
	
		SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES OUTPUT_NAME "Maya${VERSION}/Ptex" )
		SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES SUFFIX ".mll" )
		SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES LINK_FLAGS "/export:initializePlugin /export:uninitializePlugin" )
		
		ADD_CUSTOM_COMMAND( TARGET ${_PROJECT} POST_BUILD COMMAND copy ..\\..\\Bin\\$(ConfigurationName)\\Maya2011\\Ptex.mll "${MAYA_INSTALL_BASE_PATH}\\Maya${VERSION}\\bin\\plug-ins" )	
		ADD_CUSTOM_COMMAND( TARGET ${_PROJECT} POST_BUILD COMMAND copy ..\\..\\..\\code\\Maya\\AEptexTemplate.mel "${MAYA_INSTALL_BASE_PATH}\\Maya${VERSION}\\scripts\\AETemplates" )	

	ENDIF()
	
ENDFUNCTION ( add_maya_build )

FOREACH ( VERSION IN LISTS MAYA_VERSIONS )
	ADD_SUBDIRECTORY(Maya${VERSION})
ENDFOREACH()

