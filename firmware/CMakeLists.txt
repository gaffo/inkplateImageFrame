cmake_minimum_required(VERSION 3.12) # version can be different

project(inkplatePictureFrame) #name of your project

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  inkplate_test
  imageRepo_test.cpp
)
target_link_libraries(
  inkplate_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(inkplate_test)

exec_program(build/inkplate_test)