enable_testing()

add_executable(window_input_test input.cpp)
target_link_libraries(window_input_test rpg::lib rpg::test::lib
                      GTest::gtest_main)
target_compile_definitions(window_input_test PUBLIC RPG_TESTING)

add_custom_target(run_window_input_test $<TARGET_FILE:window_input_test>
                                        --gtest_color=yes)
