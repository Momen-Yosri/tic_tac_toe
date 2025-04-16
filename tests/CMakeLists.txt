# Add test executable
add_executable(tictactoe_tests
    game_engine_test.cpp
    ai_opponent_test.cpp
    user_manager_test.cpp
    db_manager_test.cpp
)

# Link test executable with Google Test and project libraries
target_link_libraries(tictactoe_tests PRIVATE
    GTest::GTest
    GTest::Main
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    SQLite::SQLite3
)

# Add tests to CTest
add_test(NAME tictactoe_tests COMMAND tictactoe_tests)

# Enable test coverage
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tictactoe_tests PRIVATE --coverage)
    target_link_options(tictactoe_tests PRIVATE --coverage)
endif() 