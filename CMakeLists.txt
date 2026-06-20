cmake_minimum_required(VERSION 3.14)
project(HybridSearchPipeline CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(hybrid_search_app
    src/main.cpp
    src/BM25Engine.cpp
    src/HNSW.cpp
    src/RRFFusion.cpp
)

target_include_directories(hybrid_search_app PRIVATE include)
