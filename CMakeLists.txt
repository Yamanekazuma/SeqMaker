cmake_minimum_required(VERSION "3.15")
project("SeqMaker")

option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory("deps/zydis")

FILE(GLOB SRCS "./src/*.cpp")
FILE(GLOB INCLUDES "./include/*.hpp")
add_executable("SeqMaker" ${SRCS} ${INCLUDES})

target_compile_features("SeqMaker" PRIVATE cxx_std_20)

target_include_directories("SeqMaker" PRIVATE "./include/")

target_link_libraries("SeqMaker" PRIVATE "Zydis")

target_compile_options("SeqMaker" PRIVATE
  "-Wall"
  "-Wextra"
  "-Werror"
  "-finput-charset=UTF-8"
  "-fexec-charset=CP932"
  "-municode"
)
