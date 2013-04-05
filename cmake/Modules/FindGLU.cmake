# - try to find glu library and include files
#  GLU_INCLUDE_DIR, where to find GL/glu.h, etc.
#  GLU_LIBRARIES, the libraries to link against

IF (WIN32)

  IF(CYGWIN)

    FIND_PATH( GLU_INCLUDE_DIR GL/glu.h
      /usr/include
    )

    FIND_LIBRARY( GLU_glu_LIBRARY glu32
      ${OPENGL_LIBRARY_DIR}
      /usr/lib
      /usr/lib/w32api
      /usr/local/lib
      /usr/X11R6/lib
    )


  ELSE(CYGWIN)

#    FIND_PATH( GLU_INCLUDE_DIR GL/glu.h
#     ${GLU_ROOT_PATH}/include
#   )

#   FIND_LIBRARY( GLU_glu_LIBRARY glu32
#     ${GLU_ROOT_PATH}/lib
#     ${OPENGL_LIBRARY_DIR}
#   )

FIND_PATH( GLU_INCLUDE_DIR GL/glu.h
  ${GLU_ROOT_PATH}/include
  ${PROJECT_SOURCE_DIR}/extern/glu/include
  DOC "The directory where GL/glu.h resides")
FIND_LIBRARY( GLU_glu_LIBRARY
  NAMES glu GLU glu32 glu32s
  PATHS
  ${GLU_ROOT_PATH}/lib
  ${PROJECT_SOURCE_DIR}/extern/glu/bin
  ${PROJECT_SOURCE_DIR}/extern/glu/lib
  ${OPENGL_LIBRARY_DIR}
  DOC "The GLU library")

  ENDIF(CYGWIN)

ELSE (WIN32)

    FIND_PATH( GLU_INCLUDE_DIR GL/glu.h
      /usr/include
      /usr/include/GL
      /usr/local/include
      /usr/openwin/share/include
      /usr/openwin/include
      /usr/X11R6/include
      /usr/include/X11
      /opt/graphics/OpenGL/include
      /opt/graphics/OpenGL/contrib/libglu
    )

    FIND_LIBRARY( GLU_glu_LIBRARY
      NAMES glu GLU glu32 glu32s
      PATHS
      /usr/lib
      /usr/local/lib
      /usr/openwin/lib
      /usr/X11R6/lib
    )

ENDIF (WIN32)

SET( GLU_FOUND "NO" )
IF(GLU_INCLUDE_DIR)
  IF(GLU_glu_LIBRARY)
    SET( GLU_LIBRARIES
      ${GLU_glu_LIBRARY}
    )
    SET( GLU_FOUND "YES" )

#The following deprecated settings are for backwards compatibility with CMake1.4
    SET (GLU_LIBRARY ${GLU_LIBRARIES})
    SET (GLU_INCLUDE_PATH ${GLU_INCLUDE_DIR})

  ENDIF(GLU_glu_LIBRARY)
ENDIF(GLU_INCLUDE_DIR)

MARK_AS_ADVANCED(
  GLU_INCLUDE_DIR
  GLU_glu_LIBRARY
)