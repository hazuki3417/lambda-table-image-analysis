cmake_minimum_required(VERSION 3.20.0)
project(lambda-table-image-analysis 
    VERSION 1.0.0
    DESCRIPTION "git conflict marker check command"
    LANGUAGES CXX)

################################################################################
# 初期設定
################################################################################

set (CMAKE_CXX_STANDARD 17)

set(INCLUDE_PATH /usr/include)
set(LIB_PATH /usr/lib/x86_64-linux-gnu)

set(SRC_DIR src)
set(TEST_DIR tests)

set(ORIGINAL_PACKAGE_PATH ${SRC_DIR}/selen)
set(ORIGINAL_PACKAGE_C_DIR ${ORIGINAL_PACKAGE_PATH})
set(ORIGINAL_PACKAGE_H_DIR ${ORIGINAL_PACKAGE_PATH})

file(GLOB MAIN_FILE ${SRC_DIR}/main.cpp)
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)
file(GLOB API_FILE ${SRC_DIR}/api.cpp)

file(GLOB ORIGINAL_PACKAGE_C_FILES ${ORIGINAL_PACKAGE_C_DIR}/*.cpp)

file(GLOB LIB_FILES_BOOST ${LIB_PATH}/libboost_*.so)
file(GLOB LIB_FILE_LEPTONICA ${LIB_PATH}/liblept.so)
file(GLOB LIB_FILES_OPENCV ${LIB_PATH}/libopencv_*.so)
file(GLOB LIB_FILE_TESSERACT ${LIB_PATH}/libtesseract.so)


# debug: path check
#message(STATUS "cpp files:")
#message(STATUS "    main : ${MAIN_FILE}")
#message(STATUS "    selen: ${ORIGINAL_PACKAGE_C_FILES}")
#message(STATUS "    test : ${TEST_FILES}")
#message(STATUS "    api  : ${API_FILE}")
#
#message(STATUS "header directories:")
#message(STATUS "    lib  : ${INCLUDE_PATH}")
#message(STATUS "    boost : ${LIB_FILES_BOOST}")
#message(STATUS "    leptonica : ${LIB_FILE_LEPTONICA}")
#message(STATUS "    opencv : ${LIB_FILES_OPENCV}")
#message(STATUS "    tesseract : ${LIB_FILE_TESSERACT}")


################################################################################
# target common include header and libraries
################################################################################

# TODO: すべてのヘッダーやライブラリが取り込まれるのか、必要なものだけ取り込まれるのか確認する

include_directories(
	${INCLUDE_PATH}
	${ORIGINAL_PACKAGE_H_DIR}
	/usr/include/opencv4
)

################################################################################
# main build
################################################################################

set(BUILD_BINARY_MAIN ${PROJECT_NAME})

add_executable(
	${BUILD_BINARY_MAIN}
	${MAIN_FILE} 
	${ORIGINAL_PACKAGE_C_FILES} 
)

# -I option
target_link_libraries(
	${BUILD_BINARY_MAIN}
	${LIB_FILES_BOOST}
	${LIB_FILE_LEPTONICA}
	${LIB_FILES_OPENCV}
	${LIB_FILE_TESSERACT}
	/lib
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
	${LIB_FILE_LEPTONICA}
	${LIB_FILES_OPENCV}
	${LIB_FILE_TESSERACT}
)

add_test(NAME ${BUILD_BINARY_TESTING} COMMAND ${BUILD_BINARY_TESTING})


################################################################################
# lambda package build
################################################################################

set(BUILD_BINARY_API ${PROJECT_NAME}-api)

find_package(aws-lambda-runtime REQUIRED)
find_package(AWSSDK COMPONENTS core)

add_executable(
	${BUILD_BINARY_API}
	${API_FILE}
	${ORIGINAL_PACKAGE_C_FILES} 
)

target_link_libraries(
	${BUILD_BINARY_API} 
	PUBLIC 
	AWS::aws-lambda-runtime 
	${AWSSDK_LINK_LIBRARIES}
	${LIB_FILES_BOOST}
	${LIB_FILE_LEPTONICA}
	${LIB_FILES_OPENCV}
	${LIB_FILE_TESSERACT}
)

aws_lambda_package_target(${BUILD_BINARY_API})
