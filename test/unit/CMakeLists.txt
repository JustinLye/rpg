enable_testing()

add_executable(scheduled_action scheduled_action.cpp)
target_link_libraries(scheduled_action rpg::lib rpg::test::lib
                      GTest::gtest_main)

add_custom_target(run_scheduled_action_test $<TARGET_FILE:scheduled_action>
                                            --gtest_color=yes)
add_dependencies(run_all_unit_tests run_scheduled_action_test)

add_subdirectory(controllers)
add_subdirectory(window)
