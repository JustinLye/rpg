enable_testing()

add_executable(controllers_movement_test movement.cpp)
target_link_libraries(controllers_movement_test rpg::lib rpg::test::lib
                      GTest::gtest_main)

add_custom_target(run_controllers_movement_test
                  $<TARGET_FILE:controllers_movement_test> --gtest_color=yes)
