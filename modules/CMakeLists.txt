file(GLOB_RECURSE CORE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/commands/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vision/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp"
)

add_library(CoreLib STATIC
    ${CORE_SOURCES}
)

target_include_directories(CoreLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/commands
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/tools
    ${CMAKE_CURRENT_SOURCE_DIR}/vision
)

target_link_libraries(CoreLib PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)