#include "../include/MainWindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent),
      m_board(1, "Meu Quadro Kanban"),
      m_boardWidget(nullptr),  // ← Inicialize como nullptr
      m_newBoardAction(nullptr),
      m_openBoardAction(nullptr),
      m_saveBoardAction(nullptr),
      m_exitAction(nullptr),
      m_aboutAction(nullptr)
{
    std::cout << "MainWindow: Construtor iniciado" << std::endl;
    
    try {
        std::cout << "MainWindow: Carregando dados demo..." << std::endl;
        setupDemoData();
        
        std::cout << "MainWindow: Configurando UI..." << std::endl;
        setupUI();
        
        // Agora force a atualização do BoardWidget
        if (m_boardWidget) {
            std::cout << "MainWindow: Forçando atualização do BoardWidget..." << std::endl;
            m_boardWidget->refreshColumns();
        }
        
        std::cout << "MainWindow: Configurando menus..." << std::endl;
        setupMenus();
        
        std::cout << "MainWindow: Configurando conexões..." << std::endl;
        setupConnections();
        
        statusBar()->showMessage("Pronto - Kanban Lite");
        std::cout << "MainWindow: Construtor completo" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "ERRO CRÍTICO no MainWindow: " << e.what() << std::endl;
        setWindowTitle("Kanban Lite - Erro");
        resize(400, 200);
    }
}

void MainWindow::setupDemoData() {
    std::cout << "SetupDemoData: Iniciando..." << std::endl;
    
    // Limpar qualquer dado existente
    while (!m_board.columns().empty()) {
        m_board.removeColumn(m_board.columns()[0].id());
    }
    while (!m_board.cards().empty()) {
        m_board.deleteCard(m_board.cards()[0].id());
    }
    std::cout << "Dados limpos" << std::endl;
    
    // Adicionar colunas e GUARDAR OS IDs
    std::cout << "Criando colunas..." << std::endl;
    m_board.addColumn("To Do", 0);
    m_board.addColumn("Doing", 1); 
    m_board.addColumn("Done", 2);
    
    std::cout << "Colunas criadas: " << m_board.columns().size() << std::endl;
    
    // Verificar colunas criadas
    for (const auto& col : m_board.columns()) {
        std::cout << "Coluna: '" << col.name() << "' (ID: " << col.id() << ")" << std::endl;
    }
    
    // Adicionar usuário demo
    m_board.addUser(kanban::User(1, "João", "joao@email.com"));
    std::cout << "Usuário criado" << std::endl;
    
    // Criar cartões e ADICIONAR DIRETAMENTE PELO BOARD
    std::cout << "Criando e distribuindo cartões..." << std::endl;
    
    // Cartão 1 na coluna 1 (To Do)
    auto& card1 = m_board.createCard("Tarefa de Exemplo 1");
    card1.setDescription("Esta é uma tarefa de demonstração");
    card1.setAssigneeId(1);
    card1.addTag("urgente");
    card1.setPriority(1);
    
    // Adicionar à coluna To Do (ID 1)
    if (auto* col1 = m_board.findColumn(1)) {
        std::cout << "Adicionando cartão 1 à coluna To Do..." << std::endl;
        bool success = col1->addCard(card1.id());
        std::cout << "Resultado: " << success << std::endl;
    }
    
    // Cartão 2 na coluna 1 (To Do)  
    auto& card2 = m_board.createCard("Revisar Documentação");
    card2.setDescription("Revisar a documentação do projeto");
    card2.addTag("documentação");
    card2.setPriority(2);
    
    // Adicionar à coluna To Do (ID 1)
    if (auto* col1 = m_board.findColumn(1)) {
        std::cout << "Adicionando cartão 2 à coluna To Do..." << std::endl;
        bool success = col1->addCard(card2.id());
        std::cout << "Resultado: " << success << std::endl;
    }
    
    // Verificar estado final
    std::cout << "Estado final das colunas:" << std::endl;
    for (const auto& col : m_board.columns()) {
        std::cout << " - " << col.name() << ": " << col.cardIds().size() << " cartões" << std::endl;
        for (auto cardId : col.cardIds()) {
            if (auto* card = m_board.findCard(cardId)) {
                std::cout << "   * " << card->title() << " (ID: " << card->id() << ")" << std::endl;
            }
        }
    }
    
    std::cout << "SetupDemoData: Completo" << std::endl;
}

void MainWindow::setupUI() {
    setWindowTitle("Kanban Lite");
    setMinimumSize(1000, 700);
    
    // Central widget com BoardWidget
    m_boardWidget = new BoardWidget(m_board, this);
    setCentralWidget(m_boardWidget);
    
    std::cout << "SetupUI: BoardWidget criado" << std::endl;
}

