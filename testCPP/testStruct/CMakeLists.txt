cmake_minimum_required(VERSION 3.10)
project(testStruct)
set(CMAKE_VERBOSE_MAKEFILE on)
option(USE_ENGINE "need use engine to test" false)
include_directories(. Buffer config dao Processer Processer/impl utils)
if (USE_ENGINE)
    message(FATAL_ERROR 456)
    include_directories(include/engine)
    set(EXT_DEF USE_ENGINE)
    set(EXT_SRC Processer/impl/EngineProcesser.cpp
            Processer/impl/EngineProcesser.h)

    add_library(engine STATIC IMPORTED)
    set_target_properties(engine
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/SingleChannelEngine.lib)

    set(EXT_LIB engine)
else ()
    #    message(FATAL_ERROR 123)
    set(EXT_DEF
            BUILDING_LIBCURL
            WEBRTC_WIN
            #[==[解决 std::min 的问题]==]
            -DNOMINMAX
            #[==[解决 winsock.h 和 winsock2.h 的问题]==]
            -DWIN32_LEAN_AND_MEAN
            #[==[比设置否则报错]==]
            -DWEBRTC_APM_DEBUG_DUMP=0
            EVLIB_EXPORTS
            DNN_EVLIB_EXPORTS
            -DOUTSIDE_SPEEX
            -DFLOATING_POINT
            )
    include_directories(include/dnn Processer/impl/resample)
    include_directories(include/aec)
    set(EXT_SRC Processer/impl/dnn/DnnProcesser.cpp
            Processer/impl/dnn/DnnProcesser.h
            Processer/impl/resample/speex/resample.c
            Processer/impl/aec/EVDspProcess.cpp
            )
    add_library(dnn STATIC IMPORTED)
    set_target_properties(dnn
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/SingleChannelNSDNNPro.lib
            )

        add_library(dsp STATIC IMPORTED)
        set_target_properties(dsp
                PROPERTIES IMPORTED_LOCATION
                D:/workspace/AndroidStudio/AndroidStudioProjects/MyApplication/app/src/main/cpp/src/thirdparty/webrtc_dsp/build/Windows-10.0.18363/x64/mydsp.lib
                )

    add_library(myshared SHARED sharedlibrary.cpp)
    target_link_libraries(myshared PRIVATE dnn
            )
#    set_target_properties(myshared PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:SingleChannelNSDNNPro ")
    set_target_properties(myshared PROPERTIES LINK_OPTIONS  "/WHOLEARCHIVE:$<TARGET_FILE_NAME:dnn> ")


    set(EXT_LIB
            myshared
            dsp
            Wldap32.lib
            ws2_32.lib
            #[==[时间函数timeGetTime]==]
            winmm
            )
endif ()
set(WIN_REL_FLAGS $<1:Release>)
#add_compile_options("$<$<CONFIG:DEBUG>:/MDd>")
#add_compile_options("$<$<CONFIG:RELEASE>:/MT>")

#set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "$<$<CONFIG:Release>:/MT">)
#set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} ${WIN_REL_FLAGS})
#set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "$<$<CONFIG:Release>:/MTd">)
set(CMAKE_SUPPRESS_REGENERATION true)


set(SRC main.cpp ProcessCenter.cpp
        ProcessCenter.h
        Processer/BaseProcesser.cpp
        Processer/BaseProcesser.h
        dao/DataMsg.h
        Buffer/MyMicBuffer.cpp Buffer/MyMicBuffer.h
        EventCenter.cpp EventCenter.h
        dao/MsgCreator.cpp
        dao/MsgCreator.h
        dao/DataMsg.cpp
        Buffer/EVByteQueue.cpp
        Buffer/EVByteQueue.h
        config/ElevocLog.h
        config/EVAssert.cpp
        config/EVAssert.h
        utils/EVPCMHelper.cpp
        Processer/impl/FinalProcesser.cpp
        Processer/impl/FinalProcesser.h
        ${EXT_SRC}
        Processer/ProcesserPori.h
        Processer/impl/resample/ResampleInProcesser.cpp
        Processer/impl/resample/ResampleInProcesser.h
        Processer/impl/resample/BaseResample.h
        TEST_CONSTANT.h Processer/impl/resample/ResampleOutProcesser.cpp
        Processer/impl/resample/ResampleOutProcesser.h
        Processer/impl/aec/AECProcesser.cpp
        Processer/impl/aec/AECProcesser.h
        Buffer/MyRefBuffer.cpp
        Buffer/MyRefBuffer.h
        Processer/HelloProcesser.cpp
        Processer/HelloProcesser.h
        Processer/impl/resample/BaseResampleAdapter.cpp
        Processer/impl/resample/BaseResampleAdapter.h Processer/impl/resample/SpeexResamplerAdapter.cpp Processer/impl/resample/SpeexResamplerAdapter.h)

set(CMAKE_CXX_STANDARD 11)
add_executable(testStruct ${SRC})
target_compile_definitions(testStruct PUBLIC DNN_EVLIB_EXPORTS EVLIB_EXPORTS DEBUG ${EXT_DEF})

#target_compile_options(testStruct PUBLIC /MT)

#set(CompilerFlags
#        CMAKE_CXX_FLAGS
#        CMAKE_CXX_FLAGS_DEBUG
#        CMAKE_CXX_FLAGS_RELEASE
#        CMAKE_C_FLAGS
#        CMAKE_C_FLAGS_DEBUG
#        CMAKE_C_FLAGS_RELEASE
#        )
#foreach (CompilerFlag ${CompilerFlags})
#    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
#endforeach ()

if (WIN32)
    message(STATUS "is win32 ${CMAKE_BUILD_TYPE}")

    target_link_libraries(testStruct PRIVATE ${EXT_LIB})
endif ()
