#
# $Id$
#
# - Find libmemcached
# Find libmemcached
#
#  LIBMEMCACHED_INCLUDE_DIR - where to find libmemcached/memcached.h, etc.
#  LIBMEMCACHED_LIBRARY     - List of libraries when using libmemcached.
#  LIBMEMCACHED_FOUND       - True if libmemcached found.


IF (LIBMEMCACHED_INCLUDE_DIR)
    # Already in cache, be silent
    SET(LIBMEMCACHED_FIND_QUIETLY TRUE)
ENDIF ()

FIND_PATH(LIBMEMCACHED_INCLUDE_DIR libmemcached/memcached.h)

FIND_LIBRARY(LIBMEMCACHED_LIBRARY memcached)

# handle the QUIETLY and REQUIRED arguments and set Libmemcached_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBMEMCACHED DEFAULT_MSG LIBMEMCACHED_LIBRARY LIBMEMCACHED_INCLUDE_DIR)

SET(LIBMEMCACHED_VERSION 1.0)

IF (LIBMEMCACHED_FOUND)
    if (EXISTS "${LIBMEMCACHED_INCLUDE_DIR}/libmemcached/configure.h")
        FILE(READ "${LIBMEMCACHED_INCLUDE_DIR}/libmemcached/configure.h" _MEMCACHE_VERSION_CONENTS)
        STRING(REGEX REPLACE ".*#define LIBMEMCACHED_VERSION_STRING \"([0-9.]+)\".*" "\\1" LIBMEMCACHED_VERSION "${_MEMCACHE_VERSION_CONENTS}")
    endif ()
ENDIF ()

SET(LIBMEMCACHED_VERSION ${LIBMEMCACHED_VERSION} CACHE STRING "Version number of libmemcached")

MARK_AS_ADVANCED(LIBMEMCACHED_LIBRARY LIBMEMCACHED_INCLUDE_DIR LIBMEMCACHED_VERSION)