void MainWindow::setupMenus() {
    // Menu Arquivo
    QMenu* fileMenu = menuBar()->addMenu("&Arquivo");
    
    m_newBoardAction = fileMenu->addAction("&Novo Quadro");
    m_openBoardAction = fileMenu->addAction("&Abrir...");
    m_saveBoardAction = fileMenu->addAction("&Salvar");
    fileMenu->addSeparator();
    m_exitAction = fileMenu->addAction("&Sair");
    
    // Menu Cartões - NOVO MENU
    QMenu* cardMenu = menuBar()->addMenu("&Cartões");
    QAction* newCardAction = cardMenu->addAction("&Novo Cartão");
    
    // Menu Ajuda
    QMenu* helpMenu = menuBar()->addMenu("&Ajuda");
    m_aboutAction = helpMenu->addAction("&Sobre");
    
    // Conectar novo cartão 
    connect(newCardAction, &QAction::triggered, this, &MainWindow::onNewCard);
    
    std::cout << "SetupMenus: Menus configurados" << std::endl;
}

void MainWindow::onNewCard() {
    std::cout << "MainWindow: Abrindo diálogo para novo cartão..." << std::endl;
    
    // Verificar se há colunas disponíveis
    if (m_board.columns().empty()) {
        QMessageBox::warning(this, "Aviso", "Crie pelo menos uma coluna antes de adicionar cartões.");
        return;
    }
    
    // Criar e mostrar diálogo
    CardDialog dialog(m_board, this);
    if (dialog.exec() == QDialog::Accepted) {
        std::cout << "MainWindow: Criando novo cartão..." << std::endl;
        
        // Criar cartão no board
        auto& newCard = m_board.createCard(dialog.getTitle().toStdString());
        newCard.setDescription(dialog.getDescription().toStdString());
        
        // Configurar tags
        auto tags = dialog.getTags();
        for (const auto& tag : tags) {
            newCard.addTag(tag);
        }
        
        // Configurar assignee
        if (auto assigneeId = dialog.getAssigneeId()) {
            newCard.setAssigneeId(*assigneeId);
        }
        
        // Configurar prioridade
        if (auto priority = dialog.getPriority()) {
            newCard.setPriority(*priority);
        }
        
        // Adicionar à PRIMEIRA coluna (To Do) - CORRIGIDO
        if (!m_board.columns().empty()) {
            // Usar findColumn para obter referência não-const
            auto* firstColumn = m_board.findColumn(m_board.columns()[0].id());
            if (firstColumn) {
                bool added = firstColumn->addCard(newCard.id());
                
                if (added) {
                    std::cout << "MainWindow: Cartão adicionado à coluna '" << firstColumn->name() << "'" << std::endl;
                    
                    // Registrar atividade
                    kanban::ActivityLog::Entry entry;
                    entry.timestamp = std::chrono::system_clock::now();
                    entry.action = "CREATE_CARD";
                    entry.details = "Novo cartão criado: " + newCard.title();
                    m_board.activityLog().append(std::move(entry));
                    
                    // Atualizar interface
                    if (m_boardWidget) {
                        m_boardWidget->refreshColumns();
                    }
                    
                    statusBar()->showMessage("Cartão criado com sucesso!");
                } else {
                    std::cout << "ERRO: Não foi possível adicionar cartão à coluna" << std::endl;
                    QMessageBox::warning(this, "Erro", "Não foi possível adicionar o cartão à coluna.");
                }
            }
        }
    } else {
        std::cout << "MainWindow: Criação de cartão cancelada" << std::endl;
    }
}

void MainWindow::setupConnections() {
    // Conectar usando lambda functions
    connect(m_newBoardAction, &QAction::triggered, [this]() { onNewBoard(); });
    connect(m_openBoardAction, &QAction::triggered, [this]() { onOpenBoard(); });
    connect(m_saveBoardAction, &QAction::triggered, [this]() { onSaveBoard(); });
    connect(m_exitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(m_aboutAction, &QAction::triggered, [this]() { onAbout(); });
    
    std::cout << "SetupConnections: Conexões estabelecidas" << std::endl;
}

void MainWindow::onNewBoard() {
    QMessageBox::information(this, "Novo Quadro", "Criando novo quadro...");
}

void MainWindow::onOpenBoard() {
    QString fileName = QFileDialog::getOpenFileName(this, "Abrir Quadro", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "Abrir", "Abrindo: " + fileName);
    }
}

void MainWindow::onSaveBoard() {
    QString fileName = QFileDialog::getSaveFileName(this, "Salvar Quadro", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "Salvar", "Salvando: " + fileName);
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "Sobre Kanban Lite", 
        "Kanban Lite v1.0\n\n"
        "Sistema de gerenciamento de tarefas colaborativas\n"
        "Desenvolvido para a disciplina de POO - C++\n"
        "Qt 6.9.3");
}