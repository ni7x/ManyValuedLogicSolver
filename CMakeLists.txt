# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyProject VERSION 1.0)

# Set the C++ standard (optional but recommended)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the directory where the source files are located
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Collect all .cpp files in the src directory (not headers since they aren't compiled directly)
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

# Include header files in the project for IDE visibility (optional)
file(GLOB HEADERS "${SRC_DIR}/*.h")

# Add all header and source files to the executable target
add_executable(MyExecutable ${SOURCES} ${HEADERS}
        src/TruthTable.h
        src/f.cpp
        src/LogicalOperator.h)

# Optionally, add include directories if needed
target_include_directories(MyExecutable PRIVATE ${SRC_DIR})

# Optional: Link libraries (if you have any external dependencies)
# target_link_libraries(MyExecutable PRIVATE <libraries>)

# Include headers for better IDE support
source_group(TREE ${SRC_DIR} PREFIX "Header Files" FILES ${HEADERS})
source_group(TREE ${SRC_DIR} PREFIX "Source Files" FILES ${SOURCES})
