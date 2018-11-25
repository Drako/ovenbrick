add_executable(ovenbrick_tests test_main.cxx game_state_manager_test.cxx)
target_link_libraries(ovenbrick_tests game)
target_include_directories(ovenbrick_tests PRIVATE ../contrib/Catch2/single_include/catch2)
add_test(NAME headless_tests COMMAND ovenbrick_tests)
