cmake_minimum_required(VERSION 3.14.4)
project(conflict-marker-check 
    VERSION 1.0.0
    DESCRIPTION "git conflict marker check command"
    LANGUAGES CXX)


################################################################################
# 初期設定
################################################################################

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

include_directories(${ORIGINAL_PACKAGE_H_DIR})

################################################################################
# main build
################################################################################

# add_executable(conflict-marker-check  ${MAIN_FILE} ${ORIGINAL_PACKAGE_C_FILES})


################################################################################
# testing build
################################################################################

add_executable(cmd-testing ${TEST_FILES} ${ORIGINAL_PACKAGE_C_FILES})

add_subdirectory(submodules/googletest)

target_include_directories(
	cmd-testing PUBLIC
	${INCLUDE_PATH}
)

target_link_libraries(
	cmd-testing
	gtest
)

add_test(NAME cmd-testing COMMAND cmd-testing)
