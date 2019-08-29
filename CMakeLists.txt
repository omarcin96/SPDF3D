# cmake minimal version required to work.
cmake_minimum_required(VERSION 3.14.4)

include(cmake-utils/libraries.cmake)

# Project name.
project(spdf3d)

if(NOT CMAKE_CONFIGURATION_TYPES)
	message("CMAKE_CONFIGURATION_TYPES not defined. Try CMAKE_BUILD_TYPE flag")
	if(DEFINED CMAKE_BUILD_TYPE)
		message("Set: " ${CMAKE_BUILD_TYPE})
		set(CMAKE_CONFIGURATION_TYPES Debug)
	else()
		message("Set: " Release)
		set(CMAKE_CONFIGURATION_TYPES Release)
	endif()
endif()

message("Configuration types: " ${CMAKE_CONFIGURATION_TYPES})

# Globals:
set(GLM_ROOT_DIR "C:/glm/" CACHE PATH "GLM library path")
set(HARU_INCLUDES_DIR "C:/libharu-build/include" CACHE PATH "libharu includes dir")
set(HARU_DBG_LIB_DIR "C:/libharu-build/src/Debug" CACHE PATH "libharu libraries debug dir")
set(HARU_OPT_LIB_DIR "C:/libharu-build/src/Release" CACHE PATH "libharu libraries release dir")
set(PRC_INCLUDES_DIR "C:/prc-build/src/asymptote" CACHE PATH "libprc includes dir")
set(PRC_DBG_LIBS_DIR "C:/prc-build/lib/Debug" CACHE PATH "libprc libraries debug dir")
set(PRC_OPT_LIBS_DIR "C:/prc-build/lib/Release" CACHE PATH "libprc libraries release dir")
set(OCS_INCLUDES_DIR "C:/ocs/inc" CACHE PATH "libprc includes dir")
set(OCS_DBG_LIBS_DIR "C:/ocs/win64/vc14/libd" CACHE PATH "libprc libraries debug dir")
set(OCS_OPT_LIBS_DIR "C:/ocs/win64/vc14/lib" CACHE PATH "libprc libraries release dir")
set(ZLIB_DBG_LIBS_DIR "C:/OpenCASCADE-7.2.0-vc10-64/zlib-1.2.8-vc14-64/lib" CACHE PATH "zlib libraries debug dir")
set(ZLIB_OPT_LIBS_DIR "C:/OpenCASCADE-7.2.0-vc10-64/zlib-1.2.8-vc14-64/lib" CACHE PATH "zlib libraries debug dir")

# Project version.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 0)
set (VERSION_PATCH 1)

#find glm paths.
find_path(GLM_INCLUDES NAMES glm/glm.hpp PATHS ${GLM_ROOT_DIR})

# find libharu paths.
find_path(HARU_INCLUDES NAMES hpdf_config.h PATHS ${HARU_INCLUDES_DIR})

set(HARU_LIB_DBG "HARU_LIB_DBG-NOTFOUND")
set(HARU_LIB_OPT "HARU_LIB_OPT-NOTFOUND")

if(CMAKE_CONFIGURATION_TYPES MATCHES Debug)
	message(STATUS "Trying find libharu debug version")
	find_library(HARU_LIB_DBG NAMES libhpdfsd PATHS ${HARU_DBG_LIB_DIR})
endif()

if(CMAKE_CONFIGURATION_TYPES MATCHES Release)
	message(STATUS "Trying find libharu release version")
	find_library(HARU_LIB_OPT NAMES libhpdfs PATHS ${HARU_OPT_LIB_DIR})		
endif()
									
# find libprc.
find_path(PRC_INCLUDES NAMES PRC.h PATHS ${PRC_INCLUDES_DIR})
	
# set variables:
set(PRC_DBG_LIBS_LIST "asymptoted;libPRCd")
set(PRC_DBG_LIBS "")
set(PRC_DBG_PREF "PRC_DBG")

set(PRC_OPT_LIBS_LIST "asymptote;libPRC")
set(PRC_OPT_LIBS "")
set(PRC_OPT_PREF "PRC_OPT")

if(CMAKE_CONFIGURATION_TYPES MATCHES Debug)
	FIND_LIBS(PRC_DBG_PREF PRC_DBG_LIBS_LIST PRC_DBG_LIBS_DIR PRC_DBG_LIBS)
	message("Status:" ${PRC_DBG_LIBS})
endif()

if(CMAKE_CONFIGURATION_TYPES MATCHES Release)
	FIND_LIBS(PRC_OPT_PREF PRC_OPT_LIBS_LIST PRC_OPT_LIBS_DIR PRC_OPT_LIBS)
	message("Status:" ${PRC_OPT_LIBS})
endif()

# find opencascade
find_path(OCS_INCLUDES NAMES XCAFDoc.hxx PATHS ${OCS_INCLUDES_DIR})
	
