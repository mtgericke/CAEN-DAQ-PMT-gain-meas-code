cmake_minimum_required(VERSION 3.8...3.18)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(EMMonitor)

LIST( APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake/Modules
)

set(CMAKE_CXX_FLAGS "-g -Wall -pg -Wno-shadow -Wno-pedantic -Wno-unused-parameter -Wno-overloaded-virtual -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable")

find_package(ROOT REQUIRED)

set(RDATA_LIBRARIES "${PROJECT_SOURCE_DIR}/lib/build/libRData.a")
set(RDATA_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/lib/include")
set(CAEN_LIBRARY "/usr/lib")
set(CAEN_INCLUDE_DIR "/usr/include")

# ROOT version 6 required
if(ROOT_FOUND)
  message(STATUS "ROOT include: ${ROOT_INCLUDE_DIRS}")
  STRING(REGEX MATCH "6.*" VERSION6MATCH ${ROOT_VERSION})
  if(NOT VERSION6MATCH)
    message(FATAL_ERROR "EMMonitor requires ROOT 6")
  endif()
endif()

include(${ROOT_USE_FILE}) 

if(EXISTS "${ROOT_DIR}/modules/RootNewMacros.cmake")
  include("${ROOT_DIR}/modules/RootNewMacros.cmake")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${ROOT_INCLUDE_DIRS}
		    ${RDATA_INCLUDE_DIRS}
		    ${CAEN_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

message(STATUS "RData Library:  ${RDATA_LIBRARIES}")
message(STATUS "ROOT Library:  ${ROOT_LIBRARIES}")

ROOT_GENERATE_DICTIONARY(EMMonitorDict ${PROJECT_SOURCE_DIR}/include/EMMonitor.h LINKDEF ${PROJECT_SOURCE_DIR}/include/EMMonitorDictLinkDef.h)


add_executable(EMMonitor ${PROJECT_SOURCE_DIR}/source/EMMonitor.cxx ${sources} ${headers} EMMonitorDict.cxx)
add_library(EMMonitorDictionaries SHARED ${sources} ${headers} EMMonitorDict.cxx)
target_link_libraries(EMMonitorDictionaries ${ROOT_LIBRARIES})

target_link_libraries(EMMonitor -lCAENVME -lm EMMonitorDictionaries ${RDATA_LIBRARIES} ${ROOT_LIBRARIES}  ROOT::Gui ROOT::Core ROOT::Imt ROOT::RIO ROOT::Net ROOT::Hist ROOT::Graf ROOT::Graf3d ROOT::Gpad ROOT::ROOTVecOps ROOT::Tree ROOT::TreePlayer ROOT::Rint ROOT::Postscript ROOT::Matrix ROOT::Physics ROOT::MathCore ROOT::Thread ROOT::MultiProc ROOT::ROOTDataFrame -pthread)
