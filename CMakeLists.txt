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

set(SRC_DIR ./src)
set(TEST_DIR ./tests)

set(ORIGINAL_PACKAGE_PATH ${SRC_DIR}/selen)
set(ORIGINAL_PACKAGE_C_DIR ${ORIGINAL_PACKAGE_PATH})
set(ORIGINAL_PACKAGE_H_DIR ${ORIGINAL_PACKAGE_PATH})

file(GLOB MAIN_FILE ${SRC_DIR}/main.cpp)
file(GLOB TEST_FILES ${TEST_DIR}/*.*)
file(GLOB API_FILE ${SRC_DIR}/api.cpp)

file(GLOB ORIGINAL_PACKAGE_C_FILES ${ORIGINAL_PACKAGE_C_DIR}/*.cpp)

# macos
#file(GLOB LIB_FILES_OPENCV ${LIB_PATH}/libopencv_*.dylib)
#file(GLOB LIB_FILES_BOOST ${LIB_PATH}/libboost_*.dylib)
#file(GLOB LIB_FILE_TESSERACT ${LIB_PATH}/libtesseract.dylib)
#file(GLOB LIB_FILE_LEPTONICA ${LIB_PATH}/liblept.dylib)

# linux
file(GLOB LIB_FILES_OPENCV ${LIB_PATH}/libopencv_*.so)
file(GLOB LIB_FILES_BOOST ${LIB_PATH}/libboost_*.so)
file(GLOB LIB_FILE_TESSERACT ${LIB_PATH}/libtesseract.so)
file(GLOB LIB_FILE_LEPTONICA ${LIB_PATH}/liblept.so)


################################################################################
# package info
################################################################################

message(STATUS "package library info")

find_package(OpenCV REQUIRED) 
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

find_package(Boost REQUIRED)
message(STATUS "Boost library status:")
message(STATUS "    config: ${Boost_DIR}")
message(STATUS "    version: ${Boost_VERSION}")
message(STATUS "    libraries: ${Boost_LIBS}")
message(STATUS "    include path: ${Boost_INCLUDE_DIRS}")

################################################################################
# target common include header and libraries
################################################################################

# TODO: すべてのヘッダーやライブラリが取り込まれるのか、必要なものだけ取り込まれるのか確認する

include_directories(
	${ORIGINAL_PACKAGE_H_DIR}
	${INCLUDE_PATH}
)

################################################################################
# main build
################################################################################

set(BUILD_BINARY_MAIN ${PROJECT_NAME})

find_package(OpenCV REQUIRED)

add_executable(
	${BUILD_BINARY_MAIN}
	${MAIN_FILE} 
	${ORIGINAL_PACKAGE_C_FILES} 
)

# -I option
target_link_libraries(
	${BUILD_BINARY_MAIN}
	${LIB_FILES_BOOST}
	${OpenCV_LIBS}
	${LIB_FILE_TESSERACT}
	${LIB_FILE_LEPTONICA}
)


################################################################################
# testing build
################################################################################

set(BUILD_BINARY_TESTING ${PROJECT_NAME}-testing)

add_executable(
	${BUILD_BINARY_TESTING}
	${TEST_FILES} 
	${ORIGINAL_PACKAGE_C_FILES}
)

add_subdirectory(submodules/googletest)

# -I option
target_link_libraries(
	${BUILD_BINARY_TESTING}
	gtest
	${LIB_FILES_BOOST}
	${OpenCV_LIBS}
	${LIB_FILE_TESSERACT}
	${LIB_FILE_LEPTONICA}
)

add_test(NAME ${BUILD_BINARY_TESTING} COMMAND ${BUILD_BINARY_TESTING})


################################################################################
# lambda package build
################################################################################

set(BUILD_BINARY_API ${PROJECT_NAME}-api)

find_package(aws-lambda-runtime REQUIRED)
find_package(AWSSDK COMPONENTS core)

add_executable(${BUILD_BINARY_API} ${API_FILE})
target_link_libraries(
	${BUILD_BINARY_API} 
	PUBLIC 
	AWS::aws-lambda-runtime 
	${AWSSDK_LINK_LIBRARIES}
)

aws_lambda_package_target(${BUILD_BINARY_API})
