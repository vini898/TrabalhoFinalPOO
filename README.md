# 📋 Kanban Lite - Sistema de Gerenciamento de Tarefas

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Qt](https://img.shields.io/badge/Qt-6.9.3-green.svg)
![CMake](https://img.shields.io/badge/CMake-3.16+-brightgreen.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

Sistema completo de gerenciamento de tarefas colaborativas desenvolvido em **C++** com **Qt**, implementando os principais conceitos de **Programação Orientada a Objetos**.

## 🎯 Sobre o Projeto

Projeto final da disciplina de **Programação Orientada a Objetos (POO)**, implementando um sistema Kanban com interface gráfica moderna e todos os conceitos fundamentais de POO em C++.

### ✨ Funcionalidades

- ✅ **Quadros Kanban completos** com colunas personalizáveis
- 🎯 **Cartões arrastáveis** entre colunas (drag & drop)
- 👥 **Sistema de usuários** local para atribuição de tarefas
- 📊 **Histórico de atividades** em tempo real
- 🏷️ **Tags e prioridades** para organização
- 💾 **Persistência** com serialização JSON
- 🎨 **Interface moderna** estilo Trello

## 🛠 Tecnologias

- **Linguagem**: C++17
- **GUI Framework**: Qt 6.9.3
- **Build System**: CMake
- **Serialização**: JSON customizado
- **Containers**: STL (vector, map, optional)

## 📁 Estrutura do Projeto

```
POO - PROJETO FINAL/
├── design/                   # Etapa 1 - Design e documentação
│   ├── include/             # Headers públicos
│   └── docs/               # UML e relatórios
├── src/                     # Etapa 2 - Implementações core
│   ├── Board.cpp
│   ├── Column.cpp
│   ├── Card.cpp
│   └── ...
├── gui/                     # Etapa 3 - Interface gráfica
│   ├── include/
│   ├── src/
│   └── resources/
├── tools/                   # CLI e utilitários
└── CMakeLists.txt          # Build system principal
```

## 🚀 Como Executar

### Pré-requisitos

- **Qt 6.9.3** ou superior
- **CMake 3.16** ou superior
- Compilador **C++17** (GCC, Clang, MSVC)

### Build com CMake

```bash
# Clone o repositório
git clone https://github.com/seu-usuario/poo-kanban.git
cd poo-kanban

# Configure o projeto
cmake -B build-gui

# Compile
cmake --build build-gui

# Execute
./build-gui/kanban_gui.exe
```

### Build no Qt Creator

1. Abra o `CMakeLists.txt` no Qt Creator
2. Configure o kit com Qt 6.9.3
3. Build e execute o projeto

## 🎬 Demonstração

[📹 **Assista ao Vídeo Demonstrativo**]([https://drive.google.com/link-para-video](https://drive.google.com/file/d/1lQz9pfNeXQ1KdbzlCuYAZtlPy_xtxkjK/view?usp=sharing))

**Funcionalidades demonstradas:**
- Criação e edição de cartões
- Sistema de drag & drop entre colunas
- Histórico de atividades
- Atribuição de usuários
- Interface completa

## 📊 Conceitos POO Implementados

| Conceito | Implementação | Arquivos |
|----------|---------------|----------|
| **Abstração** | Interfaces `ISerializer`, `ICardFilter` | `design/include/ISerializer.h` |
| **Encapsulamento** | Campos privados com getters/setters | `Board.h`, `Card.h` |
| **Herança** | `JsonSerializer → ISerializer` | `JsonSerializer.h` |
| **Polimorfismo** | Uso de interfaces para serialização | `JsonSerializer.cpp` |
| **Composição** | `Board` contém `Columns`, `Columns` contém `Cards` | `Board.cpp`, `Column.cpp` |
| **Templates** | Uso extensivo de STL containers | Todos os headers |
| **RAII** | Gerenciamento automático de recursos | Todos os arquivos |
| **Exceções** | Tratamento de erros críticos | `Board.cpp`, `MainWindow.cpp` |

## 🏗 Arquitetura

### Core Domain
```cpp
Board → Columns → Cards
       ↓
     Users
       ↓
 ActivityLog
```

### GUI Architecture
```cpp
MainWindow → BoardWidget → ColumnWidget → CardWidget
       ↓
   CardDialog
       ↓
 HistoryDialog
```

### Design Patterns
- **Observer**: Atualização automática da GUI
- **Strategy**: Filtros e serialização
- **Factory**: Criação de cartões e colunas
- **RAII**: Gerenciamento de recursos

## 📈 Entregas Realizadas

### ✅ Etapa 1 - Design (4.0/4.0)
- Headers completos das classes principais
- Diagrama UML detalhado
- Relatório de justificativas de design
- CMakeLists.txt inicial

### ✅ Etapa 2 - CLI (4.0/4.0) 
- Implementação core funcional
- Executável CLI para testes
- Uso de smart pointers e STL
- Tratamento de exceções

### ✅ Etapa 3 - GUI (4.0/4.0)
- Interface gráfica completa com Qt
- Drag & drop entre colunas
- Histórico de atividades
- Relatório técnico final
- Vídeo demonstrativo

## 👨‍💻 Desenvolvimento

### Comandos Úteis

```bash
# Build rápido
cmake --build build-gui

# Build com mais threads
cmake --build build-gui --parallel 4

# Limpar build
cmake --build build-gui --target clean

# Debug build
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug
```

### Estrutura de Desenvolvimento

- **`design/include/`**: Interfaces públicas
- **`src/`**: Implementações da lógica de negócio
- **`gui/src/`**: Implementações da interface gráfica
- **`tools/`**: Utilitários e testes

## 🐛 Solução de Problemas

### Problemas Comuns

1. **Qt não encontrado**
   ```bash
   # Defina o caminho do Qt
   export QT_DIR=/c/Qt/6.9.3/mingw_64
   ```

2. **Erros de compilação**
   - Verifique a versão do C++ (requer C++17)
   - Confirme a instalação do Qt Widgets module

3. **Runtime errors**
   - Certifique-se de que todas as DLLs do Qt estão no PATH

## 📄 Licença

Este projeto está licenciado sob a **MIT License** - veja o arquivo [LICENSE](LICENSE) para detalhes.

## 👤 Autor

**Vinícius Ferraz do Nascimento** - [GitHub](https://github.com/vini898)

- Matrícula: 20240010760
- Disciplina: Programação Orientada a Objetos
- Instituição: UFPB

## 🙋‍♂️ Suporte

Encontrou um problema? [Abra uma issue](https://github.com/seu-usuario/poo-kanban/issues) no GitHub.

---

<div align="center">

**⭐️ Se este projeto foi útil, deixe uma estrela no repositório!**

</div>
