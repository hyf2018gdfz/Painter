add_library(Vision STATIC
    mainwindow.cpp
    canvasview.cpp
)

target_include_directories(Vision PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/modules/tools
)

target_link_libraries(Vision PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)