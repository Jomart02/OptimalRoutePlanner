cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME RoutePlanner)
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/QtApp/include)


set(PROJECT_INCLUDE_DIR ./QtApp/include)
set(PROJECT_SRC_DIR     ./QtApp/src)
set(PROJECT_UI_DIR      ./QtApp/ui)

set(HEADERS
     ${PROJECT_INCLUDE_DIR}/MainWindow.h
     ${PROJECT_INCLUDE_DIR}/RouteScene.h
)

set(SOURCES
     ${PROJECT_SRC_DIR}/main.cpp
     ${PROJECT_SRC_DIR}/MainWindow.cpp
     ${PROJECT_SRC_DIR}/MainWindow.cpp
     ${PROJECT_SRC_DIR}/RouteScene.cpp
)



set(CMAKE_AUTOUIC_SEARCH_PATHS 
     ${PROJECT_UI_DIR}/
)

find_package(Qt6 COMPONENTS Widgets Core REQUIRED)

# Создаём исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES}  ${HEADERS} )

# Линкуем Qt
target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Core)
