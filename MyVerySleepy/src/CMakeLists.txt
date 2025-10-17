message(STATUS "当前项目名: ${PROJECT_NAME}Lib")

file(GLOB PROFILER_HEADERS 
    ./profiler/debugger.h
    ./profiler/processinfo.h
    ./profiler/profiler.h
    ./profiler/profilerthread.h
    ./profiler/symbolinfo.h
    ./profiler/threadinfo.h
)

file(GLOB PROFILER_SOURCES 
    ./profiler/debugger.cpp
    ./profiler/processinfo.cpp
    ./profiler/profiler.cpp
    ./profiler/profilerthread.cpp
    ./profiler/symbolinfo.cpp
    ./profiler/threadinfo.cpp
)

file(GLOB UTILS 
    #./utils/container.h
    ./utils/dbginterface.h
    ./utils/dbginterface.cpp
    ./utils/except.h
    ./utils/mythread.h
    ./utils/mythread.cpp
    ./utils/osutils.cpp
    #./utils/sortlist.h
    #./utils/sortlist.cpp
    ./utils/stringutils.h
    ./utils/stringutils.cpp
    ./utils/WoW64.h
    ./utils/WoW64.cpp
)

file(GLOB COMMON_WX 
    ./wxWidgets/include/*.*
)

source_group("profiler" FILES ${PROFILER_HEADERS} ${PROFILER_SOURCES})
source_group("utils" FILES ${UTILS})

# extern lib directory must be before add_execute or add_library and it is take effect in global
# link_directories(
#     ../libs/x64/Debug/
# )

set(STATIC_LIB_NAME "PerfProfLib")
set(SHARED_LIB_NAME "PerfProf")

message(STATUS "目录名: ${PROJECT_BINARY_DIR}")

# compile static library
add_library(${STATIC_LIB_NAME} STATIC 
    ${PROFILER_HEADERS}
    ${PROFILER_SOURCES}
    ${UTILS}
    ${COMMON_WX}
)

target_compile_definitions(${STATIC_LIB_NAME} PUBLIC 
    _WIN64
    WIN32
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    _WINDOWS
    _CRT_SECURE_NO_WARNINGS
    _CRT_SECURE_NO_DEPRECATE 
    _CRT_NONSTDC_NO_DEPRECATE
    UNICODE 
    _UNICODE
)

# extern include directory
target_include_directories(${STATIC_LIB_NAME} PRIVATE 
    ../include
    ./wxWidgets/include/
)

target_compile_options(${STATIC_LIB_NAME} PRIVATE 
  $<$<CONFIG:Debug>:/MTd>
  $<$<CONFIG:Release>:/MT>
)


# compile shared library
add_library(${SHARED_LIB_NAME} SHARED 
    ${PROFILER_HEADERS}
    ${PROFILER_SOURCES}
    ${UTILS}
    ${COMMON_WX}
)

target_compile_definitions(${SHARED_LIB_NAME} PUBLIC 
    _WIN64
    WIN32
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    _WINDOWS
    _CRT_SECURE_NO_WARNINGS
    _CRT_SECURE_NO_DEPRECATE 
    _CRT_NONSTDC_NO_DEPRECATE
    UNICODE 
    _UNICODE
)

# extern include directory
target_include_directories(${SHARED_LIB_NAME} PRIVATE 
    ../include
    ./wxWidgets/include/
)

target_compile_options(${SHARED_LIB_NAME} PRIVATE 
  $<$<CONFIG:Debug>:/MTd>
  $<$<CONFIG:Release>:/MT>
)

# take effect only target
target_link_directories(${SHARED_LIB_NAME} PRIVATE
    ../libs/x64/$<CONFIG>/
)

# extern link libs
target_link_libraries(${SHARED_LIB_NAME} PRIVATE
  comctl32.lib
  rpcrt4.lib
  advapi32.lib
  shlwapi.lib
  psapi.lib
  #crashback.lib
  dbgeng.lib
  $<$<CONFIG:Debug>:wxbase31ud.lib>
  $<$<CONFIG:Debug>:wxzlibd.lib>
  $<$<CONFIG:Release>:wxbase31u.lib>
  $<$<CONFIG:Release>:wxzlib.lib>
)