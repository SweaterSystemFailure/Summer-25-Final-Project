#include "Gradebook.h"
#include "Administrator.h"
#include "Student.h"
#include "Teacher.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <unordered_map>

namespace gradebook {

    // === Accessors ===

    /**
     * @brief Gets a modifiable reference to the vector of administrators.
     * @return Reference to vector of Administrator objects.
     */
    std::vector<Administrator>& Gradebook::getSchool() {
        return school;
    }

    /**
     * @brief Gets a modifiable reference to the vector of teachers.
     * @return Reference to vector of Teacher objects.
     */
    std::vector<Teacher>& Gradebook::getTeachers() {
        return teachers;
    }

    /**
     * @brief Gets a const reference to the vector of teachers.
     * @return Const reference to vector of Teacher objects.
     */
    const std::vector<Teacher>& Gradebook::getTeachers() const {
        return teachers;
    }

    /**
     * @brief Gets a modifiable reference to the vector of students.
     * @return Reference to vector of unique pointers to Student objects.
     */
    std::vector<std::unique_ptr<Student>>& Gradebook::getStudents() {
        return students;
    }

    /**
     * @brief Gets a const reference to the vector of students.
     * @return Const reference to vector of unique pointers to Student objects.
     */
    const std::vector<std::unique_ptr<Student>>& Gradebook::getStudents() const {
        return students;
    }

    /**
     * @brief Returns whether autosave is currently enabled.
     * @return True if autosave is enabled; false otherwise.
     */
    bool Gradebook::isAutosaveEnabled() const {
        return autosaveEnabled;
    }

    // === Admin Function ===

    /**
     * @brief Creates a new school setup with an administrator.
     * If a school already exists, prompts to overwrite it.
     */
    void Gradebook::createSchool() {
        if (!school.empty()) {
            std::cout << "A school and administrator already exist in the system." << std::endl;
            std::cout << "School: " << school.front().getSchoolName() << std::endl;
            std::cout << "Administrator: " << school.front().getFirstName() << " " << school.front().getLastName() << std::endl;

            if (!userCheck("Would you like to overwrite this school and administrator? [Y/N] ",
                "Overwriting existing school and administrator.",
                "Keeping existing data.")) {
                return;
            }

            school.clear();
        }

        Administrator myAdmin;
        std::string password;

        std::cout << "Let's get started by entering some basic information." << std::endl;

        do {
            myAdmin.setAdminTitle(stringValidator("Which honorific are you addressed by? "));
            myAdmin.setFirstName(stringValidator("What is your first name? "));
            myAdmin.setLastName(stringValidator("What is your last name? "));
            myAdmin.setSchoolName(stringValidator("What is the name of your school? "));
        } while (!userCheck("Does this look right to you? [Y/N] ",
            "Great! Let's continue.",
            "That's okay. Let's try again."));

        do {
            password = stringValidator("Please select a strong password that is at least 8 characters long, and that contains a mix of upper and lower case characters, numbers, and special symbols. ");
        } while (!isStrongPassword(password));

        myAdmin.setPassword(password);

        school.push_back(myAdmin);

        std::cout << "School and administrator successfully created." << std::endl;
    }

    // === Autosave Toggle ===

    /**
     * @brief Toggles the autosave feature on or off.
     * Prints the current status after toggling.
     */
    void Gradebook::autosaveToggle() {
        autosaveEnabled = !autosaveEnabled;
        std::cout << "Autosave " << (autosaveEnabled ? "enabled." : "disabled.") << std::endl;
    }

    // === Serialization (Save) ===

    /**
     * @brief Serializes and saves the gradebook data to a binary file ("gradebook.dat").
     * Saves administrators, students (with assignment scores), and teachers (with classroom assignments and assignments).
     */
    void Gradebook::serializeAndSave() {
        std::ofstream outFile("gradebook.dat", std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to open file for saving.\n";
            return;
        }

        // Helper lambdas to write various types
        auto writeString = [&](const std::string& str) {
            size_t len = str.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(str.c_str(), len);
            };
        auto writeUnsigned = [&](unsigned val) {
            outFile.write(reinterpret_cast<char*>(&val), sizeof(val));
            };
        auto writeFloat = [&](float val) {
            outFile.write(reinterpret_cast<char*>(&val), sizeof(val));
            };
        auto writeChar = [&](char c) {
            outFile.write(reinterpret_cast<char*>(&c), sizeof(c));
            };

        // --- Save administrators ---
        size_t adminCount = school.size();
        writeUnsigned((unsigned)adminCount);
        for (const auto& admin : school) {
            writeString(admin.getAdminTitle());
            writeString(admin.getFirstName());
            writeString(admin.getLastName());
            writeString(admin.getSchoolName());
            writeString(admin.getPassword());
        }

        // --- Save students ---
        size_t studentCount = students.size();
        writeUnsigned((unsigned)studentCount);
        for (const auto& studentPtr : students) {
            const Student& student = *studentPtr;

            writeString(student.getFirstName());
            writeString(student.getLastName());
            writeString(student.getPronouns());
            writeUnsigned(student.getAge());
            writeUnsigned(student.getGradeLevel());
            writeUnsigned(student.getID());
            writeString(student.getSeat());
            writeString(student.getNotes());
            writeChar(student.getOverallGrade());
            writeFloat(student.getGradePercent());

            // Assignment scores
            const auto& scores = student.getAssignmentScores();
            writeUnsigned((unsigned)scores.size());
            for (const auto& [name, score] : scores) {
                writeString(name);
                writeFloat(score);
            }
        }

        // --- Save teachers ---
        size_t teacherCount = teachers.size();
        writeUnsigned((unsigned)teacherCount);
        for (const auto& teacher : teachers) {
            writeString(teacher.getTitle());
            writeString(teacher.getFirstName());
            writeString(teacher.getLastName());
            writeUnsigned(teacher.getGradeLevel());
            writeString(teacher.getPassword());

            // Classroom students to write IDs
            const auto& cls = teacher.getClassroomStudents();
            writeUnsigned((unsigned)cls.size());
            for (const auto* sPtr : cls) {
                writeUnsigned(sPtr->getID());
            }

            // Assignments
            const auto& assigns = teacher.getAssignments();
            writeUnsigned((unsigned)assigns.size());
            for (const auto& a : assigns) {
                writeString(a.getAssignmentName());
                writeFloat(a.getPointsPossible());
            }
        }

        outFile.close();
        std::cout << "Gradebook data saved to gradebook.dat.\n";
    }

