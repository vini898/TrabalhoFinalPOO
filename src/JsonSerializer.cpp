#include "JsonSerializer.h"
#include "Board.h"
#include "Column.h"
#include "Card.h"
#include "User.h"
#include <sstream>
#include <iostream>
#include <algorithm>

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
    
    std::cout << "=== INICIANDO DESSERIALIZAÇÃO FUNCIONAL ===" << std::endl;
    
    // Limpar board atual
    while (!outBoard.columns().empty()) {
        outBoard.removeColumn(outBoard.columns()[0].id());
    }
    while (!outBoard.cards().empty()) {
        outBoard.deleteCard(outBoard.cards()[0].id());
    }
    while (!outBoard.users().empty()) {
        outBoard.removeUser(outBoard.users()[0].id());
    }
    
    // 1. Extrair e configurar título
    size_t titlePos = fullContent.find("\"title\": \"");
    if (titlePos != std::string::npos) {
        titlePos += 10; // "\"title\": \""
        size_t titleEnd = fullContent.find("\"", titlePos);
        if (titleEnd != std::string::npos) {
            std::string title = fullContent.substr(titlePos, titleEnd - titlePos);
            outBoard.setTitle(title);
            std::cout << "Título definido: " << title << std::endl;
        }
    }
    
    // 2. Extrair e criar USUÁRIOS
    size_t usersStart = fullContent.find("\"users\": [");
    if (usersStart != std::string::npos) {
        usersStart += 9;
        size_t usersEnd = fullContent.find("]", usersStart);
        if (usersEnd != std::string::npos) {
            std::string usersSection = fullContent.substr(usersStart, usersEnd - usersStart);
            
            size_t userStart = 0;
            while ((userStart = usersSection.find('{', userStart)) != std::string::npos) {
                size_t userEnd = usersSection.find('}', userStart);
                if (userEnd == std::string::npos) break;
                
                std::string userStr = usersSection.substr(userStart, userEnd - userStart + 1);
                
                // Extrair ID, nome e email
                size_t idPos = userStr.find("\"id\": ");
                size_t namePos = userStr.find("\"name\": \"");
                size_t emailPos = userStr.find("\"email\": \"");
                
                if (idPos != std::string::npos && namePos != std::string::npos) {
                    idPos += 6;
                    namePos += 9;
                    emailPos += 10;
                    
                    size_t idEnd = userStr.find_first_of(",}", idPos);
                    size_t nameEnd = userStr.find("\"", namePos);
                    size_t emailEnd = userStr.find("\"", emailPos);
                    
                    if (idEnd != std::string::npos && nameEnd != std::string::npos) {
                        std::uint64_t id = std::stoull(userStr.substr(idPos, idEnd - idPos));
                        std::string name = userStr.substr(namePos, nameEnd - namePos);
                        std::string email = (emailPos < userStr.length() && emailEnd != std::string::npos) 
                                          ? userStr.substr(emailPos, emailEnd - emailPos) 
                                          : "";
                        
                        // Adicionar usuário
                        outBoard.addUser(kanban::User(id, name, email));
                        std::cout << "Usuário criado: " << name << " (ID: " << id << ")" << std::endl;
                    }
                }
                
                userStart = userEnd + 1;
            }
        }
    }
    
    // 3. Extrair e criar CARTÕES
    std::vector<std::uint64_t> allCardIds;
    size_t cardsStart = fullContent.find("\"cards\": [");
    if (cardsStart != std::string::npos) {
        cardsStart += 9;
        size_t cardsEnd = fullContent.find("]", cardsStart);
        if (cardsEnd != std::string::npos) {
            std::string cardsSection = fullContent.substr(cardsStart, cardsEnd - cardsStart);
            
            size_t cardStart = 0;
            while ((cardStart = cardsSection.find('{', cardStart)) != std::string::npos) {
                size_t cardEnd = cardsSection.find('}', cardStart);
                if (cardEnd == std::string::npos) break;
                
                std::string cardStr = cardsSection.substr(cardStart, cardEnd - cardStart + 1);
                
                // Extrair dados do cartão
                size_t idPos = cardStr.find("\"id\": ");
                size_t titlePos = cardStr.find("\"title\": \"");
                size_t descPos = cardStr.find("\"description\": \"");
                size_t assigneePos = cardStr.find("\"assignee_id\": ");
                size_t priorityPos = cardStr.find("\"priority\": ");
                
                if (idPos != std::string::npos && titlePos != std::string::npos) {
                    idPos += 6;
                    titlePos += 10;
                    descPos += 16;
                    assigneePos += 15;
                    priorityPos += 13;
                    
                    size_t idEnd = cardStr.find_first_of(",}", idPos);
                    size_t titleEnd = cardStr.find("\"", titlePos);
                    size_t descEnd = cardStr.find("\"", descPos);
                    
                    if (idEnd != std::string::npos && titleEnd != std::string::npos) {
                        std::uint64_t id = std::stoull(cardStr.substr(idPos, idEnd - idPos));
                        std::string title = cardStr.substr(titlePos, titleEnd - titlePos);
                        std::string description = (descPos < cardStr.length() && descEnd != std::string::npos)
                                                ? cardStr.substr(descPos, descEnd - descPos)
                                                : "";
                        
                        // Criar cartão
                        auto& card = outBoard.createCard(title);
                        card.setDescription(description);
                        allCardIds.push_back(id);
                        
                        // Configurar assignee
                        if (assigneePos < cardStr.length()) {
                            size_t assigneeEnd = cardStr.find_first_of(",}", assigneePos);
                            if (assigneeEnd != std::string::npos) {
                                std::uint64_t assigneeId = std::stoull(cardStr.substr(assigneePos, assigneeEnd - assigneePos));
                                card.setAssigneeId(assigneeId);
                            }
                        }
                        
                        // Configurar priority
                        if (priorityPos < cardStr.length()) {
                            size_t priorityEnd = cardStr.find_first_of(",}", priorityPos);
                            if (priorityEnd != std::string::npos) {
                                int priority = std::stoi(cardStr.substr(priorityPos, priorityEnd - priorityPos));
                                card.setPriority(priority);
                            }
                        }
                        
                        // Extrair e adicionar tags
                        size_t tagsStart = cardStr.find("\"tags\": [");
                        if (tagsStart != std::string::npos) {
                            tagsStart += 9;
                            size_t tagsEnd = cardStr.find("]", tagsStart);
                            if (tagsEnd != std::string::npos) {
                                std::string tagsStr = cardStr.substr(tagsStart, tagsEnd - tagsStart);
                                size_t tagPos = 0;
                                
                                while ((tagPos = tagsStr.find('\"', tagPos)) != std::string::npos) {
                                    size_t tagEnd = tagsStr.find('\"', tagPos + 1);
                                    if (tagEnd == std::string::npos) break;
                                    
                                    std::string tag = tagsStr.substr(tagPos + 1, tagEnd - tagPos - 1);
                                    card.addTag(tag);
                                    tagPos = tagEnd + 1;
                                }
                            }
                        }
                        
                        std::cout << "Cartão criado: " << title << " (ID: " << id << ")" << std::endl;
                    }
                }
                
                cardStart = cardEnd + 1;
            }
        }
    }
    
    // 4. Extrair e criar COLUNAS e associar cartões
    size_t columnsStart = fullContent.find("\"columns\": [");
    if (columnsStart != std::string::npos) {
        columnsStart += 11;
        size_t columnsEnd = fullContent.find("]", columnsStart);
        if (columnsEnd != std::string::npos) {
            std::string columnsSection = fullContent.substr(columnsStart, columnsEnd - columnsStart);
            
            size_t columnStart = 0;
            while ((columnStart = columnsSection.find('{', columnStart)) != std::string::npos) {
                size_t columnEnd = columnsSection.find('}', columnStart);
                if (columnEnd == std::string::npos) break;
                
                std::string columnStr = columnsSection.substr(columnStart, columnEnd - columnStart + 1);
                
                // Extrair dados da coluna
                size_t namePos = columnStr.find("\"name\": \"");
                size_t orderPos = columnStr.find("\"order_index\": ");
                size_t cardsPos = columnStr.find("\"card_ids\": [");
                
                if (namePos != std::string::npos && orderPos != std::string::npos) {
                    namePos += 9;
                    orderPos += 15;
                    cardsPos += 13;
                    
                    size_t nameEnd = columnStr.find("\"", namePos);
                    size_t orderEnd = columnStr.find_first_of(",}", orderPos);
                    size_t cardsEnd = columnStr.find("]", cardsPos);
                    
                    if (nameEnd != std::string::npos && orderEnd != std::string::npos) {
                        std::string name = columnStr.substr(namePos, nameEnd - namePos);
                        int orderIndex = std::stoi(columnStr.substr(orderPos, orderEnd - orderPos));
                        
                        // Adicionar coluna
                        auto& column = outBoard.addColumn(name, orderIndex);
                        std::cout << "Coluna criada: " << name << " (order: " << orderIndex << ")" << std::endl;
                        
                        // Associar cartões à coluna
                        if (cardsPos < columnStr.length() && cardsEnd != std::string::npos) {
                            std::string cardsStr = columnStr.substr(cardsPos, cardsEnd - cardsPos);
                            
                            // Extrair IDs dos cartões
                            size_t numPos = 0;
                            while (numPos < cardsStr.length()) {
                                if (std::isdigit(cardsStr[numPos])) {
                                    size_t numEnd = cardsStr.find_first_not_of("0123456789", numPos);
                                    if (numEnd == std::string::npos) numEnd = cardsStr.length();
                                    
                                    std::uint64_t cardId = std::stoull(cardsStr.substr(numPos, numEnd - numPos));
                                    
                                    // Verificar se o cartão existe e adicionar à coluna
                                    if (std::find(allCardIds.begin(), allCardIds.end(), cardId) != allCardIds.end()) {
                                        column.addCard(cardId);
                                        std::cout << "  + Cartão " << cardId << " adicionado à coluna" << std::endl;
                                    }
                                    
                                    numPos = numEnd;
                                } else {
                                    numPos++;
                                }
                            }
                        }
                    }
                }
                
                columnStart = columnEnd + 1;
            }
        }
    }
    
    std::cout << "=== DESSERIALIZAÇÃO COMPLETA ===" << std::endl;
    std::cout << "Resumo: " << outBoard.users().size() << " usuários, " 
              << outBoard.cards().size() << " cartões, " 
              << outBoard.columns().size() << " colunas" << std::endl;
}

} // namespace kanban