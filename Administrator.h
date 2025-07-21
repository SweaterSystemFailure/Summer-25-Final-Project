#pragma once
#include <string>
#include "User.h"
#include "Gradebook.h"

namespace gradebook {

    class Administrator : public User {
    private:
        std::string adminTitle;
        std::string schoolName;

    public:
        // Constructors / Destructor if needed
        Administrator() = default;
        ~Administrator() override = default;

        // Setters for Administrator-specific members
        void setAdminTitle(const std::string& entry);
        void setSchoolName(const std::string& entry);

        // Getters for Administrator-specific members
        std::string getAdminTitle() const;
        std::string getSchoolName() const;

        // Override base class pure virtual functions
        std::string getFirstName() const override;
        std::string getLastName() const override;
        std::string getPassword() const override;

        void setPassword(const std::string& entry) override {
            password = entry;
        }

        std::string getRole() const override {
            return "Administrator";
        }

        void menu(Gradebook& gradebook) override;

        // Other Administrator member functions declarations
        void createClassroom(Gradebook& gradebook);
        void addStudent(Gradebook& gradebook);
        void printSchoolReport(Gradebook& gradebook) const;
        void saveSchoolReportToCSV(Gradebook& gradebook);
    };
}