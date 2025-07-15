cmake_minimum_required(VERSION 3.30.5)

set(PROJECT_NAME $ENV{PROJECT_NAME})

project($ENV{PROJECT_NAME} LANGUAGES CXX)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(QT_NO_DEBUG_OUTPUT)
endif()

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
set(QT_LIBS Qt6::Core Qt6::Gui Qt6::Widgets)

qt_standard_project_setup()

add_subdirectory(modules)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(WIN32)
        add_executable(${PROJECT_NAME} WIN32 apps/main.cpp)
    else()
        add_executable(${PROJECT_NAME} apps/main.cpp)
    endif()
else()
    add_executable(${PROJECT_NAME} apps/main.cpp)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE CoreLib ${QT_LIBS})
