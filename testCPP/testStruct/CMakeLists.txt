cmake_minimum_required(VERSION 3.10)
project(testStruct)
set(CMAKE_VERBOSE_MAKEFILE on)
##################### options #########################################
option(USE_ENGINE "need use engine to test" OFF)
#option(USE_ELE_WEBRTC "use elevoc webrtc ?" OFF)
set(USE_ELE_WEBRTC ON CACHE BOOL "use elevoc webrtc" FORCE)
#######################################################################


########################### AEC #######################################
include_directories(Processer/impl/aec)
set(AEC_FILES
        Processer/impl/aec/AECProcesser.cpp
        Processer/impl/aec/AECProcesser.h
        Processer/impl/aec/BaseAECAdapter.cpp
        Processer/impl/aec/BaseAECAdapter.h
        Processer/impl/aec/AECConfig.cpp
        Processer/impl/aec/AECConfig.h
        )
if (USE_ELE_WEBRTC)
    # 包含 aec 头文件
    include_directories(include/ele_aec)
    include_directories(Processer/impl/aec/elevoc_webrtc)
    set(AEC_FILES
            ${AEC_FILES}
            Processer/impl/aec/elevoc_webrtc/WebrtcAec.cpp Processer/impl/aec/elevoc_webrtc/WebrtcAec.h
            Processer/impl/aec/elevoc_webrtc/EVAECProcess.cpp Processer/impl/aec/elevoc_webrtc/EVAECProcess.h
            )

    add_library(dsp STATIC IMPORTED)
    set_target_properties(dsp
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/windows/aec/x64/Debug/elevoc_teams_aec.lib
            )
#    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    if(WIN32)
        set(CompilerFlags
                CMAKE_CXX_FLAGS
                CMAKE_CXX_FLAGS_DEBUG
                CMAKE_CXX_FLAGS_RELEASE
                CMAKE_C_FLAGS
                CMAKE_C_FLAGS_DEBUG
                CMAKE_C_FLAGS_RELEASE
                )
        foreach(CompilerFlag ${CompilerFlags})
            string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach()
    endif()
#    message(STATUS ${CMAKE_CXX_FLAGS_DEBUG})
#    add_compile_options("$<$<CONFIG:DEBUG>:/MTd>")
#    add_compile_options("$<$<CONFIG:RELEASE>:/MT>")
#    set(WEBRTC_DEF NDEBUG
#            ELEVOCNSENGINE_EXPORTS
#            CURL_STATICLIB
#            EVLIB_EXPORTS
#            _CRT_SECURE_NO_WARNINGS
#            _WINDOWS
#            _USRDLL
#            FLOATING_POINT
#            OUTSIDE_SPEEX
#            USE_SSE)

    set(WEBRTC_LIBS
            dsp
            )
else ()
    message(FATAL_ERROR 452)
    include_directories(include/aec)
    include_directories(Processer/impl/aec/webrtc)
    set(AEC_FILES
            ${AEC_FILES}
            Processer/impl/aec/webrtc/WebrtcAec.cpp
            Processer/impl/aec/webrtc/WebrtcAec.h
            Processer/impl/aec/webrtc/EVDspProcess.cpp
            Processer/impl/aec/webrtc/EVDspProcess.h
            )

    add_library(dsp STATIC IMPORTED)
    set_target_properties(dsp
            PROPERTIES IMPORTED_LOCATION
            D:/workspace/AndroidStudio/AndroidStudioProjects/MyApplication/app/src/main/cpp/src/thirdparty/webrtc_dsp/build/Windows-10.0.18363/x64/mydsp.lib
            )
    set(WEBRTC_LIBS
            dsp
            )
endif ()
#######################################################################

