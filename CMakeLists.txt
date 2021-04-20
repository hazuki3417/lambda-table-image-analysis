cmake_minimum_required(VERSION 3.20.1)
project(lambda-table-image-analysis 
    VERSION 1.0.0
    DESCRIPTION "git conflict marker check command"
    LANGUAGES CXX)

################################################################################
# 初期設定
################################################################################

set (CMAKE_CXX_STANDARD 17)

set(INCLUDE_PATH /usr/local/include)
set(LIB_PATH /usr/local/lib)

set(SRC_DIR src)
set(TEST_DIR tests)

set(ORIGINAL_PACKAGE_PATH ${SRC_DIR}/selen)
set(ORIGINAL_PACKAGE_C_DIR ${ORIGINAL_PACKAGE_PATH})
set(ORIGINAL_PACKAGE_H_DIR ${ORIGINAL_PACKAGE_PATH})

file(GLOB MAIN_FILE ${SRC_DIR}/main.cpp)
file(GLOB TEST_FILES ${TEST_DIR}/*.*)

file(GLOB ORIGINAL_PACKAGE_C_FILES ${ORIGINAL_PACKAGE_C_DIR}/*.cpp)

# macos
#file(GLOB LIB_FILES_OPENCV ${LIB_PATH}/libopencv_*.dylib)
#file(GLOB LIB_FILES_BOOST ${LIB_PATH}/libboost_*.dylib)

# linux
file(GLOB LIB_FILES_OPENCV ${LIB_PATH}/libopencv_*.so)
file(GLOB LIB_FILES_BOOST ${LIB_PATH}/libboost_*.so)


################################################################################
# target common include header and libraries
################################################################################

# TODO: すべてのヘッダーやライブラリが取り込まれるのか、必要なものだけ取り込まれるのか確認する

include_directories(
	${ORIGINAL_PACKAGE_H_DIR}
)

# NOTE: 必要なライブラリは随時下記に追加していく

#link_libraries(
#	${LIB_FILES_BOOST}
#	${LIB_FILES_OPENCV}
#	${LIB_PATH}/libtesseract.so
#	${LIB_PATH}/libleptonica.so
#)

################################################################################
# main build
################################################################################

add_executable(
	table-image-analysis 
	${MAIN_FILE} 
	${ORIGINAL_PACKAGE_C_FILES} 
)

target_link_libraries(
	table-image-analysis
	${LIB_FILES_BOOST}
	${LIB_FILES_OPENCV}
	${LIB_PATH}/libtesseract.so
	${LIB_PATH}/libleptonica.so
)


################################################################################
# testing build
################################################################################

add_executable(
	table-image-analysis-testing 
	${TEST_FILES} 
	${ORIGINAL_PACKAGE_C_FILES}
)

add_subdirectory(submodules/googletest)

target_link_libraries(
	table-image-analysis-testing
	gtest
	${LIB_FILES_BOOST}
	${LIB_FILES_OPENCV}
	${LIB_PATH}/libtesseract.so
	${LIB_PATH}/libleptonica.so
)

add_test(NAME table-image-analysis-testing COMMAND table-image-analysis-testing)
