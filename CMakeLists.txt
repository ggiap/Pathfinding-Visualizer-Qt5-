cmake_minimum_required(VERSION 3.17)
project(PathfindingVisualization)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
        ${CMAKE_MODULE_PATH})

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_MODULE_PATH}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
            "${CMAKE_MODULE_PATH}/conan.cmake")
endif()

include(${CMAKE_MODULE_PATH}/conan.cmake)
conan_add_remote(NAME bincrafters URL
        https://api.bintray.com/conan/bincrafters/public-conan)

conan_cmake_run(REQUIRES
        sfml/2.5.1@bincrafters/stable
        flac/1.3.2@bincrafters/stable
        BASIC_SETUP
        BUILD missing)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(PathfindingVisualization
        A_star.cpp
        A_star.h
        Button.cpp
        Button.h
        CMakeLists.txt
        Dijkstra.cpp
        Dijkstra.h
        GreedyBestFS.cpp
        GreedyBestFS.h
        Grid.cpp
        Grid.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Node.cpp
        Node.h
        Search.cpp
        Search.h
        SearchAlgorithm.cpp
        SearchAlgorithm.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CONAN_LIBS} Qt5::Widgets)

add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Font
        ${CMAKE_CURRENT_BINARY_DIR}/Font)

add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Grids_for_testing
        ${CMAKE_CURRENT_BINARY_DIR}/Grids_for_testing)
