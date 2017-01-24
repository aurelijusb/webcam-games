cmake_minimum_required(VERSION 3.5)
project(CvGames)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/Objects3D/Material.cpp
        src/Objects3D/Object3D.cpp
        src/Objects3D/Objects3D.cpp
        src/Objects3D/Objects3D.h
        src/Output/DebugView.cpp
        src/Output/DebugView.h
        src/Output/Bubble.cpp
        src/Output/Bubble.h
        src/Output/BubbleShot.cpp
        src/Output/BubbleShot.h
        src/Output/Output3D.cpp
        src/Output/Output3D.h
        src/Output/WebCamEffect.cpp
        src/Output/WebCamEffect.h
        src/Output/WebCamVJ.cpp
        src/Output/WebCamVJ.h
        src/Tracker/TrackerBase.cpp
        src/Tracker/TrackerBase.h
        src/Tracker/TrackerDifference.cpp
        src/Tracker/TrackerDifference.h
        src/Tracker/TrackerMotion.cpp
        src/Tracker/TrackerMotion.h
        src/main.cpp
        README.md)

add_executable(CvGames ${SOURCE_FILES})

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(CvGames ${OpenCV_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(CvGames ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
