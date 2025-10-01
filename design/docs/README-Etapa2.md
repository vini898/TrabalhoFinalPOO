# Kanban Lite - Etapa 2 (CLI)

## 📋 Descrição
CLI (Command Line Interface) funcional para o sistema de gerenciamento de tarefas colaborativas Kanban. Desenvolvido em C++ moderno como parte da disciplina de Programação Orientada a Objetos.

## 🛠 Build
```bash
# Criar pasta de build e configurar
mkdir build && cd build
cmake ..

# Compilar o projeto
cmake --build .

# No Windows use:
# cmake --build . --config Release
```

## 🚀 Execução
```bash
# Linux/Mac
./kanban_cli

# Windows
.\kanban_cli.exe
```

## 🎯 Funcionalidades Implementadas

### Operações Principais
- ✅ **Criar colunas** - Adicionar novas colunas ao quadro (ex: "To Do", "Doing", "Done")
- ✅ **Criar cartões** - Criar tarefas com título, descrição, tags e responsável
- ✅ **Mover cartões** - Transferir cartões entre colunas com registro no activity log
- ✅ **Listar quadro completo** - Visualizar todas as colunas e seus cartões
- ✅ **Filtrar cartões** - Buscar cartões por tags específicas
- ✅ **Gerenciar usuários** - Adicionar e atribuir usuários aos cartões

### Funcionalidades de Demo
- Setup automático com dados de exemplo
- Colunas pré-configuradas: "To Do", "Doing", "Done"
- Usuário demo e cartão de exemplo com tags

## 🏗 Arquitetura e Design POO

### Principais Classes
- **`Board`** - Agrega colunas, cartões e usuários
- **`Column`** - Lista ordenada de cartões
- **`Card`** - Item de trabalho com metadados
- **`User`** - Participante do sistema
- **`ActivityLog`** - Registro de atividades
- **`ICardFilter`** - Interface para filtros polimórficos

### Requisitos POO Atendidos

| Requisito | Implementação |
|-----------|---------------|
| **Abstração & Encapsulamento** | Interfaces claras (.h/.cpp), campos privados, getters/setters |
| **Classes e Objetos** | Arquitetura MVC-like: Model (Board, Card), View (CLI) |
| **Herança & Polimorfismo** | `ICardFilter` → `TagFilter`, `AssigneeFilter` |
| **Composição** | `Board` contém `vector<Column>`, `Column` contém `vector<Card::Id>` |
| **STL Containers** | `std::vector`, `std::unordered_map`, `std::optional`, `std::string` |
| **Sobrecarga de Operadores** | `Card::operator<`, `Card::operator==`, `Column::operator<` |
| **Smart Pointers** | Design preparado para `std::unique_ptr` e `std::shared_ptr` |
| **Tratamento de Exceções** | Validações básicas e tratamento de erros de entrada |

## 📁 Estrutura do Projeto
```
POO - PROJETO FINAL/
├── design/include/     # Headers da Etapa 1
├── src/               # Implementações da Etapa 2
├── tools/             # CLI e utilitários
├── build/             # Gerado pelo CMake
└── CMakeLists.txt     # Build automation
```

## 🎮 Como Usar

### Fluxo Básico
1. **Execute o programa**: `./kanban_cli`
2. **Liste o quadro**: Opção 4 para ver o estado atual
3. **Crie um cartão**: Opção 2 com título desejado
4. **Movimente cartões**: Opção 3 seguindo os IDs mostrados
5. **Filtre por tags**: Opção 5 para buscar cartões específicos

### Exemplo de Sessão
```
=== Kanban CLI ===
1. Criar coluna
2. Criar cartão
3. Mover cartão
4. Listar tudo
5. Filtrar cartões
6. Sair

Escolha: 4

=== QUADRO: Meu Quadro Kanban ===

--- [1] To Do ---
  [1] Tarefa de Exemplo (@ João) [Tags: urgente ]

--- [2] Doing ---
  (vazia)

--- [3] Done ---
  (vazia)
```

## 🔧 Tecnologias e Padrões
- **Linguagem**: C++17
- **Build System**: CMake
- **STL**: Containers, algoritmos, smart pointers
- **Padrões**: RAII, Interface Segregation, Composition over Inheritance

## 📝 Notas de Desenvolvimento
- Foco em código limpo e boas práticas de C++ moderno
- Separação clara entre lógica de negócio e interface
- Preparado para extensão com GUI na Etapa 3
- Uso de namespaces para organização do código

---

**Próxima etapa**: Interface gráfica (Qt/JUCE) e funcionalidades avançadas de persistência.