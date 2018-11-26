add_library(game STATIC
        game_state.hxx
        game_state_manager.cxx game_state_manager.hxx dummy_game_state.cxx dummy_game_state.hxx keyboard_layout.hxx keyboard_layout.cxx)
target_include_directories(game PUBLIC ../contrib/GSL/include)
target_link_libraries(game PUBLIC sfml-system sfml-window)
target_link_libraries(game PUBLIC Boost::log)
