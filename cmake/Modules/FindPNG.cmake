FIND_PATH( PNG_INCLUDE_PATH png.h
  ${PROJECT_SOURCE_DIR}/../libpng
  DOC "zlib root include")
FIND_LIBRARY( PNG_LIBRARY
  NAMES png libpng png15 libpng15 png15d libpng15d png14 libpng14 png14d libpng14d png12 libpng12 png12d libpng12d
  PATHS
  ${PROJECT_SOURCE_DIR}/../libpng/.build/Debug
  ${PROJECT_SOURCE_DIR}/../libpng/.build/Release
  DOC "The PNG library")

IF (PNG_INCLUDE_PATH)
   SET( PNG_FOUND 1 CACHE STRING "Set to 1 if PNG is found, 0 otherwise")
ELSE (PNG_INCLUDE_PATH)
   SET( PNG_FOUND 0 CACHE STRING "Set to 1 if PNG is found, 0 otherwise")
ENDIF (PNG_INCLUDE_PATH)

MARK_AS_ADVANCED( PNG_FOUND )
