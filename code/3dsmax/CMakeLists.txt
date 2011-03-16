# 3 d s   m a x

# 3ds Max 2008 | MAXSDK10 | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2009 | MAXSDK11 | Visual Studio 8.0 | 32 Bits | 64 Bits 
# 3ds Max 2010 | MAXSDK12 | Visual Studio 9.0 | 32 Bits | 64 Bits 
# 3ds Max 2011 | MAXSDK13 | Visual Studio 9.0 | 32 Bits | 64 Bits 
                                                      
# Define the Project Name.
#  Projects that are built with the same IDE (ie. Visual Studio 2005) will be
#  added to the same Solution. For this reason all project need to have 
#  unique names.

SET ( PROJECT_NAME_MAX_2008 "Max2008" )
SET ( PROJECT_NAME_MAX_2009 "Max2009" )
SET ( PROJECT_NAME_MAX_2010 "Max2010" )
SET ( PROJECT_NAME_MAX_2011 "Max2011" )

# Define the header libraries for each SDK

SET ( MAXSDK_2008_HEADERS_DIR "c:/Libraries/maxsdk/10/include" )
SET ( MAXSDK_2009_HEADERS_DIR "c:/Libraries/maxsdk/11/include" )
SET ( MAXSDK_2010_HEADERS_DIR "c:/Libraries/maxsdk/12/include" )
SET ( MAXSDK_2011_HEADERS_DIR "c:/Libraries/maxsdk/13/include" )

# Define the library files for each SDK, and platform. 64 bits lib path will replace
# 32 bits lib path.

# 32 BITS 

SET ( MAXSDK_2008_LIBRARY_DIR "c:/Libraries/maxsdk/10/lib" )
SET ( MAXSDK_2009_LIBRARY_DIR "c:/Libraries/maxsdk/11/lib" )
SET ( MAXSDK_2010_LIBRARY_DIR "c:/Libraries/maxsdk/12/lib" )
SET ( MAXSDK_2011_LIBRARY_DIR "c:/Libraries/maxsdk/13/lib" )

SET ( PROGRAM_FILES "c:\\Program Files (x86)" )

IF ( CMAKE_CL_64 ) # 64 BITS

	SET ( MAXSDK_2008_LIBRARY_DIR "c:/Libraries/maxsdk/10/x64/lib" )
	SET ( MAXSDK_2009_LIBRARY_DIR "c:/Libraries/maxsdk/11/x64/lib" )
	SET ( MAXSDK_2010_LIBRARY_DIR "c:/Libraries/maxsdk/12/x64/lib" )
	SET ( MAXSDK_2011_LIBRARY_DIR "c:/Libraries/maxsdk/13/x64/lib" )
	
	SET ( PROGRAM_FILES "c:\\Program Files" )
	
ENDIF ()

SET ( PTEX_HEADERS_DIR "${CMAKE_SOURCE_DIR}/zlib" "${CMAKE_SOURCE_DIR}/PtexLib" )

# Add the source files.

SET ( SOURCE_FILES
	"../About.cpp"
	"../Debug.cpp"
	"../DLLEntry.cpp"
	"../Modifier.cpp"
	"../Texture.cpp"
)

# Add the header files.

SET ( HEADER_FILES
	"../About.h"
	"../Debug.h"
	"../Modifier.h"
	"../Texture.h"
)

# Add the resource and definition file.

SET ( RESOURCE_FILES
	"../Ptex.rc"
	"../Ptex.def"
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

ADD_SUBDIRECTORY( Max2010 )
ADD_SUBDIRECTORY( Max2011 )
