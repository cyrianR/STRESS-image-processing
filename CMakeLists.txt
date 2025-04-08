cmake_minimum_required(VERSION 3.10.0)
project(STRESS VERSION 0.1.0 LANGUAGES C CXX)

include(GNUInstallDirs)

# set the output path for the generated files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR} )

find_package(OpenCV 4.0 REQUIRED)
message(STATUS "OpenCV found, version ${OpenCV_VERSION}")

add_subdirectory(src)


