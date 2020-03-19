CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
INCLUDE(ExternalProject)
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_BINARY_DIR}/include)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)
SET(ENV{PATH} ${CMAKE_BINARY_DIR}/bin:$ENV{PATH})
SET(ENV{PKG_CONFIG_PATH} ${CMAKE_BINARY_DIR}/lib/pkgconfig)

option(ENABLE_OPENSSL "compile with openssl" ON)

set(CURL_LIB ${CMAKE_BINARY_DIR}/lib/libcurl.a)
set(OTHER_LIB )
set(MY_DEPENDS)
set(MY_INSTALL_PREFIX -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR})

set(ANDROID_BUILD_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}"
    "-DCMAKE_TOOLCHAIN_FILE=/cross/github/jni/android-ndk-r14b/build/cmake/android.toolchain.cmake"
    "-DANDROID_TOOLCHAIN=clang"
    "-DANDROID_ABI=armeabi-v7a"
    "-DANDROID_PLATFORM=android-21"
    "-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang")


# 全局编译参数
add_compile_options("-fPIC")


# 全局的CMAKE_ARGS
set(MY_CMAKE_ARGS
    # 需要指定安装路径，否则安装会报没有权限
    ${MY_INSTALL_PREFIX}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    )
#todo : 这里可以使用try_compile 进行测试,是否可以使用某个参数
IF(CMAKE_COMPILER_IS_GNUCXX)
    # Needs fpic to link in shared lib on Linux with GCC
    LIST(APPEND MY_CMAKE_ARGS -DCMAKE_CXX_FLAGS=-fPIC)
    LIST(APPEND MY_CMAKE_ARGS -DCMAKE_C_FLAGS=-fPIC)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

if(ANDROID)
    LIST(APPEND MY_CMAKE_ARGS ${ANDROID_BUILD_ARGS})
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
endif()
if(BUILD_SHARED_LIBS)
    LIST(APPEND MY_CMAKE_ARGS -DBUILD_SHARED_LIBS=ON)
endif()

# 输出CMAKE 的编译参数
message(STATUS "------------>>> " ${MY_CMAKE_ARGS})


if(ENABLE_OPENSSL)
    set(OPENSSL_LIB ${CMAKE_BINARY_DIR}/lib/libssl.a)
    set(OPENSSL_LIB ${OPENSSL_LIB} ${CMAKE_BINARY_DIR}/lib/libcrypto.a)
    if(ANDROID)
        #[==[
        set(MY_DEPENDS ${MY_DEPENDS} zlib)
        # 如果是Android 的话，还需要编译zlib的库
        externalproject_add(
            zlib
            # 当前目录的压缩包，可以变成远程链接
            #URL http://zlib.net/zlib-1.2.11.tar.gz
            URL ${CMAKE_SOURCE_DIR}/vendor/zlib-1.2.11.tar.gz
            BUILD_COMMAND  $(MAKE)
            CMAKE_ARGS
            ${MY_CMAKE_ARGS}
            )
        # 测试如果调用了两遍会怎样
        # 如果别的项目引用了这个项目，他也导入了这个库怎么破？会不会出现重复定义呢？
        #find_library(zlib)
        #if(ZLIB_FOUND)
        #    message(STATUS "----------- hello zlib")
        #else()
        #    message(STATUS "----------- oh! no!!!")
        #endif()
        set(OTHER_LIB ${OTHER_LIB} ${CMAKE_BINARY_DIR}/lib/libz.a)
        #]==]

        set(OTHER_LIB ${OTHER_LIB} -lz )
    else()
        set(OTHER_LIB ${OTHER_LIB} -lz -ldl)
    endif()
    #openssl
    #externalproject_add(
    #    openssl
    #    # 当前目录的压缩包，可以变成远程链接
    #    URL ${CMAKE_SOURCE_DIR}/vendor/openssl-1.0.2m.tar.gz
    #    CONFIGURE_COMMAND ./Configure no-asm no-shared linux-elf -fPIC --prefix=${CMAKE_BINARY_DIR}
    #    BUILD_COMMAND make depend && make
    #    BUILD_IN_SOURCE 1
    #    )
    #[==[
    set(MY_DEPENDS ${MY_DEPENDS} openssl)
    externalproject_add(
        openssl
        GIT_REPOSITORY https://github.com/telenewbie/openssl-cmake.git
        CMAKE_COMMAND ${CMAKE_COMMAND}
        BUILD_COMMAND make
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}
        UPDATE_DISCONNECTED 1
        CMAKE_ARGS
        ${MY_CMAKE_ARGS}
        INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
        )


    externalproject_add(
        curl
        # 当前目录的压缩包，可以变成远程链接
        URL ${CMAKE_SOURCE_DIR}/vendor/curl-7.65.0.tar.gz
        DEPENDS ${MY_DEPENDS}
        #DEPENDS openssl
        CMAKE_ARGS
        ${MY_CMAKE_ARGS}
        -DOPENSSL_ROOT_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=NEVER
        -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=NEVER
        -DOPENSSL_USE_STATIC_LIBS=TRUE
        -DBUILD_SHARED_LIBS=OFF
        -DBUILD_CURL_EXE=OFF
        -DBUILD_TESTING=OFF
        -DCMAKE_BUILD_TYPE=Release
       BUILD_COMMAND make
        )
    set(MY_DEPENDS ${MY_DEPENDS} curl)
    #]==]
    set(targetName curl)
    find_path(had_imported
        name CMakeLists.txt
        PATHS ${CMAKE_BINARY_DIR}/${targetName}-prefix/src/${targetName}
        )
    message("------>${had_imported}")
    if(NOT had_imported)
        message("----------> had imported")
    else()
        message("------------> hadn't imported")
    endif()
else()
    set(MY_DEPENDS ${MY_DEPENDS} curl)
    externalproject_add(
        curl
        # 当前目录的压缩包，可以变成远程链接
        URL ${CMAKE_SOURCE_DIR}/vendor/curl-7.65.0.tar.gz
        CONFIGURE_COMMAND  cmake -DCMAKE_CROSSCOMPILING=TRUE -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR} -DCMAKE_USE_OPENSSL=OFF -DCURL_CA_PATH_SET=FALSE -DCURL_CA_BUNDLE_SET=FALSE -DBUILD_SHARED_LIBS=OFF
        BUILD_COMMAND PATH=$ENV{PATH} make
        BUILD_IN_SOURCE 1
        )
endif()
add_executable(t10 main.cpp a.cpp)
add_dependencies(t10 ${MY_DEPENDS})
set(MY_LINK_FLAGS)
if(ANDROID)
    set(MY_LINK_FLAGS ${MY_LINK_FLAGS})
elseif(UNIX)
    set(MY_LINK_FLAGS ${MY_LINK_FLAGS} -lpthread)
endif()


target_link_libraries(t10 PUBLIC ${CURL_LIB}
    ${OPENSSL_LIB}
    ${MY_LINK_FLAGS}
    ${OTHER_LIB}
    )
