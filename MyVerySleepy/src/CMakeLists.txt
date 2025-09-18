message(STATUS "当前项目名: ${PROJECT_NAME}")

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

# define execulate program
add_executable(${PROJECT_NAME} 
    main.cpp
    ${PROFILER_HEADERS}
    ${PROFILER_SOURCES}
    ${UTILS}
    ${COMMON_WX}
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

message(STATUS "目录名: ${PROJECT_BINARY_DIR}")

# extern include directory
target_include_directories(${PROJECT_NAME} PRIVATE 
    ../include
    ./wxWidgets/include/
)

# take effect only target
target_link_directories(${PROJECT_NAME} PRIVATE
    ../libs/x64/Debug/
)

# extern link libs
target_link_libraries(${PROJECT_NAME} PRIVATE
  comctl32.lib
  rpcrt4.lib
  advapi32.lib
  shlwapi.lib
  psapi.lib
  #crashback.lib
  dbgeng.lib
  wxbase31ud.lib
  wxzlibd.lib
)

set(OBJ_FILE_DIR $<TARGET_FILE_DIR:${PROJECT_NAME}>)
# copy dll to exe path
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "OBJ_FILE_DIR is ${OBJ_FILE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "CMAKE_SOURCE_DIR is ${CMAKE_SOURCE_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/dll/dbghelp_x64/dbghelpdr.dll      ${OBJ_FILE_DIR}/dbghelpdr.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/dll/dbghelp_x64/dbghelpms.dll      ${OBJ_FILE_DIR}/dbghelpms.dll   
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/dll/dbghelp_x64/dbghelpms6.dll     ${OBJ_FILE_DIR}/dbghelpms6.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/dll/dbghelp_x64/dbghelpw.dll       ${OBJ_FILE_DIR}/dbghelpw.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/dll/dbghelp_x64/dbghelpw_wow64.dll ${OBJ_FILE_DIR}/dbghelpw_wow64.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/dll/dbghelp_x64/srcsrv.dll         ${OBJ_FILE_DIR}srcsrv.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/dll/dbghelp_x64/symsrv.dll         ${OBJ_FILE_DIR}/symsrv.dll 
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/dll/dbghelp_x64/symsrv.yes         ${OBJ_FILE_DIR}/symsrv.yes
)