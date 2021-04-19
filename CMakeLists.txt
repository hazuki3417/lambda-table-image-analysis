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
set(C_DIR ${ORIGINAL_PACKAGE_PATH})
set(H_DIR ${ORIGINAL_PACKAGE_PATH})

file(GLOB MAIN_SOURCE ${SRC_DIR}/main.cpp)
file(GLOB LIB_SOURCE ${C_DIR}/*.cpp)
file(GLOB TEST_SOURCE ${TEST_DIR}/*.*)

include_directories(${H_DIR})

################################################################################
# main build
################################################################################

# add_executable(conflict-marker-check  ${MAIN_SOURCE} ${LIB_SOURCE})


################################################################################
# testing build
################################################################################

add_executable(cmd-testing ${TEST_SOURCE} ${LIB_SOURCE})

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
