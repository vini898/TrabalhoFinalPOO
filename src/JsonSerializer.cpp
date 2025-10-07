#include "JsonSerializer.h"
#include "Board.h"
#include "Column.h"
#include "Card.h"
#include "User.h"
#include <sstream>
#include <iostream>

namespace kanban {

std::string JsonSerializer::serialize(const Board& board) const {
    std::ostringstream oss;
    serializeToStream(board, oss);
    return oss.str();
}

void JsonSerializer::serializeToStream(const Board& board, std::ostream& os) const {
    os << "{" << std::endl;
    os << "  \"board_id\": " << board.id() << "," << std::endl;
    os << "  \"title\": \"" << board.title() << "\"," << std::endl;
    
    // Serializar usuários
    os << "  \"users\": [";
    const auto& users = board.users();
    for (size_t i = 0; i < users.size(); ++i) {
        os << "{";
        os << "\"id\": " << users[i].id() << ",";
        os << "\"name\": \"" << users[i].name() << "\",";
        os << "\"email\": \"" << users[i].email() << "\"";
        os << "}";
        if (i < users.size() - 1) os << ",";
    }
    os << "]," << std::endl;
    
    // Serializar cartões
    os << "  \"cards\": [";
    const auto& cards = board.cards();
    for (size_t i = 0; i < cards.size(); ++i) {
        os << "{";
        os << "\"id\": " << cards[i].id() << ",";
        os << "\"title\": \"" << cards[i].title() << "\",";
        os << "\"description\": \"" << cards[i].description() << "\",";
        
        // Serializar tags
        os << "\"tags\": [";
        const auto& tags = cards[i].tags();
        for (size_t j = 0; j < tags.size(); ++j) {
            os << "\"" << tags[j] << "\"";
            if (j < tags.size() - 1) os << ",";
        }
        os << "],";
        
        // Serializar assignee e priority
        if (cards[i].assigneeId().has_value()) {
            os << "\"assignee_id\": " << *cards[i].assigneeId() << ",";
        }
        if (cards[i].priority().has_value()) {
            os << "\"priority\": " << *cards[i].priority() << ",";
        }
        
        os << "\"created_at\": \"2024-01-01\""; // Placeholder
        os << "}";
        if (i < cards.size() - 1) os << ",";
    }
    os << "]," << std::endl;
    
    // Serializar colunas
    os << "  \"columns\": [";
    const auto& columns = board.columns();
    for (size_t i = 0; i < columns.size(); ++i) {
        os << "{";
        os << "\"id\": " << columns[i].id() << ",";
        os << "\"name\": \"" << columns[i].name() << "\",";
        os << "\"order_index\": " << columns[i].orderIndex() << ",";
        
        // Serializar card IDs na coluna
        os << "\"card_ids\": [";
        const auto& cardIds = columns[i].cardIds();
        for (size_t j = 0; j < cardIds.size(); ++j) {
            os << cardIds[j];
            if (j < cardIds.size() - 1) os << ",";
        }
        os << "]";
        
        os << "}";
        if (i < columns.size() - 1) os << ",";
    }
    os << "]" << std::endl;
    
    os << "}" << std::endl;
}

void JsonSerializer::deserialize(const std::string& data, Board& outBoard) const {
    std::istringstream iss(data);
    deserializeFromStream(iss, outBoard);
}

void JsonSerializer::deserializeFromStream(std::istream& is, Board& outBoard) const {
    std::string line;
    std::string fullContent;
    
    // Ler todo o conteúdo
    while (std::getline(is, line)) {
        fullContent += line + "\n";
    }
    
    std::cout << "Desserializando JSON..." << std::endl;
    
    // Limpar board atual
    while (!outBoard.columns().empty()) {
        outBoard.removeColumn(outBoard.columns()[0].id());
    }
    while (!outBoard.cards().empty()) {
        outBoard.deleteCard(outBoard.cards()[0].id());
    }
    
    // Parse básico - vamos processar seções
    size_t pos = 0;
    
    // 1. Extrair título
    size_t titleStart = fullContent.find("\"title\": \"");
    if (titleStart != std::string::npos) {
        titleStart += 10;
        size_t titleEnd = fullContent.find("\"", titleStart);
        if (titleEnd != std::string::npos) {
            std::string title = fullContent.substr(titleStart, titleEnd - titleStart);
            outBoard.setTitle(title);
            std::cout << "Título: " << title << std::endl;
        }
    }
    
    // 2. Extrair e criar usuários
    std::vector<std::uint64_t> userIds;
    size_t usersStart = fullContent.find("\"users\": [");
    if (usersStart != std::string::npos) {
        usersStart += 9;
        size_t usersEnd = fullContent.find("]", usersStart);
        if (usersEnd != std::string::npos) {
            std::string usersSection = fullContent.substr(usersStart, usersEnd - usersStart);
            size_t userPos = 0;
            
            while ((userPos = usersSection.find('{', userPos)) != std::string::npos) {
                size_t userEnd = usersSection.find('}', userPos);
                if (userEnd == std::string::npos) break;
                
                std::string userStr = usersSection.substr(userPos, userEnd - userPos + 1);
                
                // Extrair dados do usuário
                size_t idStart = userStr.find("\"id\": ");
                size_t nameStart = userStr.find("\"name\": \"");
                size_t emailStart = userStr.find("\"email\": \"");
                
                if (idStart != std::string::npos && nameStart != std::string::npos) {
                    idStart += 6;
                    nameStart += 9;
                    emailStart += 10;
                    
                    size_t idEnd = userStr.find_first_of(",}", idStart);
                    size_t nameEnd = userStr.find("\"", nameStart);
                    size_t emailEnd = userStr.find("\"", emailStart);
                    
                    if (idEnd != std::string::npos && nameEnd != std::string::npos) {
                        std::uint64_t id = std::stoull(userStr.substr(idStart, idEnd - idStart));
                        std::string name = userStr.substr(nameStart, nameEnd - nameStart);
                        std::string email = (emailStart != std::string::npos && emailEnd != std::string::npos) 
                                          ? userStr.substr(emailStart, emailEnd - emailStart) 
                                          : "";
                        
                        // Adicionar usuário
                        outBoard.addUser(User(id, name, email));
                        userIds.push_back(id);
                        std::cout << "Usuário criado: " << name << " (ID: " << id << ")" << std::endl;
                    }
                }
                
                userPos = userEnd + 1;
            }
        }
    }
    
    // 3. Extrair e criar cartões
    std::vector<std::uint64_t> cardIds;
    size_t cardsStart = fullContent.find("\"cards\": [");
    if (cardsStart != std::string::npos) {
        cardsStart += 9;
        size_t cardsEnd = fullContent.find("]", cardsStart);
        if (cardsEnd != std::string::npos) {
            std::string cardsSection = fullContent.substr(cardsStart, cardsEnd - cardsStart);
            size_t cardPos = 0;
            
            while ((cardPos = cardsSection.find('{', cardPos)) != std::string::npos) {
                size_t cardEnd = cardsSection.find('}', cardPos);
                if (cardEnd == std::string::npos) break;
                
                std::string cardStr = cardsSection.substr(cardPos, cardEnd - cardPos + 1);
                
                // Extrair dados do cartão
                size_t idStart = cardStr.find("\"id\": ");
                size_t titleStart = cardStr.find("\"title\": \"");
                size_t descStart = cardStr.find("\"description\": \"");
                size_t assigneeStart = cardStr.find("\"assignee_id\": ");
                size_t priorityStart = cardStr.find("\"priority\": ");
                
                if (idStart != std::string::npos && titleStart != std::string::npos) {
                    idStart += 6;
                    titleStart += 10;
                    descStart += 16;
                    assigneeStart += 15;
                    priorityStart += 13;
                    
                    size_t idEnd = cardStr.find_first_of(",}", idStart);
                    size_t titleEnd = cardStr.find("\"", titleStart);
                    size_t descEnd = cardStr.find("\"", descStart);
                    
                    if (idEnd != std::string::npos && titleEnd != std::string::npos) {
                        std::uint64_t id = std::stoull(cardStr.substr(idStart, idEnd - idStart));
                        std::string title = cardStr.substr(titleStart, titleEnd - titleStart);
                        std::string description = (descStart != std::string::npos && descEnd != std::string::npos)
                                                ? cardStr.substr(descStart, descEnd - descStart)
                                                : "";
                        
                        // Criar cartão
                        auto& card = outBoard.createCard(title);
                        // Forçar o ID correto (já que createCard gera novo ID)
                        // Em uma implementação real, precisaríamos de um método setId
                        card.setDescription(description);
                        
                        // Extrair assignee
                        if (assigneeStart != std::string::npos) {
                            size_t assigneeEnd = cardStr.find_first_of(",}", assigneeStart);
                            if (assigneeEnd != std::string::npos) {
                                std::uint64_t assigneeId = std::stoull(cardStr.substr(assigneeStart, assigneeEnd - assigneeStart));
                                card.setAssigneeId(assigneeId);
                            }
                        }
                        
                        // Extrair priority
                        if (priorityStart != std::string::npos) {
                            size_t priorityEnd = cardStr.find_first_of(",}", priorityStart);
                            if (priorityEnd != std::string::npos) {
                                int priority = std::stoi(cardStr.substr(priorityStart, priorityEnd - priorityStart));
                                card.setPriority(priority);
                            }
                        }
                        
                        // Extrair tags
                        size_t tagsStart = cardStr.find("\"tags\": [");
                        if (tagsStart != std::string::npos) {
                            tagsStart += 9;
                            size_t tagsEnd = cardStr.find("]", tagsStart);
                            if (tagsEnd != std::string::npos) {
                                std::string tagsSection = cardStr.substr(tagsStart, tagsEnd - tagsStart);
                                size_t tagPos = 0;
                                
                                while ((tagPos = tagsSection.find('\"', tagPos)) != std::string::npos) {
                                    size_t tagEnd = tagsSection.find('\"', tagPos + 1);
                                    if (tagEnd == std::string::npos) break;
                                    
                                    std::string tag = tagsSection.substr(tagPos + 1, tagEnd - tagPos - 1);
                                    card.addTag(tag);
                                    std::cout << "  Tag adicionada: " << tag << std::endl;
                                    
                                    tagPos = tagEnd + 1;
                                }
                            }
                        }
                        
                        cardIds.push_back(id);
                        std::cout << "Cartão criado: " << title << " (ID: " << id << ")" << std::endl;
                    }
                }
                
                cardPos = cardEnd + 1;
            }
        }
    }
    
    // 4. Extrair e criar colunas
    std::vector<std::pair<std::uint64_t, std::vector<std::uint64_t>>> columnCards;
    size_t columnsStart = fullContent.find("\"columns\": [");
    if (columnsStart != std::string::npos) {
        columnsStart += 11;
        size_t columnsEnd = fullContent.find("]", columnsStart);
        if (columnsEnd != std::string::npos) {
            std::string columnsSection = fullContent.substr(columnsStart, columnsEnd - columnsStart);
            size_t columnPos = 0;
            
            while ((columnPos = columnsSection.find('{', columnPos)) != std::string::npos) {
                size_t columnEnd = columnsSection.find('}', columnPos);
                if (columnEnd == std::string::npos) break;
                
                std::string columnStr = columnsSection.substr(columnPos, columnEnd - columnPos + 1);
                
                // Extrair dados da coluna
                size_t nameStart = columnStr.find("\"name\": \"");
                size_t orderStart = columnStr.find("\"order_index\": ");
                size_t cardsStart = columnStr.find("\"card_ids\": [");
                
                if (nameStart != std::string::npos && orderStart != std::string::npos) {
                    nameStart += 9;
                    orderStart += 15;
                    cardsStart += 13;
                    
                    size_t nameEnd = columnStr.find("\"", nameStart);
                    size_t orderEnd = columnStr.find_first_of(",}", orderStart);
                    size_t cardsEnd = columnStr.find("]", cardsStart);
                    
                    if (nameEnd != std::string::npos && orderEnd != std::string::npos) {
                        std::string name = columnStr.substr(nameStart, nameEnd - nameStart);
                        int orderIndex = std::stoi(columnStr.substr(orderStart, orderEnd - orderStart));
                        
                        // Adicionar coluna
                        auto& column = outBoard.addColumn(name, orderIndex);
                        std::cout << "Coluna criada: " << name << " (order: " << orderIndex << ")" << std::endl;
                        
                        // Extrair card IDs da coluna
                        std::vector<std::uint64_t> columnCardIds;
                        if (cardsStart != std::string::npos && cardsEnd != std::string::npos) {
                            std::string cardsStr = columnStr.substr(cardsStart, cardsEnd - cardsStart);
                            size_t numPos = 0;
                            
                            while (numPos < cardsStr.length()) {
                                if (std::isdigit(cardsStr[numPos])) {
                                    size_t numEnd = cardsStr.find_first_not_of("0123456789", numPos);
                                    if (numEnd == std::string::npos) numEnd = cardsStr.length();
                                    
                                    std::uint64_t cardId = std::stoull(cardsStr.substr(numPos, numEnd - numPos));
                                    columnCardIds.push_back(cardId);
                                    
                                    // Adicionar cartão à coluna
                                    if (outBoard.findCard(cardId)) {
                                        column.addCard(cardId);
                                        std::cout << "  Cartão " << cardId << " adicionado à coluna" << std::endl;
                                    }
                                    
                                    numPos = numEnd;
                                } else {
                                    numPos++;
                                }
                            }
                        }
                        
                        columnCards.emplace_back(column.id(), std::move(columnCardIds));
                    }
                }
                
                columnPos = columnEnd + 1;
            }
        }
    }
    
    std::cout << "Desserialização completa!" << std::endl;
    std::cout << " - Usuários: " << userIds.size() << std::endl;
    std::cout << " - Cartões: " << cardIds.size() << std::endl;
    std::cout << " - Colunas: " << columnCards.size() << std::endl;
}

} // namespace kanban