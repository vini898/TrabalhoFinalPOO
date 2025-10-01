#include <iostream>
#include <string>
#include <memory>
#include "../design/include/Board.h"
#include "../design/include/Card.h"
#include "../design/include/Column.h"
#include "../design/include/User.h"
#include "../design/include/TagFilter.h"
#include "../design/include/AssigneeFilter.h"

using namespace kanban;

class SimpleCLI {
private:
    Board m_board;

public:
    SimpleCLI() : m_board(1, "Meu Quadro Kanban") {
        // Setup inicial para demonstração
        setupDemoData();
    }

    void run() {
        std::cout << "=== Kanban CLI ===\n";
        
        while (true) {
            printMenu();
            int choice = getChoice();
            
            switch (choice) {
                case 1: createColumn(); break;
                case 2: createCard(); break;
                case 3: moveCard(); break;
                case 4: listEverything(); break;
                case 5: filterCards(); break;
                case 6: 
                    std::cout << "Saindo...\n";
                    return;
                default:
                    std::cout << "Opção inválida!\n";
            }
        }
    }

private:
    void setupDemoData() {
        // Colunas padrão
        m_board.addColumn("To Do", 0);
        m_board.addColumn("Doing", 1);
        m_board.addColumn("Done", 2);

        // Usuário demo
        m_board.addUser(User(1, "João", "joao@email.com"));

        // Cartão demo
        Card& card = m_board.createCard("Tarefa de Exemplo");
        card.setDescription("Esta é uma tarefa de exemplo");
        card.setAssigneeId(1);
        card.addTag("urgente");
        
        // Adiciona à primeira coluna
        if (auto* column = m_board.findColumn(1)) {
            column->addCard(card.id());
        }
    }

    void printMenu() {
        std::cout << "\n1. Criar coluna\n";
        std::cout << "2. Criar cartão\n";
        std::cout << "3. Mover cartão\n";
        std::cout << "4. Listar tudo\n";
        std::cout << "5. Filtrar cartões\n";
        std::cout << "6. Sair\n";
        std::cout << "Escolha: ";
    }

    int getChoice() {
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Limpa buffer
        return choice;
    }

    void createColumn() {
        std::string name;
        std::cout << "Nome da coluna: ";
        std::getline(std::cin, name);
        
        auto& column = m_board.addColumn(name, m_board.columns().size());
        std::cout << "Coluna criada: ID " << column.id() << "\n";
    }

    void createCard() {
        std::string title;
        std::cout << "Título do cartão: ";
        std::getline(std::cin, title);
        
        Card& card = m_board.createCard(title);
        
        // Adiciona à primeira coluna disponível
        if (!m_board.columns().empty()) {
            auto& firstColumn = m_board.columns()[0];
            firstColumn.addCard(card.id());
        }
        
        std::cout << "Cartão criado: ID " << card.id() << "\n";
    }

    void moveCard() {
        std::cout << "Funcionalidade de mover cartão - em desenvolvimento\n";
    }

    void listEverything() {
        std::cout << "\n=== QUADRO: " << m_board.title() << " ===\n";
        
        // Lista colunas e cartões
        for (const auto& column : m_board.columns()) {
            std::cout << "\n--- " << column.name() << " ---\n";
            
            for (Card::Id cardId : column.cardIds()) {
                if (const Card* card = m_board.findCard(cardId)) {
                    std::cout << "  [" << card->id() << "] " << card->title();
                    if (card->assigneeId()) {
                        if (const User* user = m_board.findUser(*card->assigneeId())) {
                            std::cout << " (@ " << user->name() << ")";
                        }
                    }
                    std::cout << "\n";
                }
            }
        }
    }

    void filterCards() {
        std::cout << "\nFiltrar por:\n";
        std::cout << "1. Tag\n";
        std::cout << "2. Responsável\n";
        
        int choice = getChoice();
        
        if (choice == 1) {
            std::string tag;
            std::cout << "Tag: ";
            std::getline(std::cin, tag);
            
            TagFilter filter(tag);
            auto results = m_board.filterCards(filter);
            
            std::cout << "\nCartões com tag '" << tag << "':\n";
            for (const Card* card : results) {
                std::cout << "  [" << card->id() << "] " << card->title() << "\n";
            }
        }
    }
};

int main() {
    try {
        SimpleCLI cli;
        cli.run();
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}