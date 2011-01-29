# Mental Ray Shader

# 3ds Max 9    | MAXSDK9  | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2008 | MAXSDK10 | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2009 | MAXSDK11 | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2010 | MAXSDK12 | Visual Studio 9.0 | 32 Bits | 64 Bits 
# 3ds Max 2011 | MAXSDK13 | Visual Studio 9.0 | 32 Bits | 64 Bits 
                                                      
# Define the Project Name.
#  Projects that are built with the same IDE (ie. Visual Studio 2005) will be
#  added to the same Solution. For this reason all project need to have 
#  unique names.

SET ( MENTAL_RAY_MAX_6  "PtexMentalRayMax6" )
SET ( MENTAL_RAY_MAX_9  "PtexMentalRayMax9" )
SET ( MENTAL_RAY_MAX_10 "PtexMentalRayMax2008" )
SET ( MENTAL_RAY_MAX_11 "PtexMentalRayMax2009" )
SET ( MENTAL_RAY_MAX_12 "PtexMentalRayMax2010" )
SET ( MENTAL_RAY_MAX_13 "PtexMentalRayMax2011" )

# Define the header libraries for each SDK

SET ( MENTALRAY6_HEADERS_DIR  "c:/Libraries/maxsdk/6/mentalray/include" )
SET ( MENTALRAY9_HEADERS_DIR  "c:/Libraries/maxsdk/9/mentalray/include" )
SET ( MENTALRAY10_HEADERS_DIR "c:/Libraries/maxsdk/10/mentalray/include" )
SET ( MENTALRAY11_HEADERS_DIR "c:/Libraries/maxsdk/11/mentalray/include" )
SET ( MENTALRAY12_HEADERS_DIR "c:/Libraries/maxsdk/12/mentalray/include" )
SET ( MENTALRAY13_HEADERS_DIR "c:/Libraries/maxsdk/13/mentalray/include" )

# Define the library files for each SDK, and platform. 64 bits lib path will replace
# 32 bits lib path.

# 32 BITS 

SET ( MENTALRAY6_LIBRARY_DIR  "c:/Libraries/maxsdk/6/mentalray/lib32" )
SET ( MENTALRAY9_LIBRARY_DIR  "c:/Libraries/maxsdk/9/mentalray/lib32" )
SET ( MENTALRAY10_LIBRARY_DIR "c:/Libraries/maxsdk/10/mentalray/lib32" )
SET ( MENTALRAY11_LIBRARY_DIR "c:/Libraries/maxsdk/11/mentalray/lib32" )
SET ( MENTALRAY12_LIBRARY_DIR "c:/Libraries/maxsdk/12/mentalray/lib32" )
SET ( MENTALRAY13_LIBRARY_DIR "c:/Libraries/maxsdk/13/mentalray/lib32" )

SET ( PROGRAM_FILES "c:\\Program Files (x86)" )

IF ( CMAKE_CL_64 ) # 64 BITS

	SET ( MENTALRAY9_LIBRARY_DIR  "c:/Libraries/maxsdk/9/mentalray/lib64" )
	SET ( MENTALRAY10_LIBRARY_DIR "c:/Libraries/maxsdk/10/mentalray/lib64" )
	SET ( MENTALRAY11_LIBRARY_DIR "c:/Libraries/maxsdk/11/mentalray/lib64" )
	SET ( MENTALRAY12_LIBRARY_DIR "c:/Libraries/maxsdk/12/mentalray/lib64" )
	SET ( MENTALRAY13_LIBRARY_DIR "c:/Libraries/maxsdk/13/mentalray/lib64" )
	
	SET ( PROGRAM_FILES "c:\\Program Files" )
	
ENDIF ()

SET ( PTEX_HEADERS_DIR "${CMAKE_SOURCE_DIR}/zlib" "${CMAKE_SOURCE_DIR}/PtexLib" )

# Add the source files.

SET ( SOURCE_FILES
	"PtexColor.cpp"
)

# Add the header files.

SET ( HEADER_FILES
	"PtexColor.h"
)

# Define the max libraries.

SET( MAX_LIBS  
	"shader.lib"
)

