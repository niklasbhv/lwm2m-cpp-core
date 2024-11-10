cmake_minimum_required(VERSION 3.22.1)
project(lwm2m_cpp_edge VERSION 0.1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally, enable position-independent code
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Include CPM
include(cmake/CPM.cmake)

# Install additional packages using CPM
CPMAddPackage("gh:zeux/pugixml@1.14")

# Include the "include" directory for public headers
include_directories(include)

# Add the library target from source files
add_library(${PROJECT_NAME}
        src/lwm2m_cpp_edge.cpp
        src/sdf_to_lwm2m.cpp
        src/lwm2m_to_sdf.cpp)

# Optionally, specify whether to build as a shared or static library
# set(BUILD_SHARED_LIBS ON)

# Specify include directories for the library's public headers
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>  # For local development
        $<INSTALL_INTERFACE:include>                      # For installation
)

# Link additional libraries
target_link_libraries(${PROJECT_NAME} PRIVATE pugixml::pugixml)

# Set properties (versioning and compatibility)
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
#        PUBLIC_HEADER "include/my_library.h"  # Set the main public header, if applicable
)

# Optionally, define compile options
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

# Install library target
install(TARGETS ${PROJECT_NAME}
        EXPORT lwm2m_cpp_edge_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}    # For shared libraries (.so/.dylib)
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}    # For static libraries (.a)
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}    # For executables on Windows (.dll)
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lwm2m_cpp_edge
)

# Install the pugixml target
install(TARGETS pugixml
        EXPORT lwm2m_cpp_edge_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pugixml
)

# Install the export configuration for other CMake projects to use this library
install(EXPORT lwm2m_cpp_edge_targets
        FILE lwm2m_cpp_edge_targets.cmake
        NAMESPACE lwm2m::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lwm2m_cpp_edge
)

# Install public headers
install(DIRECTORY include/lwm2m_cpp_edge
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
