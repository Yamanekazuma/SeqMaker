cmake_minimum_required(VERSION "3.20")
project("SeqMaker" VERSION 1.0.0 LANGUAGES CXX)

option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory("deps/zydis" EXCLUDE_FROM_ALL)

FILE(GLOB SRCS "./src/*.cpp")
FILE(GLOB INCLUDES "./include/*.hpp")
FILE(GLOB SHARED "./shared/*.h")
add_library("SeqMaker" SHARED ${SRCS} ${INCLUDES} ${SHARED})
add_library(SeqMaker::SeqMaker ALIAS SeqMaker)

target_compile_features("SeqMaker" PRIVATE cxx_std_20)

target_include_directories("SeqMaker" PRIVATE "./include/")

target_link_libraries("SeqMaker" PRIVATE "Zydis")

target_compile_definitions("SeqMaker" PRIVATE DLL_SEQ_MAKER)

target_compile_options("SeqMaker" PRIVATE
  "-Wall"
  "-Wextra"
  "-Werror"
  "-finput-charset=UTF-8"
  "-fexec-charset=CP932"
  "-municode"
)

target_include_directories (
	"SeqMaker" PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/shared>
	$<INSTALL_INTERFACE:include>
)

install (
	TARGETS "SeqMaker"
	EXPORT seqmaker-config
  RUNTIME DESTINATION lib
	ARCHIVE DESTINATION lib
)

install (
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shared/
	DESTINATION include
)

install (
	EXPORT seqmaker-config
	NAMESPACE SeqMaker::
	DESTINATION cmake
)
