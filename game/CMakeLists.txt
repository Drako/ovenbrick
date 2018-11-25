add_library(game STATIC
        game_state.hxx
        game_state_manager.cxx game_state_manager.hxx)
target_include_directories(game PUBLIC ../contrib/GSL/include)
target_link_libraries(game sfml-system)
