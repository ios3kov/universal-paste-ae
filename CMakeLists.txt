cmake_minimum_required(VERSION 3.20)

project(UniversalPasteCore LANGUAGES CXX)

option(UNIVERSAL_PASTE_BUILD_TESTS "Build Universal Paste core tests" ON)

add_library(universal_paste_core STATIC
  src/core/ColorParser.cpp
  src/core/ClipboardClassifier.cpp
)

target_include_directories(universal_paste_core PUBLIC include)
target_compile_features(universal_paste_core PUBLIC cxx_std_17)

if(MSVC)
  target_compile_options(universal_paste_core PRIVATE /W4 /permissive-)
else()
  target_compile_options(universal_paste_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(UNIVERSAL_PASTE_BUILD_TESTS)
  enable_testing()
  add_executable(universal_paste_core_tests tests/CoreTests.cpp)
  target_link_libraries(universal_paste_core_tests PRIVATE universal_paste_core)
  add_test(NAME universal_paste_core_tests COMMAND universal_paste_core_tests)
endif()
