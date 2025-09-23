#pragma once

#include <string>
#include <cstdint>

namespace kanban {

/**
 * User: participante do Board.
 * Igualdade por id; e-mail opcionalmente validado na implementação.
 */
class User {
public:
    using Id = std::uint64_t;

    // Construtores
    User(Id id, std::string name, std::string email);
    User(const User&) = default;
    User(User&&) noexcept = default;
    User& operator=(const User&) = default;
    User& operator=(User&&) noexcept = default;
    ~User() = default;

    // Identidade
    Id id() const noexcept;

    // Nome
    const std::string& name() const noexcept;
    void setName(std::string name);

    // E-mail
    const std::string& email() const noexcept;
    void setEmail(std::string email);

    // Igualdade por id
    bool operator==(const User& other) const noexcept;

private:
    Id m_id;
    std::string m_name;
    std::string m_email;
};

} // namespace kanban
