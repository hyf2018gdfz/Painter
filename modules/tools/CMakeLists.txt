add_library(Tools STATIC
    basiccommands.hpp
)

target_include_directories(Tools PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Tools PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)