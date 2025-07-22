#pragma once
#include <string>
#include <iostream>

namespace gradebook {

    class Gradebook;

    /**
     * @brief Abstract base class representing a user in the gradebook system.
     *
     * This class provides the basic interface and data members for any user
     * type (e.g., Student, Teacher, Administrator). It is meant to be inherited from.
     */
    class User {
    protected:
        std::string firstName;   ///< User's first name.
        std::string lastName;    ///< User's last name.
        std::string password;    ///< User's password.

    public:
        /// Virtual destructor for proper cleanup of derived classes.
        virtual ~User() = default;

        // === Mutators ===

        /**
         * @brief Sets the user's password.
         * @param entry The new password to assign.
         */
        virtual void setPassword(const std::string& entry) {
            password = entry;
        }

        /**
         * @brief Sets the user's first name.
         * @param fn The new first name.
         */
        void setFirstName(const std::string& fn) { firstName = fn; }

        /**
         * @brief Sets the user's last name.
         * @param ln The new last name.
         */
        void setLastName(const std::string& ln) { lastName = ln; }

        // === Pure Virtual Interface ===

        /**
         * @brief Launches the interactive menu for the user.
         *
         * This function must be implemented by derived classes to handle
         * user-specific actions in the gradebook system.
         *
         * @param gradebook Reference to the Gradebook object for access to system data.
         */
        virtual void menu(Gradebook& gradebook) = 0;

        /**
         * @brief Retrieves the user's role (e.g., "Student", "Teacher", "Administrator").
         * @return The role as a string.
         */
        virtual std::string getRole() const = 0;

        /**
         * @brief Gets the user's first name.
         * @return The user's first name.
         */
        virtual std::string getFirstName() const = 0;

        /**
         * @brief Gets the user's last name.
         * @return The user's last name.
         */
        virtual std::string getLastName() const = 0;

        /**
         * @brief Gets the user's password.
         * @return The user's password.
         */
        virtual std::string getPassword() const = 0;
    };
}