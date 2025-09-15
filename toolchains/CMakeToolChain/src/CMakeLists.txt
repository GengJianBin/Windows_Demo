# 定义可执行程序
add_executable(CMakeToolProject main.cpp)

# 如果需要额外的 include 目录，可以写：
 target_include_directories(CMakeToolProject PRIVATE ../include)

# 如果需要链接库，可以写：
target_link_libraries(CMakeToolProject PRIVATE ../libs)
