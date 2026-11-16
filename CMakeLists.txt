cmake_minimum_required(VERSION 3.16)

project(AdaptiveCacheStorageEngine LANGUAGES CXX)

add_executable(adaptive_cache src/main.cpp)
target_include_directories(adaptive_cache PRIVATE include)
target_compile_features(adaptive_cache PRIVATE cxx_std_17)