    // === Deserialization (Load) ===

    /**
     * @brief Loads and deserializes gradebook data from the binary file ("gradebook.dat").
     * Clears existing data and populates administrators, students, and teachers accordingly.
     */
    void Gradebook::deserializeAndLoad() {
        std::ifstream inFile("gradebook.dat", std::ios::binary);
        if (!inFile) {
            std::cerr << "Failed to load gradebook.dat.\n";
            return;
        }

        // Clear existing data before loading
        school.clear();
        students.clear();
        teachers.clear();

        // Helper lambdas to read various types
        auto readString = [&](std::ifstream& f) {
            size_t len = 0;
            f.read(reinterpret_cast<char*>(&len), sizeof(len));
            std::string s(len, '\0');
            f.read(&s[0], len);
            return s;
            };
        auto readUnsigned = [&](std::ifstream& f) {
            unsigned v = 0;
            f.read(reinterpret_cast<char*>(&v), sizeof(v));
            return v;
            };
        auto readFloat = [&](std::ifstream& f) {
            float v = 0.0f;
            f.read(reinterpret_cast<char*>(&v), sizeof(v));
            return v;
            };
        auto readChar = [&](std::ifstream& f) {
            char c = '\0';
            f.read(reinterpret_cast<char*>(&c), sizeof(c));
            return c;
            };

        // --- Load administrators ---
        unsigned adminCount = readUnsigned(inFile);
        for (unsigned i = 0; i < adminCount; ++i) {
            Administrator admin;
            admin.setAdminTitle(readString(inFile));
            admin.setFirstName(readString(inFile));
            admin.setLastName(readString(inFile));
            admin.setSchoolName(readString(inFile));
            admin.setPassword(readString(inFile));
            school.push_back(std::move(admin));
        }

        // --- Load students ---
        unsigned studentCount = readUnsigned(inFile);
        students.reserve(studentCount);
        std::unordered_map<unsigned, Student*> idToStudentPtr;

        for (unsigned i = 0; i < studentCount; ++i) {
            auto s = std::make_unique<Student>();
            s->setFirstName(readString(inFile));
            s->setLastName(readString(inFile));
            s->setPronouns(readString(inFile));
            s->setAge(readUnsigned(inFile));
            s->setGradeLevel(readUnsigned(inFile));
            s->setID(readUnsigned(inFile));
            s->setSeat(readString(inFile));
            s->setNotes(readString(inFile));
            s->setOverallGrade(readChar(inFile));
            s->setGradePercent(readFloat(inFile));

            unsigned mapSize = readUnsigned(inFile);
            for (unsigned j = 0; j < mapSize; ++j) {
                std::string nm = readString(inFile);
                float sc = readFloat(inFile);
                s->setAssignmentScore(nm, sc);
            }

            unsigned id = s->getID();
            idToStudentPtr[id] = s.get();
            students.push_back(std::move(s));
        }

        // --- Load teachers ---
        unsigned teacherCount = readUnsigned(inFile);
        for (unsigned i = 0; i < teacherCount; ++i) {
            Teacher t;
            t.setTitle(readString(inFile));
            t.setFirstName(readString(inFile));
            t.setLastName(readString(inFile));
            t.setGradeLevel(readUnsigned(inFile));
            t.setPassword(readString(inFile));

            // Load student IDs assigned to teacher's classroom
            unsigned clsSize = readUnsigned(inFile);
            std::vector<unsigned> studentIDs(clsSize);
            for (unsigned j = 0; j < clsSize; ++j) {
                studentIDs[j] = readUnsigned(inFile);
            }

            // Load assignments for the teacher
            unsigned asz = readUnsigned(inFile);
            for (unsigned j = 0; j < asz; ++j) {
                Assignment a;
                a.setAssignmentName(readString(inFile));
                a.setPointsPossible(readFloat(inFile));
                t.getAssignments().push_back(a);
            }

            teachers.push_back(std::move(t));
            Teacher& refTeacher = teachers.back();

            // Link students to teacher's classroom by ID lookup
            for (unsigned id : studentIDs) {
                if (idToStudentPtr.count(id)) {
                    refTeacher.addStudentToClassroom(idToStudentPtr[id]);
                }
            }
        }

        inFile.close();
        std::cout << "Gradebook data loaded from gradebook.dat.\n";
    }

    // === Clear Cached Data ===

    /**
     * @brief Clears all cached data in memory for teachers, students, and administrators.
     */
    void Gradebook::clearCachedData() {
        teachers.clear();
        students.clear();
        school.clear();
        std::cout << "All cached data cleared from memory.\n";
    }
}