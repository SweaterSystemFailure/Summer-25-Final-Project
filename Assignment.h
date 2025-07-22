#pragma once
#include <iostream>

namespace gradebook {
    class Assignment {
    private:
        std::string assignmentName;
        std::string assignmentDescription;
        float pointsPossible;

    public:
        // === Mutators ===

        /**
         * @brief Sets the assignment's name.
         * @param entry New assignment name to assign.
         */
        void setAssignmentName(const std::string& entry);

        /**
         * @brief Sets the assignment's description.
         * @param entry New description text to assign.
         */
        void setAssignmentDescription(const std::string& entry);

        /**
         * @brief Sets the total points possible for the assignment.
         * @param entry Points possible as a float.
         */
        void setPointsPossible(float entry);

        // === Accessors ===

        /**
         * @brief Retrieves the assignment's name.
         * @return Current assignment name.
         */
        std::string getAssignmentName() const;

        /**
         * @brief Retrieves the assignment's description.
         * @return Current assignment description.
         */
        std::string getAssignmentDescription() const;

        /**
         * @brief Retrieves the points possible for the assignment.
         * @return Points possible as a float.
         */
        float getPointsPossible() const;
    };
}
