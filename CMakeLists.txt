cmake_minimum_required(VERSION 3.16)
project(KanbanLite LANGUAGES CXX)

# Padrão C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings (opcional, bom para Etapa 2)
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Biblioteca somente de headers (Interface)
add_library(kanban INTERFACE)
target_include_directories(kanban INTERFACE
    ${CMAKE_SOURCE_DIR}/design/include
)

# Executável de verificação: compila um .cpp que inclui todos os headers
# Arquivo: tests/headers_check.cpp (criar conforme instruído)
add_executable(headers_check tests/headers_check.cpp)
target_include_directories(headers_check PRIVATE ${CMAKE_SOURCE_DIR}/design/include)
target_link_libraries(headers_check PRIVATE kanban)

# Alvo “default” opcional para facilitar: build de verificação
add_custom_target(verify
    COMMAND ${CMAKE_COMMAND} --build . --target headers_check
    COMMENT "Verificando compilação dos headers (headers_check)"
)
