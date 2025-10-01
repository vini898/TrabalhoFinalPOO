cmake_minimum_required(VERSION 3.16)
project(KanbanLite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include público dos headers da etapa 1
add_library(kanban INTERFACE)
target_include_directories(kanban INTERFACE
    ${CMAKE_SOURCE_DIR}/design/include
)

# Core (implementações da etapa 2)
file(GLOB KANBAN_CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Board.cpp
    ${CMAKE_SOURCE_DIR}/src/Column.cpp
    ${CMAKE_SOURCE_DIR}/src/Card.cpp
    ${CMAKE_SOURCE_DIR}/src/User.cpp
    ${CMAKE_SOURCE_DIR}/src/ActivityLog.cpp
    ${CMAKE_SOURCE_DIR}/src/TagFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/AssigneeFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/JsonSerializer.cpp
    # IView.cpp é opcional por enquanto (interface pura)
)

add_library(kanban_core STATIC ${KANBAN_CORE_SOURCES})
target_include_directories(kanban_core PUBLIC
    ${CMAKE_SOURCE_DIR}/design/include
)
target_link_libraries(kanban_core PUBLIC kanban)

# Executável CLI mínimo para teste manual
add_executable(kanban_cli ${CMAKE_SOURCE_DIR}/tools/kanban_cli.cpp)
target_link_libraries(kanban_cli PRIVATE kanban_core)

# Alvo de verificação de headers (com stub .cpp para Windows)
add_executable(headers_check EXCLUDE_FROM_ALL ${CMAKE_SOURCE_DIR}/tools/headers_check.cpp)
target_include_directories(headers_check PRIVATE ${CMAKE_SOURCE_DIR}/design/include)