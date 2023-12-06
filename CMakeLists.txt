cmake_minimum_required(VERSION "3.15")
project("SeqMaker")

option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory("deps/zydis")

FILE(GLOB SRCS "./src/*")
FILE(GLOB INCLUDES "./include/*")
add_executable("SeqMaker" ${SRCS} ${INCLUDES})

target_include_directories("SeqMaker" PRIVATE "./include/")

target_link_libraries("SeqMaker" PRIVATE "Zydis")

