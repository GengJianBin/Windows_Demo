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

# extern lib directory must be before add_execute or add_library and it is take effect in global
# link_directories(
#     ../libs/x64/Debug/
# )

# compile static library
add_library("${PROJECT_NAME}Lib" STATIC 
    ${PROFILER_HEADERS}
    ${PROFILER_SOURCES}
    ${UTILS}
    ${COMMON_WX}
)

source_group("profiler" FILES ${PROFILER_HEADERS} ${PROFILER_SOURCES})
source_group("utils" FILES ${UTILS})

target_compile_definitions("${PROJECT_NAME}Lib" PUBLIC 
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

message(STATUS "目录名: ${PROJECT_BINARY_DIR}")

# extern include directory
target_include_directories("${PROJECT_NAME}Lib" PRIVATE 
    ../include
    ./wxWidgets/include/
)

target_compile_options("${PROJECT_NAME}Lib" PRIVATE 
  $<$<CONFIG:Debug>:/MTd>
  $<$<CONFIG:Release>:/MT>
)