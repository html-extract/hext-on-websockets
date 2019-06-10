cmake_minimum_required(VERSION 3.8)

project(hext-on-websockets VERSION 0.0.1)

# Add the top-level cmake module directory to CMAKE_MODULE_PATH
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

include(HextEnableWarnings)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Hext)
find_package(Boost 1.70)
find_package(Threads)
find_package(RapidJSON)
find_package(OpenSSL)

add_executable(
  hextws
  ${PROJECT_SOURCE_DIR}/ws/AppendResultToJson.cpp
  ${PROJECT_SOURCE_DIR}/ws/HandleRequest.cpp
  ${PROJECT_SOURCE_DIR}/ws/HextExtractor.cpp
  ${PROJECT_SOURCE_DIR}/ws/Listener.cpp
  ${PROJECT_SOURCE_DIR}/ws/Message.cpp
  ${PROJECT_SOURCE_DIR}/ws/Session.cpp
  ${PROJECT_SOURCE_DIR}/ws/SetupSSL.cpp
  ${PROJECT_SOURCE_DIR}/ws/main.cpp)

target_include_directories(
  hextws SYSTEM
    PRIVATE
      $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
      $<BUILD_INTERFACE:${OPENSSL_INCLUDE_DIR}>
      $<BUILD_INTERFACE:${RapidJSON_INCLUDE_DIRS}>)
target_include_directories(
  hextws
    PRIVATE
      $<BUILD_INTERFACE:${HEXT_INCLUDE_DIR}>
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/ws>)

target_link_libraries(
  hextws
  ${HEXT_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
  ${OPENSSL_LIBRARIES})

install(TARGETS hextws DESTINATION bin)

