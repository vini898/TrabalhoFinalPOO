cmake_minimum_required(VERSION 3.16)
project(KanbanLite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Qt6 - versão específica que você tem
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Incluir módulos Qt necessários
set(QT_LIBS Qt6::Core Qt6::Widgets)

# Include público dos headers da etapa 1
add_library(kanban INTERFACE)
target_include_directories(kanban INTERFACE
    ${CMAKE_SOURCE_DIR}/design/include
)

# Core library (implementações da etapa 2)
file(GLOB KANBAN_CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Board.cpp
    ${CMAKE_SOURCE_DIR}/src/Column.cpp
    ${CMAKE_SOURCE_DIR}/src/Card.cpp
    ${CMAKE_SOURCE_DIR}/src/User.cpp
    ${CMAKE_SOURCE_DIR}/src/ActivityLog.cpp
    ${CMAKE_SOURCE_DIR}/src/TagFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/AssigneeFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/JsonSerializer.cpp
)

add_library(kanban_core STATIC ${KANBAN_CORE_SOURCES})
target_include_directories(kanban_core PUBLIC
    ${CMAKE_SOURCE_DIR}/design/include
)
target_link_libraries(kanban_core PUBLIC kanban)

# Criar pasta gui se não existir
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/gui/src)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/gui/include)

# GUI Qt - vamos criar estes arquivos
# GUI Qt - usar qt6_wrap_cpp para o MOC
set(GUI_HEADERS
    ${CMAKE_SOURCE_DIR}/gui/include/MainWindow.h
    ${CMAKE_SOURCE_DIR}/gui/include/BoardWidget.h
    ${CMAKE_SOURCE_DIR}/gui/include/CardWidget.h
    ${CMAKE_SOURCE_DIR}/gui/include/CardDialog.h
)

set(GUI_SOURCES
    ${CMAKE_SOURCE_DIR}/gui/src/main.cpp
    ${CMAKE_SOURCE_DIR}/gui/src/MainWindow.cpp
    ${CMAKE_SOURCE_DIR}/gui/src/BoardWidget.cpp
    ${CMAKE_SOURCE_DIR}/gui/src/CardWidget.cpp 
    ${CMAKE_SOURCE_DIR}/gui/src/CardDialog.cpp
)

# Processar headers para MOC
qt6_wrap_cpp(GUI_MOC_SOURCES ${GUI_HEADERS})

# Executável GUI Qt
add_executable(kanban_gui 
    ${GUI_SOURCES}
    ${GUI_HEADERS}
    ${GUI_MOC_SOURCES}
)

target_include_directories(kanban_gui PRIVATE 
    ${CMAKE_SOURCE_DIR}/gui/include
    ${CMAKE_SOURCE_DIR}/design/include
)

target_link_libraries(kanban_gui PRIVATE 
    kanban_core 
    Qt6::Core 
    Qt6::Widgets
)

# Configurações específicas do Qt
target_compile_definitions(kanban_gui PRIVATE QT_NO_KEYWORDS)