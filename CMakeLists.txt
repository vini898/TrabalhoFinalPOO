cmake_minimum_required(VERSION 3.16)
project(KanbanLite LANGUAGES CXX)

# Padrão C++ e warnings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Biblioteca somente de headers para validar include/compilação
add_library(kanban INTERFACE)
target_include_directories(kanban INTERFACE
    ${CMAKE_SOURCE_DIR}/design/include
)

# Opcional: alvo de teste de inclusão para forçar compilação dos headers
add_executable(headers_check EXCLUDE_FROM_ALL
    ${CMAKE_SOURCE_DIR}/design/include/Card.h
    ${CMAKE_SOURCE_DIR}/design/include/Column.h
    ${CMAKE_SOURCE_DIR}/design/include/Board.h
    ${CMAKE_SOURCE_DIR}/design/include/User.h
    ${CMAKE_SOURCE_DIR}/design/include/ActivityLog.h
    ${CMAKE_SOURCE_DIR}/design/include/ICardFilter.h
    ${CMAKE_SOURCE_DIR}/design/include/TagFilter.h
    ${CMAKE_SOURCE_DIR}/design/include/AssigneeFilter.h
    ${CMAKE_SOURCE_DIR}/design/include/ISerializer.h
    ${CMAKE_SOURCE_DIR}/design/include/JsonSerializer.h
    ${CMAKE_SOURCE_DIR}/design/include/IView.h
)
target_link_libraries(headers_check PRIVATE kanban)
