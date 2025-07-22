#include "Assignment.h"
#include <iostream>

namespace gradebook {
    // === Mutators ===

    /**
     * @brief Sets the assignment's name.
     * @param entry New assignment name to assign.
     */
    void Assignment::setAssignmentName(const std::string& entry) {
        assignmentName = entry;
    }

    /**
     * @brief Sets the assignment's description.
     * @param entry New description text to assign.
     */
    void Assignment::setAssignmentDescription(const std::string& entry) {
        assignmentDescription = entry;
    }

    /**
     * @brief Sets the total points possible for the assignment.
     * @param entry Points possible as a float.
     */
    void Assignment::setPointsPossible(float entry) {
        pointsPossible = entry;
    }

    // === Accessors ===

    /**
     * @brief Retrieves the assignment's name.
     * @return Current assignment name.
     */
    std::string Assignment::getAssignmentName() const {
        return assignmentName;
    }

    /**
     * @brief Retrieves the assignment's description.
     * @return Current assignment description.
     */
    std::string Assignment::getAssignmentDescription() const {
        return assignmentDescription;
    }

    /**
     * @brief Retrieves the points possible for the assignment.
     * @return Points possible as a float.
     */
    float Assignment::getPointsPossible() const {
        return pointsPossible;
    }
}