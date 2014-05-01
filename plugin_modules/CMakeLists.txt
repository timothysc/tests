# I should really cmake-i-fy all my tests

project(plugin_tests)
cmake_minimum_required(VERSION 2.6)
set(PACKAGE "MyPluginzWhatWhat")

file (GLOB CommonSrcs MyFakeInterface.h)
file (GLOB Plugin1Srcs TestClass1.cpp)

add_library(plugin1 SHARED ${CommonSrcs};${Plugin1Srcs})

file (GLOB Plugin2Srcs TestClass2.cpp)
add_library(plugin2 SHARED ${CommonSrcs};${Plugin2Srcs})

file (GLOB MainSrcs main.cpp MesosModule.h DynamicLibrary.h)

add_executable (main_test ${CommonSrcs};${MainSrcs})
target_link_libraries (main_test dl;boost_filesystem;boost_system)

