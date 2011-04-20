# all maya versions we support

##################
# CONFIGURATION #
################
SET ( MAYA_VERSIONS 2012 2011 2010 2009 2008 8.5 )

SET (  CUSTOM_DEFINITIONS "REQUIRE_IOSTREAM;_BOOL" )

# optional base that we will use for default configuration, can be set by the user
SET (LINK_DIR_FLAG -L)
SET (INCLUDE_DIR_FLAG -I)

IF ( UNIX )
	SET (MAYA_INSTALL_BASE_DEFAULT /usr/autodesk)
ELSEIF ( APPLE )
	SET (MAYA_INSTALL_BASE_DEFAULT /Applications/Autodesk)
ELSE ( UNIX )
	SET ( MAYA_INSTALL_BASE_DEFAULT "c:/Program Files" )
	SET ( CUSTOM_DEFINITIONS ${CUSTOM_DEFINITIONS}"_AFXDLL;_MBCS NT_PLUGIN" )
	
	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )
	
	SET (LINK_DIR_FLAG "/LIBPATH:")
	SET (INCLUDE_DIR_FLAG "/I")
ENDIF ()



SET( MAYA_INSTALL_BASE_PATH ${MAYA_INSTALL_BASE_DEFAULT} CACHE STRING
    "Path containing all your maya installations, like /usr/autodesk or /Applications/Autodesk/")

SET( MAYA_INSTALL_BASE_SUFFIX "" CACHE STRING
    "Suffix to append to maya installation directories, like -x64 on 64 bit systems on linux")
    
SET ( PTEX_HEADERS_DIR "${CMAKE_SOURCE_DIR}/zlib" "${CMAKE_SOURCE_DIR}/PtexLib" )


################
# SOURCE FILES #
################
SET ( SOURCE_FILES "CmpMeshModifierCmd.cpp" 
                   "CmpMeshModifierNode.cpp" 
				   "PtexUVCmd.cpp" 
				   "PtexUVNode.cpp" 
				   "PtexColorNode.cpp" 
				   "PluginMain.cpp"
)

SET ( HEADER_FILES "CmpMeshModifierCmd.h"   
                   "CmpMeshModifierNode.h"   
				   "PtexUVCmd.h"   
				   "PtexUVNode.h"   
				   "PtexColorNode.h"
)

INCLUDE_DIRECTORIES ( ${PUBLIC_INCLUDE_DIRS} ${PTEX_HEADERS_DIR} )
LINK_DIRECTORIES ( ${PUBLIC_LIBRARY_DIRS} )

FOREACH (VERSION IN LISTS MAYA_VERSIONS)
	SET ( _MAYA_LOCATION ${MAYA_INSTALL_BASE_PATH}/maya${VERSION}${MAYA_INSTALL_BASE_SUFFIX} )
	SET ( _PROJECT Maya_${VERSION} )
	
	ADD_LIBRARY ( ${_PROJECT} SHARED ${SOURCE_FILES} ${HEADER_FILES} )
	TARGET_LINK_LIBRARIES( ${_PROJECT} ${MAYA_LIBRARIES} ${PTEX_NAME_LIB} ${ZLIB_NAME_LIB} )
	
	#INCLUDE_DIRECTORIES ( ${_MAYA_LOCATION}/include )
	#LINK_DIRECTORIES ( ${PUBLIC_LIBRARY_DIRS} ${_MAYA_LOCATION}/lib )
	
	SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES COMPILE_DEFINITIONS "${CUSTOM_DEFINITIONS}" )
	SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES OUTPUT_NAME Maya${VERSION}/ptex )
	SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )
	
	# What we really want is to define include and link directories per target. This is not possible
	# with cmake though, so we have to do it the hard way which should work unless some very special compiler 
	# is used. Maybe its best to just allow to compile for one version at once, setting the respective mayalocation
	# variable and nothing more (on linux this could easily be done from make itself, on windows it might better be done 
	# hardcoded )
	SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES COMPILE_FLAGS ${INCLUDE_DIR_FLAG}${_MAYA_LOCATION}/include )
	SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES LINK_FLAGS "${LINK_DIR_FLAG}${_MAYA_LOCATION}/lib" )
	
	if (WIN32)
		SET_TARGET_PROPERTIES( ${_PROJECT} PROPERTIES SUFFIX ".mll" )
	endif()
ENDFOREACH()

SET( MAYA_LIBRARIES Foundation OpenMaya OpenMayaAnim OpenMayaRender )

