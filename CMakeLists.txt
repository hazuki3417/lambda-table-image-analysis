cmake_minimum_required(VERSION 3.20.0)
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
file(GLOB LIB_FILES_OPENCV ${LIB_PATH}/libopencv_*.dylib)
file(GLOB LIB_FILES_BOOST ${LIB_PATH}/libboost_*.dylib)
file(GLOB LIB_FILE_TESSERACT ${LIB_PATH}/libtesseract.dylib)
file(GLOB LIB_FILE_LEPTONICA ${LIB_PATH}/liblept.dylib)

# linux
#file(GLOB LIB_FILES_OPENCV ${LIB_PATH}/libopencv_*.so)
#file(GLOB LIB_FILES_BOOST ${LIB_PATH}/libboost_*.so)
#file(GLOB LIB_FILE_TESSERACT ${LIB_PATH}/libtesseract.so)
#file(GLOB LIB_FILE_LEPTONICA ${LIB_PATH}/liblept.so)


################################################################################
# target common include header and libraries
################################################################################

# TODO: すべてのヘッダーやライブラリが取り込まれるのか、必要なものだけ取り込まれるのか確認する

include_directories(
	${ORIGINAL_PACKAGE_H_DIR}
	${INCLUDE_PATH}
)

# NOTE: 必要なライブラリは随時下記に追加していく

#link_libraries(
#	${LIB_FILES_BOOST}
#	${LIB_FILES_OPENCV}
#	${LIB_FILE_TESSERACT}
#	${LIB_FILE_LEPTONICA}
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
	${LIB_FILE_TESSERACT}
	${LIB_FILE_LEPTONICA}
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
	${LIB_FILE_TESSERACT}
	${LIB_FILE_LEPTONICA}
)

add_test(NAME table-image-analysis-testing COMMAND table-image-analysis-testing)


################################################################################
# lambda package build
################################################################################

#cmake_minimum_required(VERSION 3.5)
#set(CMAKE_CXX_STANDARD 11)
#
#project(api LANGUAGES CXX)
#
#find_package(aws-lambda-runtime REQUIRED)
#find_package(AWSSDK COMPONENTS core)
#
#add_executable(${PROJECT_NAME} "../src/main.cpp")
#target_link_libraries(${PROJECT_NAME} PUBLIC AWS::aws-lambda-runtime ${AWSSDK_LINK_LIBRARIES})
#
#aws_lambda_package_target(${PROJECT_NAME})