# Set variables:
set(OCS_DBG_LIBS_LIST "TKBin.lib;TKBinL.lib;TKBinTObj.lib;TKBinXCAF.lib;TKBO.lib;TKBool.lib;TKBRep.lib;TKCAF.lib;TKCDF.lib;TKDCAF.lib;TKDraw.lib;TKernel.lib;TKFeat.lib;TKFillet.lib;TKG2d.lib;TKG3d.lib;TKGeomAlgo.lib;TKGeomBase.lib;TKHLR.lib;TKIGES.lib;TKLCAF.lib;TKMath.lib;TKMesh.lib;TKMeshVS.lib;TKOffset.lib;TKOpenGl.lib;TKPrim.lib;TKQADraw.lib;TKService.lib;TKShHealing.lib;TKStd.lib;TKStdL.lib;TKSTEP.lib;TKSTEP209.lib;TKSTEPAttr.lib;TKSTEPBase.lib;TKSTL.lib;TKTObj.lib;TKTObjDRAW.lib;TKTopAlgo.lib;TKTopTest.lib;TKV3d.lib;TKVCAF.lib;TKViewerTest.lib;TKVRML.lib;TKXCAF.lib;TKXDEDRAW.lib;TKXDEIGES.lib;TKXDESTEP.lib;TKXMesh.lib;TKXml.lib;TKXmlL.lib;TKXmlTObj.lib;TKXmlXCAF.lib;TKXSBase.lib;TKXSDRAW.lib")
set(OCS_DBG_LIBS "")
set(OCS_DBG_PREF "OCS_DBG")

set(OCS_OPT_LIBS_LIST "TKBin.lib;TKBinL.lib;TKBinTObj.lib;TKBinXCAF.lib;TKBO.lib;TKBool.lib;TKBRep.lib;TKCAF.lib;TKCDF.lib;TKDCAF.lib;TKDraw.lib;TKernel.lib;TKFeat.lib;TKFillet.lib;TKG2d.lib;TKG3d.lib;TKGeomAlgo.lib;TKGeomBase.lib;TKHLR.lib;TKIGES.lib;TKLCAF.lib;TKMath.lib;TKMesh.lib;TKMeshVS.lib;TKOffset.lib;TKOpenGl.lib;TKPrim.lib;TKQADraw.lib;TKService.lib;TKShHealing.lib;TKStd.lib;TKStdL.lib;TKSTEP.lib;TKSTEP209.lib;TKSTEPAttr.lib;TKSTEPBase.lib;TKSTL.lib;TKTObj.lib;TKTObjDRAW.lib;TKTopAlgo.lib;TKTopTest.lib;TKV3d.lib;TKVCAF.lib;TKViewerTest.lib;TKVRML.lib;TKXCAF.lib;TKXDEDRAW.lib;TKXDEIGES.lib;TKXDESTEP.lib;TKXMesh.lib;TKXml.lib;TKXmlL.lib;TKXmlTObj.lib;TKXmlXCAF.lib;TKXSBase.lib;TKXSDRAW.lib")
set(OCS_OPT_LIBS "")
set(OCS_OPT_PREF "OCS_OPT")

if(CMAKE_CONFIGURATION_TYPES MATCHES Debug)
	FIND_LIBS(OCS_DBG_PREF OCS_DBG_LIBS_LIST OCS_DBG_LIBS_DIR OCS_DBG_LIBS)
	message("Status:" ${OCS_DBG_LIBS})
endif()

if(CMAKE_CONFIGURATION_TYPES MATCHES Release)
	FIND_LIBS(OCS_OPT_PREF OCS_OPT_LIBS_LIST OCS_OPT_LIBS_DIR OCS_OPT_LIBS)
	message("Status:" ${OCS_OPT_LIBS})
endif()

#find zlib library.
set(ZLIB_DBG_LIB "ZLIB_DBG_LIB-NOTFOUND")
set(ZLIB_OPT_LIB "ZLIB_OPT_LIB-NOTFOUND")


if(CMAKE_CONFIGURATION_TYPES MATCHES Debug)
	message(STATUS "Trying find zlib debug version")
	find_library(ZLIB_DBG_LIB NAMES zlibstatic PATHS ${ZLIB_DBG_LIBS_DIR})	
endif()

if(CMAKE_CONFIGURATION_TYPES MATCHES Release)
	message(STATUS "Trying find zlib release version")
	find_library(ZLIB_OPT_LIB NAMES zlibstatic PATHS ${ZLIB_OPT_LIBS_DIR})	
endif()

# add opencascade binary.

# Pliki naglowkowe:
include_directories(inc)
include_directories(${OCS_INCLUDES})
include_directories(${HARU_INCLUDES})
include_directories(${PRC_INCLUDES})
include_directories(${GLM_INCLUDES})

# Pliki zrodlowe:
file(GLOB SOURCES "src/*.cpp")

# Compile
add_executable(${PROJECT_NAME} ${SOURCES})

# Linking
if(CMAKE_CONFIGURATION_TYPES MATCHES Debug)
	message(STATUS "Link to debug target")
	target_link_libraries(${PROJECT_NAME} debug ${OCS_DBG_LIBS} ${HARU_LIB_DBG} ${PRC_DBG_LIBS} ${ZLIB_DBG_LIB})
endif()

if(CMAKE_CONFIGURATION_TYPES MATCHES Release)
	message(STATUS "Link to release target")
	target_link_libraries(${PROJECT_NAME} optimized ${OCS_OPT_LIBS} ${HARU_OPT_DBG} ${PRC_OPT_LIBS} ${ZLIB_OPT_LIB})
endif()