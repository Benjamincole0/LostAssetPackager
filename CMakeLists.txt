cmake_minimum_required(VERSION 3.16)
project(LAPRW LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Root Directories ----
set(COMMON_DIR   ${CMAKE_SOURCE_DIR}/Common/src)
set(READER_DIR   ${CMAKE_SOURCE_DIR}/LAPReader/src)
set(WRITER_DIR   ${CMAKE_SOURCE_DIR}/LAPWriter/src)

# ---- Common Library ----
file(GLOB_RECURSE COMMON_SOURCES "${COMMON_DIR}/*.cpp" "${COMMON_DIR}/*.h")
add_library(LAPCommon ${COMMON_SOURCES})
target_include_directories(LAPCommon PUBLIC ${COMMON_DIR})   # exported to dependents

# ---- LAPReader ----
file(GLOB_RECURSE READER_SOURCES "${READER_DIR}/*.cpp" "${READER_DIR}/*.h")
add_executable(LAPReader ${READER_SOURCES})
target_include_directories(LAPReader PRIVATE ${READER_DIR})
target_link_libraries(LAPReader PUBLIC LAPCommon)            # inherit Common includes
set_target_properties(LAPReader PROPERTIES OUTPUT_NAME lapr)

# ---- LAPWriter ----
file(GLOB_RECURSE WRITER_SOURCES "${WRITER_DIR}/*.cpp" "${WRITER_DIR}/*.h")
add_executable(LAPWriter ${WRITER_SOURCES})
target_include_directories(LAPWriter PRIVATE ${WRITER_DIR})
target_link_libraries(LAPWriter PUBLIC LAPCommon)            # inherit Common includes
set_target_properties(LAPWriter PROPERTIES OUTPUT_NAME lapw)
