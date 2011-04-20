SET( PUBLIC_INCLUDE_DIRS
	"${CMAKE_SOURCE_DIR}/zlib"
)

#######################################################
# P T E X                                             #
#######################################################

SET ( PTEX_SOURCE_FILES
	"PtexCache.cpp"
	"PtexFilters.cpp"
	"PtexHalf.cpp"
	"PtexReader.cpp"
	"PtexSeparableFilter.cpp"
	"PtexSeparableKernel.cpp"
	"PtexTriangleFilter.cpp"
	"PtexTriangleKernel.cpp"
	"PtexUtils.cpp"
	"PtexWriter.cpp"
)

SET ( PTEX_HEADER_FILES
	"PtexCache.h"
	"PtexDict.h"
	"PtexHalf.h"
	"PtexHashMap.h"
	"PtexInt.h"
	"PtexIO.h"
	"PtexMutex.h"
	"PtexPlatform.h"
	"PtexReader.h"
	"PtexSeparableFilter.h"
	"PtexSeparableKernel.h"
	"PtexTriangleFilter.h"
	"PtexTriangleKernel.h"
	"Ptexture.h"
	"PtexUtils.h"
	"PtexWriter.h"
)

INCLUDE_DIRECTORIES ( ${PUBLIC_INCLUDE_DIRS} )
LINK_DIRECTORIES ( ${PUBLIC_LIBRARY_DIRS}  )

ADD_LIBRARY ( ${PTEX_NAME_LIB} STATIC ${PTEX_SOURCE_FILES} ${PTEX_HEADER_FILES} )

IF ( UNIX )
	# required to link against shared library
	SET_TARGET_PROPERTIES( ${PTEX_NAME_LIB} PROPERTIES COMPILE_FLAGS -fPIC )
ENDIF()

SET_TARGET_PROPERTIES( ${PTEX_NAME_LIB} PROPERTIES COMPILE_DEFINITIONS "PTEX_STATIC" )
SET_TARGET_PROPERTIES( ${PTEX_NAME_LIB} PROPERTIES OUTPUT_NAME "lib_Ptex" )

#ADD_LIBRARY ( ${PTEX_NAME_DLL} SHARED ${PTEX_SOURCE_FILES} ${PTEX_HEADER_FILES} )

#SET_TARGET_PROPERTIES( ${PTEX_NAME_DLL} PROPERTIES COMPILE_DEFINITIONS "PTEX_EXPORTS" )
#SET_TARGET_PROPERTIES( ${PTEX_NAME_DLL} PROPERTIES OUTPUT_NAME "Ptex" )
#TARGET_LINK_LIBRARIES( ${PTEX_NAME_DLL} ${PUBLIC_LIBRARIES} )