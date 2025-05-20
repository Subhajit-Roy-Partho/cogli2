CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# if both legacy and GLVND libraries are available, then we want to use the latter by default 
if(POLICY CMP0072)
	cmake_policy(SET CMP0072 NEW)
endif()

OPTION(Debug "Set to ON to compile with debug symbols" OFF)
OPTION(G "Set to ON to compile with optimisations and debug symbols" OFF)
OPTION(LR_TETRA "Enable povray output for DNA tetramers" OFF)
OPTION(NOPNG "Disable png support" OFF)

PROJECT(cogli2)

# c++-11 support is mandatory
IF(CMAKE_VERSION VERSION_LESS "3.1")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSE()
	SET(CMAKE_CXX_STANDARD 11)
	SET(CMAKE_CXX_STANDARD_REQUIRED ON)
ENDIF()

# The default behaviour is to compile with optimisations
SET(CMAKE_BUILD_TYPE Release)

IF(Debug)
	SET(CMAKE_BUILD_TYPE Debug)
	MESSAGE(STATUS "Compiling in Debug mode")
ENDIF(Debug)

IF(G)
	SET(CMAKE_BUILD_TYPE RelWithDebInfo)
	MESSAGE(STATUS "Compiling in Release+Debug symbols mode")
ENDIF(G)

IF(NOT CMAKE_BUILD_TYPE MATCHES Debug)
	IF(INTEL)
		ADD_DEFINITIONS(-fast)
	ELSE()
		ADD_DEFINITIONS(-ffast-math)
	ENDIF(INTEL)
ENDIF()

# get the current svn version, if svn is installed. Avoid warnings if it isn't
FIND_PACKAGE(Subversion)
IF(Subversion_FOUND)
	EXECUTE_PROCESS(COMMAND "svnversion" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE revision OUTPUT_STRIP_TRAILING_WHITESPACE)
ELSE()
	SET(revision SVN_NOT_FOUND)
ENDIF()

# return the current date as day/month/year
MACRO(CURRENT_DATE result)
    IF(WIN32)
        EXECUTE_PROCESS(COMMAND "cmd" " /C date /T" OUTPUT_VARIABLE ${result})
        string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1/\\2/\\3" ${result} ${${result}})
    ELSEIF(UNIX)
        EXECUTE_PROCESS(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE ${result})
        string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1/\\2/\\3" ${result} ${${result}})
    ELSE(WIN32)
        MESSAGE(SEND_ERROR "date not implemented")
        SET(${result} 000000)
    ENDIF (WIN32)
ENDMACRO(CURRENT_DATE)

CURRENT_DATE(today)
ADD_DEFINITIONS(-D'SVN_VERSION="${revision}"' -D'BUILD_TIME="${today}"')
ADD_COMPILE_OPTIONS(-Wall)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

if(WIN32)
	set(LIBS opengl32)
elseif(UNIX AND NOT APPLE)
  	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
	find_package(OpenGL REQUIRED)
	add_definitions(${OPENGL_DEFINITIONS})
	set(LIBS dl pthread)
	set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
	INCLUDE_DIRECTORIES(/System/Library/Frameworks)
	FIND_LIBRARY(COCOA_LIBRARY Cocoa)
	FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
	FIND_LIBRARY(IOKit_LIBRARY IOKit)
	FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
	MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
	SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${CoreVideo_LIBRARY})
	SET(APPLE_LIBS ${APPLE_LIBS} ${ASSIMP_LIBRARY})
	set(LIBS ${LIBS} ${APPLE_LIBS})
else()
	set(LIBS )
endif(WIN32)

INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/extern )

# GLFW
add_subdirectory(extern/glfw)
include_directories(extern/glfw/include)

# GLAD
set(GLAD_GL ${PROJECT_SOURCE_DIR}/extern/glad/src/gl.c)
include_directories(extern/glad/include)

ADD_SUBDIRECTORY(src)
