cmake_minimum_required(VERSION 3.25)

project(hext-on-websockets VERSION 0.0.1)

# Add the top-level cmake module directory to CMAKE_MODULE_PATH
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

option(HEXTWS_SECCOMP "Use seccomp" OFF)
option(HEXTWS_SECCOMP_TEST "Enable seccomp testing" OFF)

include(HextEnableWarnings)

find_package(Hext)
find_package(Boost)
find_package(Threads)
find_package(RapidJSON)
find_package(OpenSSL)
find_package(Libseccomp)

add_executable(
  hextws
  ${PROJECT_SOURCE_DIR}/ws/AppendResultToJson.cpp
  ${PROJECT_SOURCE_DIR}/ws/HandleRequest.cpp
  ${PROJECT_SOURCE_DIR}/ws/HextExtractor.cpp
  ${PROJECT_SOURCE_DIR}/ws/Listener.cpp
  ${PROJECT_SOURCE_DIR}/ws/Message.cpp
  ${PROJECT_SOURCE_DIR}/ws/Session.cpp
  ${PROJECT_SOURCE_DIR}/ws/SetupSSL.cpp
  "$<$<BOOL:${HEXTWS_SECCOMP}>:${PROJECT_SOURCE_DIR}/ws/SetupSeccomp.cpp>"
  ${PROJECT_SOURCE_DIR}/ws/main.cpp)
hext_enable_warnings(hextws PRIVATE)

target_include_directories(
  hextws
    PRIVATE
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/ws>)

target_compile_definitions(
  hextws
    PRIVATE
      "$<$<CONFIG:Debug>:HEXTWS_DEBUG>"
      "$<$<CONFIG:Debug>:BOOST_ASIO_ENABLE_HANDLER_TRACKING=1>"
      "$<$<BOOL:${HEXTWS_SECCOMP}>:HEXTWS_SECCOMP>"
      "$<$<BOOL:${HEXTWS_SECCOMP_TEST}>:HEXTWS_SECCOMP_TEST>")

target_link_libraries(
  hextws
  hext::hext
  Threads::Threads
  OpenSSL::SSL
  "$<$<BOOL:${HEXTWS_SECCOMP}>:Libseccomp::Libseccomp>")

install(TARGETS hextws DESTINATION bin)