IF ( MSVC80 )

	#######################################################
	# P L U G I N   F O R   3 D S M A X   9               #
	#######################################################
	
	INCLUDE_DIRECTORIES ( ${MENTALRAY9_HEADERS_DIR} ${PTEX_HEADERS_DIR} )
	LINK_DIRECTORIES ( ${MENTALRAY9_LIBRARY_DIR} )

	ADD_LIBRARY ( ${MENTAL_RAY_MAX_9} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
	TARGET_LINK_LIBRARIES( ${MENTAL_RAY_MAX_9} ${MAX_LIBS} ${PTEX_LIBS} ${PTEX_NAME_LIB} ${ZLIB_NAME_LIB} )

	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_9} PROPERTIES OUTPUT_NAME "Max9/Ptex" )
	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_9} PROPERTIES SUFFIX ".dll" )
	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_9} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )
	
	ADD_CUSTOM_COMMAND( TARGET "ptex" POST_BUILD COMMAND "copy this" )

	#######################################################
	# P L U G I N   F O R   3 D S M A X   2 0 0 8         #
	#######################################################
	
	INCLUDE_DIRECTORIES ( ${MENTALRAY10_HEADERS_DIR} ${PTEX_HEADERS_DIR} )
	LINK_DIRECTORIES ( ${MENTALRAY10_LIBRARY_DIR} )

	ADD_LIBRARY ( ${MENTAL_RAY_MAX_10} SHARED ${SOURCE_FILES} ${HEADER_FILES} )
	TARGET_LINK_LIBRARIES( ${MENTAL_RAY_MAX_10} ${MAX_LIBS} ${PTEX_LIBS} ${PTEX_NAME_LIB} ${ZLIB_NAME_LIB} )

	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_10} PROPERTIES OUTPUT_NAME "Max2008/Ptex" )
	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_10} PROPERTIES SUFFIX ".dll" )
	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_10} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )

ENDIF ( MSVC80 )

IF ( MSVC90 )

	#######################################################
	# P L U G I N   F O R   3 D S M A X   2 0 1 0         #
	#######################################################
	
	INCLUDE_DIRECTORIES ( ${MENTALRAY12_HEADERS_DIR} ${PTEX_HEADERS_DIR} )
	LINK_DIRECTORIES ( ${MENTALRAY12_LIBRARY_DIR} )
	
	ADD_LIBRARY ( ${MENTAL_RAY_MAX_12} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
	TARGET_LINK_LIBRARIES( ${MENTAL_RAY_MAX_12} ${MAX_LIBS} ${PTEX_LIBS} ${PTEX_NAME_LIB} ${ZLIB_NAME_LIB} )

	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_12} PROPERTIES OUTPUT_NAME "Max2010/Ptex" )
	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_12} PROPERTIES SUFFIX ".dll" )
	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_12} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )	
	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_12} PROPERTIES COMPILE_DEFINITIONS ${DEPRECATION_FLAGS} )
	
	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )
	
	ADD_CUSTOM_COMMAND( TARGET ${MENTAL_RAY_MAX_12} POST_BUILD COMMAND copy ..\\Bin\\$(ConfigurationName)\\Max2010\\Ptex.dll "${PROGRAM_FILES}\\Autodesk\\3ds Max 2010\\mentalray\\shaders_3rdparty\\shaders" )
	
	ADD_CUSTOM_COMMAND( TARGET ${MENTAL_RAY_MAX_12} POST_BUILD COMMAND copy ..\\..\\code\\MentalRay\\Ptex.mi "${PROGRAM_FILES}\\Autodesk\\3ds Max 2010\\mentalray\\shaders_3rdparty\\include" )
	
	#######################################################
	# P L U G I N   F O R   3 D S M A X   2 0 1 1         #
	#######################################################
	
	INCLUDE_DIRECTORIES ( ${MENTALRAY13_HEADERS_DIR} ${PTEX_HEADERS_DIR} )
	LINK_DIRECTORIES ( ${MENTALRAY13_LIBRARY_DIR} )
	
	ADD_LIBRARY ( ${MENTAL_RAY_MAX_13} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} )
	TARGET_LINK_LIBRARIES( ${MENTAL_RAY_MAX_13} ${MAX_LIBS} ${PTEX_LIBS} ${PTEX_NAME_LIB} ${ZLIB_NAME_LIB} )

	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_13} PROPERTIES OUTPUT_NAME "Max2011/Ptex" )
	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_13} PROPERTIES SUFFIX ".dll" )
	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_13} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )	
	SET_TARGET_PROPERTIES( ${MENTAL_RAY_MAX_13} PROPERTIES COMPILE_DEFINITIONS ${DEPRECATION_FLAGS} )
	
	SET ( LOCAL_WARNING_FLAGS /W3 )
	SET ( LOCAL_RTTI_FLAGS /GR )

	ADD_CUSTOM_COMMAND( TARGET ${MENTAL_RAY_MAX_13} POST_BUILD COMMAND copy ..\\Bin\\$(ConfigurationName)\\Max2011/Ptex.dll "${PROGRAM_FILES}\\Autodesk\\3ds Max 2011\\mentalray\\shaders_3rdparty\\shaders" )
	
	ADD_CUSTOM_COMMAND( TARGET ${MENTAL_RAY_MAX_13} POST_BUILD COMMAND copy ..\\..\\code\\MentalRay\\Ptex.mi "${PROGRAM_FILES}\\Autodesk\\3ds Max 2010\\mentalray\\shaders_3rdparty\\include" )
		
ENDIF ( MSVC90 )
