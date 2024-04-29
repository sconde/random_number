cmake_minimum_required(VERSION 3.10)
project(RNG)

# Enable C++11
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Set the binary output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(USE_CLANG_TIDY "Enable Clang-Tidy" ON)

if (USE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if (CLANG_TIDY_EXE)
        message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
        add_compile_options(-Wunused-variable
        -Wunused-parameter
        -Wunused-function
        -Wunused-value)
    else()
        message(FATAL_ERROR "clang-tidy not found.")
    endif()
endif()

# Add clang-format
find_program(CLANG_FORMAT_EXE NAMES "clang-format")
if (CLANG_FORMAT_EXE)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_EXE}")
    set(CLANG_FORMAT_COMMAND "${CLANG_FORMAT_EXE}" -i -style=file)
    file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
    add_custom_target(clang-format COMMAND ${CLANG_FORMAT_COMMAND} ${ALL_SOURCE_FILES})
else()
    message(FATAL_ERROR "clang-format not found.")
endif()

# Check build type
#if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
#    add_compile_options(-fno-omit-frame-pointer -mno-omit-leaf-frame-pointer)
#endif()


# Find OpenMP
find_package(OpenMP REQUIRED)
message("OPENMP FOUND")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# Add your source files
add_executable(rng.x main.cpp rng.cpp
        main.cpp)
target_link_libraries(rng.x PRIVATE OpenMP::OpenMP_CXX)
# Add the -pg flag to enable profiling with gprof
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Enable profiling with LLVM's built-in tools
        target_compile_options(rng.x PRIVATE -fprofile-instr-generate)
        target_link_options(rng.x PRIVATE -fprofile-instr-generate)
    else()
        target_compile_options(rng.x PRIVATE -pg)
        target_link_options(rng.x PRIVATE -pg)
endif()

# Add clang-tidy checks
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
