option(BUILD_SHARED_LIBS "compile shared libs" OFF)
#[==[
include(ExternalProject)
externalproject_add(
    openssl
    GIT_REPOSITORY https://github.com/janbar/openssl-cmake.git
    #URL ${CMAKE_SOURCE_DIR}/vendor/openssl-cmake-1.1.1.tar.gz
    #CONFIGURE_COMMAND ""
    CMAKE_COMMAND ${CMAKE_COMMAND}
    BUILD_COMMAND make
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
    #UPDATE_DISCONNECTED 1
    CMAKE_ARGS
    ${MY_CMAKE_ARGS}
    #        "-DCMAKE_CXX_FLAGS=-fPIC"
    #"-DCMAKE_C_FLAGS=-fPIC"
    -DBUILD_SHARED_LIBS=OFF
    BUILD_IN_SOURCE 1
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}

    )
#]==]
if(WIN32)
    set(PROTOBUF_LIB_DIR "${MYPROJ_SOURCE_DIR}/dependencies/win")
    set(prefix "")
    set(suffix ".lib")
elseif(APPLE)
    set(PROTOBUF_LIB_DIR "${MYPROJ_SOURCE_DIR}/dependencies/mac")
    set(prefix "lib")
    set(suffix ".a")
else()
    set(PROTOBUF_LIB_DIR "${MYPROJ_SOURCE_DIR}/dependencies/linux")
    set(prefix "lib")
    set(suffix ".a")
endif()

if(BUILD_SHARED_LIBS)
    set(suffix ".so")
endif()

set(OPENSSL_FOUND TRUE)
#set(TEMPDIR build3/)
message(STATUS "--------------------->CMAKE_CURRENT_BINARY_DIR:${CMAKE_CURRENT_BINARY_DIR}, CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
set(OPENSSL_INCLUDE_DIR ${CMAKE_BINARY_DIR}/${TEMPDIR}include)

set(OPENSSL_SSL_LIBRARY ${CMAKE_BINARY_DIR}/${TEMPDIR}lib/${prefix}ssl${suffix})
set(OPENSSL_CRYPTO_LIBRARY ${CMAKE_BINARY_DIR}/${TEMPDIR}lib/${prefix}crypto${suffix})
set(OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} )
set(OPENSSL_VERSION 1.1.1b)
