cmake_minimum_required(VERSION 3.30.5)

set(PROJECT_NAME $ENV{PROJECT_NAME})

project($ENV{PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
set(QT_LIBS Qt6::Core Qt6::Gui Qt6::Widgets)

qt_standard_project_setup()

add_subdirectory(modules)

add_executable(${PROJECT_NAME} apps/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE GraphicItems Vision)
