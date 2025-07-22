#pragma once
#include <string>
#include "User.h"

namespace gradebook {

    class Administrator : public User {
    private:
        std::string adminTitle;  ///< Administrator's title (e.g., Mr., Mrs., Dr.)
        std::string schoolName;  ///< Name of the school administrator manages

    public:
        // === Mutators ===

        /**
         * @brief Sets the administrator's title.
         * @param entry The new title to assign (e.g., Mr., Mrs., Dr.).
         */
        void setAdminTitle(const std::string& entry);

        /**
         * @brief Sets the name of the school.
         * @param entry The school name to assign.
         */
        void setSchoolName(const std::string& entry);

        // === Accessors ===

        /**
         * @brief Retrieves the administrator's title.
         * @return The current administrator title.
         */
        std::string getAdminTitle() const;

        /**
         * @brief Retrieves the name of the school.
         * @return The current school name.
         */
        std::string getSchoolName() const;

        // === Overrides for User base class pure virtual functions ===

        /**
         * @brief Gets the administrator's first name.
         * @return First name as a string.
         */
        std::string getFirstName() const override;

        /**
         * @brief Gets the administrator's last name.
         * @return Last name as a string.
         */
        std::string getLastName() const override;

        /**
         * @brief Gets the administrator's password.
         * @return Password string.
         */
        std::string getPassword() const override;

        /**
         * @brief Sets the administrator's password.
         * @param entry New password string to set.
         */
        void setPassword(const std::string& entry) override {
            password = entry;
        }

        /**
         * @brief Gets the role name of the user.
         * @return Always returns "Administrator".
         */
        std::string getRole() const override {
            return "Administrator";
        }

        // === Menu functions ===

        /**
         * @brief Main menu function for administrator interaction.
         * @param gradebook Reference to the gradebook instance.
         */
        void menu(Gradebook& gradebook) override;

        // === Additional Administrator member functions ===

        /**
         * @brief Creates a new classroom within the gradebook.
         * @param gradebook Reference to the gradebook instance.
         */
        void createClassroom(Gradebook& gradebook);

        /**
         * @brief Adds a new student to the gradebook.
         * @param gradebook Reference to the gradebook instance.
         */
        void addStudent(Gradebook& gradebook);

        /**
         * @brief Prints a school-wide report of students.
         * @param gradebook Reference to the gradebook instance.
         */
        void printSchoolReport(Gradebook& gradebook) const;

        /**
         * @brief Saves the school report to a CSV file.
         * @param gradebook Reference to the gradebook instance.
         */
        void saveSchoolReportToCSV(Gradebook& gradebook);
    };
}
