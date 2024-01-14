cmake_minimum_required(VERSION 3.19)

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

message("Compiler Version: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}") 

find_package( Boost COMPONENTS unit_test_framework REQUIRED )

### UNIT TEST
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} unit_test/*.cpp)
foreach(testSrc ${TEST_SRCS})
  # Extract the filename without an extension (NAME_WE)
  get_filename_component(testName ${testSrc} NAME_WE)
  # Add compile target
  add_executable(${testName} ${testSrc})
  # Require the standard
  target_compile_features(${testName} PRIVATE cxx_std_23)
  # Ignore warnings about subtle ABI change
  target_compile_options(${testName} PUBLIC "-Wno-psabi")
  # Link to dependencies
  target_link_libraries(${testName} project::project Boost::unit_test_framework)
  # Specifies include directories to use when compiling the target
  target_include_directories(
    ${testName} PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
  # Add it to test execution
  add_test(
    NAME ${testName}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test
    COMMAND ${testName} )
endforeach(testSrc)
