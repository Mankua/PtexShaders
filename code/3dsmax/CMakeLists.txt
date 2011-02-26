# 3 d s   m a x

# 3ds Max 9    | MAXSDK9  | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2008 | MAXSDK10 | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2009 | MAXSDK11 | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2010 | MAXSDK12 | Visual Studio 9.0 | 32 Bits | 64 Bits 
# 3ds Max 2011 | MAXSDK13 | Visual Studio 9.0 | 32 Bits | 64 Bits 
                                                      
# Define the Project Name.
#  Projects that are built with the same IDE (ie. Visual Studio 2005) will be
#  added to the same Solution. For this reason all project need to have 
#  unique names.

SET ( PROJECT_NAME_MAX_6  "PtexMax6" )
SET ( PROJECT_NAME_MAX_9  "PtexMax9" )
SET ( PROJECT_NAME_MAX_10 "PtexMax2008" )
SET ( PROJECT_NAME_MAX_11 "PtexMax2009" )
SET ( PROJECT_NAME_MAX_12 "PtexMax2010" )
SET ( PROJECT_NAME_MAX_13 "PtexMax2011" )

# Define the header libraries for each SDK

SET ( MAXSDK6_HEADERS_DIR  "c:/Libraries/maxsdk/6/include" )
SET ( MAXSDK9_HEADERS_DIR  "c:/Libraries/maxsdk/9/include" )
SET ( MAXSDK10_HEADERS_DIR "c:/Libraries/maxsdk/10/include" )
SET ( MAXSDK11_HEADERS_DIR "c:/Libraries/maxsdk/11/include" )
SET ( MAXSDK12_HEADERS_DIR "c:/Libraries/maxsdk/12/include" )
SET ( MAXSDK13_HEADERS_DIR "c:/Libraries/maxsdk/13/include" )

# Define the library files for each SDK, and platform. 64 bits lib path will replace
# 32 bits lib path.

# 32 BITS 

SET ( MAXSDK6_LIBRARY_DIR  "c:/Libraries/maxsdk/6/lib" )
SET ( MAXSDK9_LIBRARY_DIR  "c:/Libraries/maxsdk/9/lib" )
SET ( MAXSDK10_LIBRARY_DIR "c:/Libraries/maxsdk/10/lib" )
SET ( MAXSDK11_LIBRARY_DIR "c:/Libraries/maxsdk/11/lib" )
SET ( MAXSDK12_LIBRARY_DIR "c:/Libraries/maxsdk/12/lib" )
SET ( MAXSDK13_LIBRARY_DIR "c:/Libraries/maxsdk/13/lib" )

SET ( PROGRAM_FILES "c:\\Program Files (x86)" )

IF ( CMAKE_CL_64 ) # 64 BITS

	SET ( MAXSDK9_LIBRARY_DIR  "c:/Libraries/maxsdk/9/x64/lib" )
	SET ( MAXSDK10_LIBRARY_DIR "c:/Libraries/maxsdk/10/x64/lib" )
	SET ( MAXSDK11_LIBRARY_DIR "c:/Libraries/maxsdk/11/x64/lib" )
	SET ( MAXSDK12_LIBRARY_DIR "c:/Libraries/maxsdk/12/x64/lib" )
	SET ( MAXSDK13_LIBRARY_DIR "c:/Libraries/maxsdk/13/x64/lib" )
	
	SET ( PROGRAM_FILES "c:\\Program Files" )
	
ENDIF ()

SET ( PTEX_HEADERS_DIR "${CMAKE_SOURCE_DIR}/zlib" "${CMAKE_SOURCE_DIR}/PtexLib" )

# Add the source files.

SET ( SOURCE_FILES
	"About.cpp"
	"Debug.cpp"
	"DLLEntry.cpp"
	"Modifier.cpp"
	"Texture.cpp"
)

# Add the header files.

SET ( HEADER_FILES
	"About.h"
	"Debug.h"
	"Modifier.h"
	"Texture.h"
)

# Add the resource and definition file.

SET ( RESOURCE_FILES
  "Ptex.rc"
  "Ptex.def"
)

# Define the max libraries.

