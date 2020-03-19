CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE(ExternalProject)
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_BINARY_DIR}/include)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)
SET(ENV{PATH} ${CMAKE_BINARY_DIR}/bin:$ENV{PATH})
SET(ENV{PKG_CONFIG_PATH} ${CMAKE_BINARY_DIR}/lib/pkgconfig)
# todo:目前进展，cmake支持解压一个库，并使用，但是不知道libevent怎么支持openssl，待解决，增加enable-openssl 还是不行。

 # 测试externalproject_add的使用，和压缩包一起使用
 #解压数据，并提供依赖包，以libevent为例

 # libevent 有两种配置，一种是包含openssl一种是不包含
 ## 先配置libevent不包含openssl的
 #[==[
 externalproject_add(
     event
     # 当前目录的压缩包，可以变成远程链接
     URL ${CMAKE_SOURCE_DIR}/vendor/libevent-2.1.11-stable.tar.gz
     CONFIGURE_COMMAND ./configure --enable-static  --disable-shared  --disable-samples  --disable-thread-support  --disable-malloc-replacement  --disable-libevent-regress --prefix=${CMAKE_BINARY_DIR}
     BUILD_IN_SOURCE 1
     )
 #]==]
 if(ANDROID)
     add_definitions("-fPIC -fPIE")
     add_compile_options(-fPIC -fPIE)
     set(CMAKE_SHARED_LINKER_FLAGS "-fPIC -fPIE")
     set(CMAKE_STATIC_LINKER_FLAGS "-fPIC -fPIE")
 endif()

 externalproject_add(
     openssl
     # 当前目录的压缩包，可以变成远程链接
     URL ${CMAKE_SOURCE_DIR}/vendor/openssl-1.0.2m.tar.gz
     CONFIGURE_COMMAND ./Configure no-asm no-shared linux-elf -fPIC --prefix=${CMAKE_BINARY_DIR}
     BUILD_COMMAND make depend && make
     BUILD_IN_SOURCE 1
     )
 externalproject_add(
     event
     # 当前目录的压缩包，可以变成远程链接
     DEPENDS openssl
     URL ${CMAKE_SOURCE_DIR}/vendor/libevent-2.1.11-stable.tar.gz
     CONFIGURE_COMMAND ./configure --enable-static  --disable-shared  --disable-samples  --disable-thread-support  --disable-malloc-replacement  --disable-libevent-regress --prefix=${CMAKE_BINARY_DIR} -enable-openssl
     BUILD_IN_SOURCE 1
     )

 ## 配置需要openssl的
  add_executable(t9 main1.cpp)
  set(EVENT_LIB ${CMAKE_BINARY_DIR}/lib/libevent.a)
  target_link_libraries(t9 PUBLIC ${EVENT_LIB})
  if(ANDROID)
      target_link_libraries(t9 PUBLIC -fPIC -fPIE)
  endif()
