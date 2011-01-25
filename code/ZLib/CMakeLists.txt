
#######################################################
# Z L I B                                             #
#######################################################

SET ( ZLIB_HEADER_FILES
	"zlib.h"
	"crc32.h"
	"deflate.h"
	"gzguts.h"
	"inffast.h"
	"inffixed.h"
	"inflate.h"
	"inftrees.h"
	"trees.h"
	"zutil.h"
)

SET ( ZLIB_SOURCE_FILES
	"adler32.c"
	"compress.c"
	"crc32.c"
	"deflate.c"
	"gzclose.c"
	"gzlib.c"
	"gzread.c"
	"gzwrite.c"
	"inflate.c"
	"infback.c"
	"inftrees.c"
	"inffast.c"
	"trees.c"
	"uncompr.c"
	"zutil.c"
)

ADD_LIBRARY ( ${ZLIB_NAME_LIB} STATIC ${ZLIB_HEADER_FILES} ${ZLIB_SOURCE_FILES} )
SET_TARGET_PROPERTIES( ${ZLIB_NAME_LIB} PROPERTIES OUTPUT_NAME "lib_ZLib" )