SET( MAX_LIBS  "mnmath.lib"
			   "poly.lib"
			   "paramblk2.lib"
			   "core.lib"
			   "geom.lib"
			   "mesh.lib"
			   "maxutil.lib"
			   "bmm.lib"
			   "maxscrpt.lib"
			   "comctl32.lib"
)

IF ( MSVC80 )

	#######################################################
	# P L U G I N   F O R   3 D S M A X   9               #
	#######################################################
	
	INCLUDE_DIRECTORIES ( ${MAXSDK9_HEADERS_DIR} ${PTEX_HEADERS_DIR} )
	LINK_DIRECTORIES ( ${MAXSDK9_LIBRARY_DIR} )

	ADD_LIBRARY ( ${PROJECT_NAME_MAX_9} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME_MAX_9} ${MAX_LIBS} ${PTEX_NAME_LIB} ${ZLIB_NAME_LIB} )

	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_9} PROPERTIES OUTPUT_NAME "Max9/Ptex" )
	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_9} PROPERTIES SUFFIX ".dlt" )
	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_9} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )
	
	#######################################################
	# P L U G I N   F O R   3 D S M A X   2 0 0 8         #
	#######################################################
	
	INCLUDE_DIRECTORIES ( ${MAXSDK10_HEADERS_DIR} ${PTEX_HEADERS_DIR} )
	LINK_DIRECTORIES ( ${MAXSDK10_LIBRARY_DIR} )

	ADD_LIBRARY ( ${PROJECT_NAME_MAX_10} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME_MAX_10} ${MAX_LIBS} ${PTEX_NAME_LIB} ${ZLIB_NAME_LIB} )

	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_10} PROPERTIES OUTPUT_NAME "Max2008/Ptex" )
	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_10} PROPERTIES SUFFIX ".dlt" )
	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_10} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )
	
ENDIF ( MSVC80 )

IF ( MSVC90 )

	#######################################################
	# P L U G I N   F O R   3 D S M A X   2 0 1 0         #
	#######################################################
	
	INCLUDE_DIRECTORIES ( ${MAXSDK12_HEADERS_DIR} ${PTEX_HEADERS_DIR} )
	LINK_DIRECTORIES ( ${MAXSDK12_LIBRARY_DIR} )
	
	ADD_LIBRARY ( ${PROJECT_NAME_MAX_12} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME_MAX_12} ${MAX_LIBS} ${PTEX_LIBS} ${PTEX_NAME_LIB} ${ZLIB_NAME_LIB} )

	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_12} PROPERTIES OUTPUT_NAME "Max2010/Ptex" )
	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_12} PROPERTIES SUFFIX ".dlt" )
	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_12} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )	
	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_12} PROPERTIES COMPILE_DEFINITIONS ${DEPRECATION_FLAGS} )
	
	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )
	
	ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME_MAX_12} POST_BUILD COMMAND copy ..\\Bin\\$(ConfigurationName)\\Max2010\\Ptex.dlt "${PROGRAM_FILES}\\Autodesk\\3ds Max 2010\\plugins" )
	
	#######################################################
	# P L U G I N   F O R   3 D S M A X   2 0 1 1         #
	#######################################################
	
	INCLUDE_DIRECTORIES ( ${MAXSDK13_HEADERS_DIR} ${PTEX_HEADERS_DIR} )
	LINK_DIRECTORIES ( ${MAXSDK13_LIBRARY_DIR} )
	
	ADD_LIBRARY ( ${PROJECT_NAME_MAX_13} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME_MAX_13} ${MAX_LIBS} ${PTEX_LIBS} ${PTEX_NAME_LIB} ${ZLIB_NAME_LIB} )

	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_13} PROPERTIES OUTPUT_NAME "Max2011/Ptex" )
	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_13} PROPERTIES SUFFIX ".dlt" )
	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_13} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )	
	SET_TARGET_PROPERTIES( ${PROJECT_NAME_MAX_13} PROPERTIES COMPILE_DEFINITIONS ${DEPRECATION_FLAGS} )
	
	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )

	ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME_MAX_13} POST_BUILD COMMAND copy ..\\Bin\\$(ConfigurationName)\\Max2011\\Ptex.dlt "${PROGRAM_FILES}\\Autodesk\\3ds Max 2011\\plugins" )
  
ENDIF ( MSVC90 )
