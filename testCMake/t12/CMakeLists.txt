cmake_minimum_required(VERSION 3.0)
project(t12)
include(ExternalProject)

set(ANDROID_NDK /cross/github/jni/android-ndk-r14b)
function(txz_import_library targetName url depends)
    message("-------------> start import ${targetName} from http://gitlab.txzing.com/${url}")
    message("------> set from function,have_set_android_ndk:${have_set_android_ndk}")
    message("------> set from function,ANDROID_CMAKE_ARGS:${ANDROID_CMAKE_ARGS}")
    message("------> set from function,have_set:${have_set}")
    message("------> set from function,MY_CMAKE_ARGS:${MY_CMAKE_ARGS}")
    message("------> set from function,had_imported_${targetName}:${had_imported_${targetName}}")

    set(MY_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}")
    if(ANDROID)
        message("--------->WOW!!! ANDROID")
        get_property(have_set_android_ndk VARIABLE PROPERTY ANDROID_NDK SET)
        if(NOT have_set_android_ndk)
            message(FATAL_ERROR "you must indicate ANDROID_NDK:${have_set_android_ndk}:${ANDROID_NDK} !!!")
        endif()
        # 是否设置了android的编译参数
        # 需要指定Android_ndk 的路径
        get_property(have_set VARIABLE PROPERTY ANDROID_CMAKE_ARGS SET)
        if(NOT have_set)
            message("--------> set ANDROID_CMAKE_ARGS")
            set(ANDROID_CMAKE_ARGS
                "-DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake"
                "-DANDROID_TOOLCHAIN=clang"
                "-DANDROID_PLATFORM=android-21"
                "-DANDROID_ABI=armeabi-v7a"
                "-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang")
            LIST(APPEND MY_CMAKE_ARGS ${ANDROID_CMAKE_ARGS})
        else()
            message("------------->have_set:${have_set}")
        endif()
        set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
    endif()
    # 如果已经有了这个则不需要再重新下载导入一遍了，当然，重新导入由于加入了相同的target名称导致ERROR
    find_path(had_imported_${targetName}
        name CMakeLists.txt
        PATHS ${CMAKE_BINARY_DIR}/${targetName}-prefix/src/${targetName}
        )
    message("-------------->MY_CMAKE_ARGS:${MY_CMAKE_ARGS}")
    if(NOT had_imported_${targetName})
        message("-------> start import ${targetName}")
        # 如果没有那个文件则需要导库
        externalproject_add(
            ${targetName}
            GIT_REPOSITORY http://gitlab.txzing.com/${url}
            DEPENDS ${depends}
            BUILD_COMMAND make
            #<prefix>/src
            #PREFIX ${CMAKE_BINARY_DIR}
            UPDATE_DISCONNECTED 1
            CMAKE_ARGS
            ${MY_CMAKE_ARGS}
            ${ARGN}
            )
    else()
        message("---------->${targetName}:${had_imported}")
    endif()
endfunction()

txz_import_library(openssl dev/lib3rd/opensource/openssl-cmake "")
#txz_import_library(libevent2  telenewbie/libevent "")
if(ANDROID)
    set(MY_CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}"
        "-DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake"
        "-DANDROID_TOOLCHAIN=clang"
        "-DANDROID_PLATFORM=android-21"
        "-DANDROID_ABI=armeabi-v7a"
        "-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang")
else()
    set(MY_CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}
        )
endif()
#[==[
set(depends openssl)
externalproject_add(
    libevent3
    GIT_REPOSITORY https://github.com/libevent/libevent.git
    DEPENDS ${depends}
    BUILD_COMMAND make
    #<prefix>/src
    #PREFIX ${CMAKE_BINARY_DIR}
    UPDATE_DISCONNECTED 1
    CMAKE_ARGS
    ${MY_CMAKE_ARGS}
    -DEVENT_INSTALL_CMAKE_DIR:PATH=${CMAKE_BINARY_DIR}/lib/cmake/libevent
    -DOPENSSL_USE_STATIC_LIBS=TRUE
    -DOPENSSL_ROOT_DIR=${CMAKE_BINARY_DIR}
    -DBUILD_SHARED_LIBS=OFF
    -DBUILD_TESTING=OFF
    -DEVENT__LIBRARY_TYPE:STRING=STATIC
    )

#]==]

# taglib
txz_import_library(taglib2 dev/lib3rd/opensource/taglib "")
#[==[
externalproject_add(
    taglib
    GIT_REPOSITORY https://github.com/taglib/taglib.git
    DEPENDS ${depends}
    BUILD_COMMAND make
    #<prefix>/src
    #PREFIX ${CMAKE_BINARY_DIR}
    UPDATE_DISCONNECTED 1
    CMAKE_ARGS
    ${MY_CMAKE_ARGS}
    -DBUILD_SHARED_LIBS=OFF
    -DBUILD_TESTING=OFF
    )
#]==]

txz_import_library(osal dev/client/common/osal "")
#txz_import_library(libmad dev/lib3rd/opensource/libmad "")
txz_import_library(libmad2 dev/lib3rd/opensource/libmad "")
#txz_import_library(opus dev/lib3rd/opensource/opus "")
txz_import_library(pcre dev/lib3rd/opensource/txz/pcre "")
txz_import_library(zlib dev/lib3rd/opensource/zlib "")
txz_import_library(bzip2 dev/lib3rd/opensource/bzip2 "")
txz_import_library(opus dev/lib3rd/opensource/opus "")


#todo 需要解决的一个问题：如果项目更新了，如何让他重新参与编译呢？难道每次都要删除码？不是把？

#opus
#[==[
externalproject_add(
    opus
    GIT_REPOSITORY https://github.com/xiph/opus.git
    CMAKE_ARGS
    ${MY_CMAKE_ARGS}
    -DBUILD_SHARED_LIBS=OFF
    -DBUILD_TESTING=OFF
    )
#]==]
add_library(t12 SHARED a.cpp)
#add_dependencies(t12 libmad)
