cmake_minimum_required(VERSION 3.16)
project(zombie-rampage)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

include_directories("${PROJECT_SOURCE_DIR}/extlibs/SFML/include")
include_directories("${PROJECT_SOURCE_DIR}/extlibs/entityx")
include_directories("${PROJECT_SOURCE_DIR}/extlibs")

file(GLOB_RECURSE ZR_SRC "src/*.cpp" "include/*.hpp")

add_executable(zombie-rampage ${ZR_SRC})
if(ZR_SERVER)
  target_compile_definitions(zombie-rampage PUBLIC ZR_SERVER=${ZR_SERVER})
endif()

link_directories("/usr/local/lib")

target_link_libraries(zombie-rampage entityx)
target_link_libraries(zombie-rampage sfml-graphics)
target_link_libraries(zombie-rampage sfml-window)
target_link_libraries(zombie-rampage sfml-network)
target_link_libraries(zombie-rampage sfml-system)
