#include "Administrator.h"

namespace gradebook{
// Mutators
void Administrator::setAdminTitle(const std::string& entry) {
    adminTitle = entry;
}

void Administrator::setAdminLastName(const std::string& entry) {
    adminLastName = entry;
}

void Administrator::setSchoolName(const std::string& entry) {
    schoolName = entry;
}

void Administrator::setAdminPassword(const std::string& entry) {
    adminPassword = entry;
}

// Accessors
std::string Administrator::getAdminTitle() const {
    return adminTitle;
}

std::string Administrator::getAdminLastName() const {
    return adminLastName;
}

std::string Administrator::getSchoolName() const {
    return schoolName;
}

std::string Administrator::getAdminPassword() const {
    return adminPassword;
}
}
