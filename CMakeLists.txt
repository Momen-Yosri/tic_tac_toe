cmake_minimum_required(VERSION 3.16)
project(TicTacToe VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(GTest REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/game/gameengine.cpp
    src/game/ai_opponent.cpp
    src/auth/user_manager.cpp
    src/database/db_manager.cpp
    src/ui/mainwindow.cpp
    src/ui/loginwindow.cpp
    src/ui/gameboard.cpp
)

# Header files
set(HEADERS
    include/game/gameengine.h
    include/game/ai_opponent.h
    include/auth/user_manager.h
    include/database/db_manager.h
    include/ui/mainwindow.h
    include/ui/loginwindow.h
    include/ui/gameboard.h
)

# UI files
set(UI_FILES
    ui/mainwindow.ui
    ui/loginwindow.ui
    ui/gameboard.ui
)

# Create executable
add_executable(${PROJECT_NAME} 
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    SQLite::SQLite3
    GTest::GTest
)

# Tests
add_subdirectory(tests) 