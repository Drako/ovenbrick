configure_file(resources.cxx.in resources.cxx @ONLY)
add_library(game STATIC
        game_state.hxx
        game_state_manager.cxx game_state_manager.hxx
        dummy_game_state.cxx dummy_game_state.hxx
        keyboard_layout.hxx keyboard_layout.cxx
        resources.hxx "${CMAKE_CURRENT_BINARY_DIR}/resources.cxx")
target_include_directories(game PUBLIC ../contrib/GSL/include ../contrib/incbin ../contrib/ddic)
target_link_libraries(game PUBLIC sfml-system sfml-window sfml-graphics)
target_link_libraries(game PUBLIC Boost::log)
target_link_libraries(game PUBLIC ddic)
