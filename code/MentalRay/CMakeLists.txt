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

SET ( MENTAL_RAY_MAX_2008 "Max2008MentalRay" )
SET ( MENTAL_RAY_MAX_2009 "Max2009MentalRay" )
SET ( MENTAL_RAY_MAX_2010 "Max2010MentalRay" )
SET ( MENTAL_RAY_MAX_2011 "Max2011MentalRay" )

SET ( MENTAL_RAY_MAYA_2010 "Maya2010MentalRay" )
SET ( MENTAL_RAY_MAYA_2011 "Maya2011MentalRay" )

# Define the header libraries for each SDK

SET ( MENTALRAY_MAX_2008_HEADERS_DIR "c:/Libraries/maxsdk/10/mentalray/include" )
SET ( MENTALRAY_MAX_2009_HEADERS_DIR "c:/Libraries/maxsdk/11/mentalray/include" )
SET ( MENTALRAY_MAX_2010_HEADERS_DIR "c:/Libraries/maxsdk/12/mentalray/include" )
SET ( MENTALRAY_MAX_2011_HEADERS_DIR "c:/Libraries/maxsdk/13/mentalray/include" )

SET ( MENTALRAY_MAYA_2010_HEADERS_DIR "C:/Program Files (x86)/Autodesk/Maya2010/devkit/mentalray/include" )
SET ( MENTALRAY_MAYA_2011_HEADERS_DIR "C:/Program Files (x86)/Autodesk/Maya2011/devkit/mentalray/include" )

# Define the library files for each SDK, and platform. 64 bits lib path will replace
# 32 bits lib path.

# 32 BITS 

SET ( MENTALRAY_MAX_2008_LIBRARY_DIR "c:/Libraries/maxsdk/10/mentalray/lib32" )
SET ( MENTALRAY_MAX_2009_LIBRARY_DIR "c:/Libraries/maxsdk/11/mentalray/lib32" )
SET ( MENTALRAY_MAX_2010_LIBRARY_DIR "c:/Libraries/maxsdk/12/mentalray/lib32" )
SET ( MENTALRAY_MAX_2011_LIBRARY_DIR "c:/Libraries/maxsdk/13/mentalray/lib32" )

SET ( MENTALRAY_MAYA_2010_LIBRARY_DIR "C:/Program Files (x86)/Autodesk/Maya2010/devkit/mentalray/lib/nt" )
SET ( MENTALRAY_MAYA_2011_LIBRARY_DIR "C:/Program Files (x86)/Autodesk/Maya2011/devkit/mentalray/lib/nt" )

SET ( PROGRAM_FILES "c:\\Program Files (x86)" )

IF ( CMAKE_CL_64 ) # 64 BITS

	SET ( MENTALRAY_MAX_2008_LIBRARY_DIR "c:/Libraries/maxsdk/10/mentalray/lib64" )
	SET ( MENTALRAY_MAX_2009_LIBRARY_DIR "c:/Libraries/maxsdk/11/mentalray/lib64" )
	SET ( MENTALRAY_MAX_2010_LIBRARY_DIR "c:/Libraries/maxsdk/12/mentalray/lib64" )
	SET ( MENTALRAY_MAX_2011_LIBRARY_DIR "c:/Libraries/maxsdk/13/mentalray/lib64" )
	
	SET ( MENTALRAY_MAYA_2010_LIBRARY_DIR "C:/Program Files/Autodesk/Maya2010/devkit/mentalray/lib/nt" )
	SET ( MENTALRAY_MAYA_2011_LIBRARY_DIR "C:/Program Files/Autodesk/Maya2011/devkit/mentalray/lib/nt" )

	SET ( PROGRAM_FILES "c:\\Program Files" )
	
ENDIF ()

SET ( PTEX_HEADERS_DIR "${CMAKE_SOURCE_DIR}/zlib" "${CMAKE_SOURCE_DIR}/PtexLib" )

# Add the source files.

SET ( SOURCE_FILES
	"../PtexColor.cpp"
)

# Add the header files.

SET ( HEADER_FILES
	"../PtexColor.h"
)

# Define the max libraries.

SET( MENTAL_RAY_LIBS  
	"shader.lib"
)

ADD_SUBDIRECTORY( Max2010 )
ADD_SUBDIRECTORY( Max2011 )
ADD_SUBDIRECTORY( Maya2010 )
ADD_SUBDIRECTORY( Maya2011 )

