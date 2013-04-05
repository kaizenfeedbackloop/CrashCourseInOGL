#
# Try to find FREETYPE library and include path.
# Once done this will define
#
# FREETYPE_FOUND
# FREETYPE_INCLUDE_PATH
# FREETYPE_LIBRARY
# 

  FIND_PATH( FREETYPE_INCLUDE_PATH freetype/freetype.h
    ${PROJECT_SOURCE_DIR}/../freetype2/include
    DOC "The directory where freetype/freetype.h resides")
  FIND_LIBRARY( FREETYPE_LIBRARY
    NAMES freetype2411_D freetype2411 freetype
    PATHS
    ${PROJECT_SOURCE_DIR}/../freetype2/objs/win32/vc2010
    /usr/local/lib

    DOC "The FREETYPE library")

IF (FREETYPE_INCLUDE_PATH)
   SET( FREETYPE_FOUND 1 CACHE STRING "Set to 1 if FREETYPE is found, 0 otherwise")
ELSE (FREETYPE_INCLUDE_PATH)
   SET( FREETYPE_FOUND 0 CACHE STRING "Set to 1 if FREETYPE is found, 0 otherwise")
ENDIF (FREETYPE_INCLUDE_PATH)

MARK_AS_ADVANCED( FREETYPE_FOUND )
