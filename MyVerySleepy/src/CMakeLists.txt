message(STATUS "当前项目名: ${PROJECT_NAME}")

file(GLOB PROFILER_HEADERS ./profiler/*.h)
file(GLOB PROFILER_SOURCES ./profiler/*.cpp)
file(GLOB UTILS 
    ./utils/container.h
    ./utils/dbginterface.h
    ./utils/dbginterface.cpp
    ./utils/except.h
    ./utils/mythread.h
    ./utils/mythread.cpp
    ./utils/osutils.cpp
    ./utils/sortlist.h
    ./utils/sortlist.cpp
    ./utils/stringutils.h
    ./utils/stringutils.cpp
    ./utils/WowW64.h
    ./utils/WowW64.cpp
)

# define execulate program
add_executable(${PROJECT_NAME} 
    main.cpp
    ${PROFILER_HEADERS}
    ${PROFILER_SOURCES}
    ${UTILS}
)

source_group("profiler" FILES ${PROFILER_HEADERS} ${PROFILER_SOURCES})
source_group("utils" FILES ${UTILS})

target_compile_definitions(${PROJECT_NAME} PUBLIC 
    _WIN64
    WIN32
    _DEBUG
    _WINDOWS
    _CRT_SECURE_NO_WARNINGS
    _CRT_SECURE_NO_DEPRECATE 
    _CRT_NONSTDC_NO_DEPRECATE
    UNICODE 
    _UNICODE
)

# extern include directory
# target_include_directories(${PROJECT_NAME} PRIVATE ../include)

# extern link libs
# target_link_libraries(${PROJECT_NAME} PRIVATE ../libs)
