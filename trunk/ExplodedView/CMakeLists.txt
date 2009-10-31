cmake_minimum_required(VERSION 2.6.4)

PROJECT(ExplodedView)

find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL)
find_package(OpenSceneGraph COMPONENTS osgViewer osgUtil osgGa osgDB osgText)
find_package(OpenGL)



#Dynamic or static linking
OPTION(STATIC_OPENSCENEGRAPH "Set to ON if OpenSceneGraph has been built staticly.  Use OFF for dynamic." ON)
SET(BUILD_SHARED_LIBS OFF)

#Includes
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
include_directories(${QT_INCLUDE_DIR})

#Defines
if(STATIC_OPENSCENEGRAPH)
	add_definitions(-DOSG_LIBRARY_STATIC)
endif(STATIC_OPENSCENEGRAPH)

#Plugins
set(PLUGIN_DIR ${OPENSCENEGRAPH_INCLUDE_DIRS}/../lib/osgPlugins-${OPENSCENEGRAPH_VERSION})
set(TARGET_ADDED_LIBRARIES_DEBUG ${PLUGIN_DIR}/osgdb_3dsd.lib)
set(TARGET_ADDED_LIBRARIES ${PLUGIN_DIR}/osgdb_3ds.lib)



#Libs
add_executable(ExplodedView ${CMAKE_CURRENT_SOURCE_DIR}/src/osgviewer.cpp)
target_link_libraries(ExplodedView ${OPENSCENEGRAPH_LIBRARIES} ${QT_LIBRARIES} ${OPENGL_LIBRARIES}
								   debug ${TARGET_ADDED_LIBRARIES_DEBUG}
								   optimized ${TARGET_ADDED_LIBRARIES})

#Debug
set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "add a postfix")


#Dirs
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/${CMAKE_SYSTEM}/${CMAKE_PLATFORM})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/${CMAKE_SYSTEM})



