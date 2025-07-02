set(CORE_SOURCES
    tools/drawtools/rectangletool.cpp
    tools/drawtools/ellipsetool.cpp
    tools/drawtools/commontool.cpp
    tools/drawtools/selecttool.cpp
    tools/drawtools/polygontool.cpp
    tools/drawtools/linetool.cpp
    vision/canvasview.cpp
    vision/mainwindow.cpp
)

add_library(CoreLib STATIC
    ${CORE_SOURCES}
)

target_include_directories(CoreLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/tools
    ${CMAKE_CURRENT_SOURCE_DIR}/vision
)

target_link_libraries(CoreLib PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)