#include "User.h"

namespace kanban {

User::User(Id id, std::string name, std::string email)
    : m_id(id), m_name(std::move(name)), m_email(std::move(email)) {
}

User::Id User::id() const noexcept {
    return m_id;
}

const std::string& User::name() const noexcept {
    return m_name;
}

void User::setName(std::string name) {
    m_name = std::move(name);
}

const std::string& User::email() const noexcept {
    return m_email;
}

void User::setEmail(std::string email) {
    m_email = std::move(email);
}

bool User::operator==(const User& other) const noexcept {
    return m_id == other.m_id;
}

} // namespace kanban