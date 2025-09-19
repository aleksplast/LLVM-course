target_sources(graphic_app PUBLIC main.c sim.c app.c)
target_include_directories(graphic_app PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(graphic_app PUBLIC SDL2)
