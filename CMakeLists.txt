cmake_minimum_required(VERSION 3.12)
project(FinanceTracker)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Define paths for source, headers, and libraries
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/libs")

# Include the header directory
include_directories(${INCLUDE_DIR})

# Collect all source files
file(GLOB_RECURSE PROJECT_SOURCES "${SRC_DIR}/*.c*")

# Add executable for the project
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Platform-specific settings
if(WIN32)
  # Set Raylib and Windows-specific libraries
  set(RAYLIB_LIBRARY "${LIB_DIR}/windows/raylib.lib")

  set(CURL_INCLUDE $ENV{CURL_INCLUDE})
  set(CURL_LIBRARY $ENV{CURL_LIB})

  include_directories(${CURL_INCLUDE})
  include_directories("/Library/Developer/CommandLineTools/SDKs/MacOSX14.5.sdk/usr/include/c++/v1/")

  #set(OPENSSL_LIBRARY $ENV{OPENSSL_LIB})
  #set(CRYPTO_LIBRARY $ENV{CRYPTO_LIB})
  #set(NGHTTP_LIBRARY $ENV{NGHTTP_LIB})
  #set(SSH_LIBRARY $ENV{SSH_LIB})
  #set(PSL_LIBRARY $ENV{PSL_LIB})

  message(STATUS "Curl include: ${CURL_INCLUDE}")
  message(STATUS "Curl library: ${CURL_LIBRARY}")
  #message(STATUS "OpenSSL library: ${OPENSSL_LIBRARY}")
  #message(STATUS "NGHTTP library: ${NGHTTP_LIBRARY}")
  #message(STATUS "Crypto library: ${CRYPTO_LIBRARY}")

  add_definitions(-DCURL_STATICLIB)

  target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARY})
  
  set(WINDOWS_LIBS winmm gdi32 kernel32 opengl32 ws2_32 wldap32 crypt32 advapi32 user32)
  target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARY} ${WINDOWS_LIBS})
else()
  # Set Raylib for Unix-based systems
  set(RAYLIB_PATH "${LIB_DIR}/unix/libraylib.a")
  find_library(RAYLIB_LIBRARY raylib HINTS "${RAYLIB_PATH}")
  
  target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARY} -lcurl)
  
  # Add additional warnings for Unix
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