########################### RESAMPLE #######################################
include_directories(Processer/impl/resample)
set(RESAMPLE_SRC Processer/impl/resample/speex/resample.c
        Processer/impl/resample/SpeexResamplerAdapter.cpp
        Processer/impl/resample/SpeexResamplerAdapter.h
        Processer/impl/resample/ResampleInProcesser.cpp
        Processer/impl/resample/ResampleInProcesser.h
        Processer/impl/resample/BaseResample.h
        Processer/impl/resample/ResampleOutProcesser.cpp
        Processer/impl/resample/ResampleOutProcesser.h
        Processer/impl/resample/BaseResampleAdapter.cpp
        Processer/impl/resample/BaseResampleAdapter.h
        )

#######################################################################

########################### SLICE #######################################
include_directories(Processer/impl/slice)
set(SLICE_SRC
        Processer/impl/slice/FrameSliceProcessor.cpp
        Processer/impl/slice/FrameSliceProcessor.h
        Processer/impl/slice/FrameSyntheticProcessor.cpp
        Processer/impl/slice/FrameSyntheticProcessor.h
        )
#######################################################################

########################### DNN #######################################

#######################################################################


include_directories(. Buffer config dao Processer Processer/impl  utils)

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
    #        message(FATAL_ERROR 123)
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
    include_directories(include/dnn )

    set(EXT_SRC
            #Processer/impl/dnn/DnnProcesser.cpp
            Processer/impl/dnn/DnnProcesser.h


            )
    add_library(dnn STATIC IMPORTED)
    set_target_properties(dnn
            PROPERTIES IMPORTED_LOCATION
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/SingleChannelNSDNNPro.lib
            )



    add_library(myshared SHARED sharedlibrary.cpp)
    target_link_libraries(myshared PRIVATE dnn
            )
    set_target_properties(myshared PROPERTIES LINK_OPTIONS "/WHOLEARCHIVE:$<TARGET_FILE_NAME:dnn> ")

    set(EXT_LIB
            myshared
            ${WEBRTC_LIBS}
            Wldap32.lib
            ws2_32.lib
            #[==[时间函数timeGetTime]==]
            winmm
            )
endif ()
set(WIN_REL_FLAGS $<1:Release>)

#add_compile_options("$<$<CONFIG:RELEASE>:/MT>")

#set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "$<$<CONFIG:Release>:/MT">)
#set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} ${WIN_REL_FLAGS})
#set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "$<$<CONFIG:Release>:/MTd">)
set(CMAKE_SUPPRESS_REGENERATION true)


set(SRC main.cpp ProcessCenter.cpp
        ProcessCenter.h
        Processer/BaseProcesser.cpp
        Processer/BaseProcesser.h
        Buffer/MyMicBuffer.cpp
        Buffer/MyMicBuffer.h
        Buffer/EVByteQueue.cpp
        Buffer/EVByteQueue.h
        Buffer/MyRefBuffer.cpp
        Buffer/MyRefBuffer.h
        EventCenter.cpp
        EventCenter.h
        dao/DataMsg.h
        dao/MsgCreator.cpp
        dao/MsgCreator.h
        dao/DataMsg.cpp
        config/ElevocLog.h
        config/EVAssert.cpp
        config/EVAssert.h
#        Processer/impl/FinalProcesser.cpp
#        Processer/impl/FinalProcesser.h
        ${EXT_SRC}
        ${AEC_FILES}
        ${RESAMPLE_SRC}
        ${SLICE_SRC}
        Processer/ProcessorConfig.cpp
        Processer/ProcessorConfig.h
        TEST_CONSTANT.h
#        utils/EVPCMHelper.cpp
        utils/DumpFileUtil.cpp
        utils/DumpFileUtil.h
        )

set(CMAKE_CXX_STANDARD 11)
add_executable(testStruct ${SRC})
target_compile_definitions(testStruct PUBLIC DNN_EVLIB_EXPORTS EVLIB_EXPORTS DEBUG ${EXT_DEF}
        ${WEBRTC_DEF}
        #fopen deprecated warning
        _CRT_SECURE_NO_DEPRECATE
        )

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


    target_compile_options(testStruct PRIVATE "/wd 4819")
    target_link_libraries(testStruct PRIVATE ${EXT_LIB})
endif ()
