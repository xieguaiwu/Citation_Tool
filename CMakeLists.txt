cmake_minimum_required(VERSION 3.10)
project(CitationTool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" HAS_CXX17)
if(HAS_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
    #On Windows
    set(PLATFORM_LIBS user32)
else()
    #On Linux
    set(PLATFORM_LIBS stdc++fs)
endif()

include_directories(includes)

add_executable(CitationTool WCT.cpp LCT.cpp)

target_sources(CitationTool PRIVATE
    includes/includes.h
    includes/basicfunc.h
    includes/files.h
)

target_link_libraries(CitationTool PRIVATE ${PLATFORM_LIBS})

set_target_properties(CitationTool PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS CitationTool
    RUNTIME DESTINATION bin
)