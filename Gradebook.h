#pragma once
#include "Teacher.h"
#include "Student.h"
#include "Administrator.h"
#include "User.h"
#include <iostream>
#include <vector>
#include <map>

namespace gradebook {
    class Gradebook {
    private:
        std::vector<Teacher> teachers;                        /**< Vector storing Teacher objects */
        std::vector<std::unique_ptr<Student>> students;       /**< Vector storing unique pointers to Student objects */
        std::vector<Administrator> school;                    /**< Vector storing Administrator objects */
        bool autosaveEnabled = true;                          /**< Flag to control autosave feature */

    public:
        // === Accessors ===

        /**
         * @brief Gets a modifiable reference to the list of teachers.
         * @return Reference to vector of Teacher objects.
         */
        std::vector<Teacher>& getTeachers();

        /**
         * @brief Gets a const reference to the list of teachers.
         * @return Const reference to vector of Teacher objects.
         */
        const std::vector<Teacher>& getTeachers() const;

        /**
         * @brief Gets a modifiable reference to the list of students.
         * @return Reference to vector of unique pointers to Student objects.
         */
        std::vector<std::unique_ptr<Student>>& getStudents();

        /**
         * @brief Gets a const reference to the list of students.
         * @return Const reference to vector of unique pointers to Student objects.
         */
        const std::vector<std::unique_ptr<Student>>& getStudents() const;

        /**
         * @brief Gets a modifiable reference to the list of administrators.
         * @return Reference to vector of Administrator objects.
         */
        std::vector<Administrator>& getSchool();

        /**
         * @brief Checks if autosave is currently enabled.
         * @return True if autosave is enabled, false otherwise.
         */
        bool isAutosaveEnabled() const;

        // === Admin Functions ===

        /**
         * @brief Creates a new school setup by initializing administrators, teachers, and students.
         */
        void createSchool();

        // === Serialization Functions ===

        /**
         * @brief Serializes current gradebook data and saves it to disk in binary format.
         */
        void serializeAndSave();

        /**
         * @brief Loads gradebook data from a binary file and deserializes into current gradebook state.
         */
        void deserializeAndLoad();

        /**
         * @brief Toggles the autosave feature on or off.
         */
        void autosaveToggle();

        /**
         * @brief Clears any cached data currently held in the gradebook.
         */
        void clearCachedData();
    };
}