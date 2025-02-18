# 规定cmake最低版本
cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

# 设置项目名称
set(TARGET_NAME "itkFastTest")
project(${TARGET_NAME})

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置头文件的包含/include路径
set(ITK_STUDY_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}
)
include_directories(${ITK_STUDY_INCLUDES})

# 查找源文件, 设置源文件正则/过滤器
file(GLOB PROJECT_SOURCE_FILES *.cc *.cpp *.cxx) # 设置过滤器
aux_source_directory(src PROJECT_SOURCE_FILES) # 对 src 目录执行 PROJECT_SOURCE_FILES对应的过滤操作

# 设置ITK_DIR目录
if(NOT ITK_DIR)
  set(ITK_DIR "/Users/huangshan/Documents/software/InsightToolkit-5.2.1/build")
endif()

find_package(ITK REQUIRED)  
include(${ITK_USE_FILE})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # 可执行文件，这句写在子目录的CMakeLists不生效

# 这句要在 target_link_libraries之前，不然会报错 ${TARGET_NAME}没有生成
add_executable(${TARGET_NAME} ${PROJECT_SOURCE_FILES})

# ITK_LIBRARIES变量，这个变量里的内容是要链接的库
target_link_libraries(${TARGET_NAME} PRIVATE ${ITK_LIBRARIES})



