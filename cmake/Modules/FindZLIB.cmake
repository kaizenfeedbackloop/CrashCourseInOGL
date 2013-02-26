FIND_PATH( ZLIB_INCLUDE_PATH zlib.h
  ${PROJECT_SOURCE_DIR}/../zlib
  DOC "zlib root include")
FIND_LIBRARY( ZLIB_LIBRARY
  NAMES z zlib zdll zlib1 zlibd zlibd1
  PATHS
  ${PROJECT_SOURCE_DIR}/../zlib/.build/Debug
  ${PROJECT_SOURCE_DIR}/../zlib/.build/Release
  DOC "The ZLIB library")

IF (ZLIB_INCLUDE_PATH)
   SET( ZLIB_FOUND 1 CACHE STRING "Set to 1 if ZLIB is found, 0 otherwise")
ELSE (ZLIB_INCLUDE_PATH)
   SET( ZLIB_FOUND 0 CACHE STRING "Set to 1 if ZLIB is found, 0 otherwise")
ENDIF (ZLIB_INCLUDE_PATH)

MARK_AS_ADVANCED( ZLIB_FOUND )
