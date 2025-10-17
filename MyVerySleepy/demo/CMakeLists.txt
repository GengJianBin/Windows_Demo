message(STATUS "当前项目名: ${PROJECT_NAME}")

file(GLOB DEMO_SRC 
    main.cpp
)

# define execulate program
add_executable(${PROJECT_NAME} 
    ${DEMO_SRC}
)

source_group("demo" FILES ${DEMO_SRC})

target_compile_definitions(${PROJECT_NAME} PUBLIC 
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
target_include_directories(${PROJECT_NAME} PRIVATE 
    ../src/
)

# take effect only target
target_link_directories(${PROJECT_NAME} PRIVATE
    ../libs/x64/$<CONFIG>/
    ${CMAKE_BINARY_DIR}/lib
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
  $<$<CONFIG:Debug>:wxbase31ud.lib>
  $<$<CONFIG:Debug>:wxzlibd.lib>
  $<$<CONFIG:Release>:wxbase31u.lib>
  $<$<CONFIG:Release>:wxzlib.lib>
  "PerfProfLib.lib"
)

# Make sure PerfProfLib is built before ${PROJECT_NAME}
add_dependencies(${PROJECT_NAME} PerfProfLib)
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

target_compile_options("${PROJECT_NAME}" PRIVATE 
  $<$<CONFIG:Debug>:/MTd>
  $<$<CONFIG:Release>:/MT>
)