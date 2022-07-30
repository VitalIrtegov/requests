cmake_minimum_required(VERSION 3.14)

project(requests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(Qt6 REQUIRED COMPONENTS Network)

add_executable(requests
  main.cpp
)
target_link_libraries(requests PRIVATE Qt6::Network Qt${QT_VERSION_MAJOR}::Core)

# добавил: target_link_libraries(requests PRIVATE Qt6::Network) - не работает
# было ранее: target_link_libraries(requests Qt${QT_VERSION_MAJOR}::Core)
# mytarget
