#include "Gradebook.h"
#include "Administrator.h"
#include "Student.h"
#include "Teacher.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>


namespace gradebook {
    //Accessors
    std::vector<Administrator>& Gradebook::getSchool() {
        return school;
    }
    std::vector<Teacher>& Gradebook::getTeachers() {
        return teachers;
    }

    const std::vector<Teacher>& Gradebook::getTeachers() const {
        return teachers;
    }

    std::vector<Student>& Gradebook::getStudents() {
        return students;
    }
    const std::vector<Student>& Gradebook::getStudents() const {
        return students;
    }

    bool Gradebook::isAutosaveEnabled() const {
        return autosaveEnabled;
    }

    //Admin function
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

    //Save/Load Functions
    void Gradebook::autosaveToggle() {
        autosaveEnabled = !autosaveEnabled;
        std::cout << "Autosave " << (autosaveEnabled ? "enabled." : "disabled.") << std::endl;
    }

    void Gradebook::serializeAndSave() {
        std::ofstream outFile("gradebook.dat", std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to open file for saving.\n";
            return;
        }

        // Helper lambdas
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
        outFile.write(reinterpret_cast<char*>(&adminCount), sizeof(adminCount));
        for (const auto& admin : school) {
            writeString(admin.getAdminTitle());
            writeString(admin.getFirstName());
            writeString(admin.getLastName());
            writeString(admin.getSchoolName());
            writeString(admin.getPassword());
        }

        // --- Save students ---
        size_t studentCount = students.size();
        outFile.write(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
        for (const auto& student : students) {
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

            // Assignment scores map
            const auto& scores = student.getAssignmentScores();
            size_t mapSize = scores.size();
            outFile.write(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
            for (const auto& [name, score] : scores) {
                writeString(name);
                writeFloat(score);
            }
        }

        // --- Save teachers ---
        size_t teacherCount = teachers.size();
        outFile.write(reinterpret_cast<char*>(&teacherCount), sizeof(teacherCount));
        for (const auto& teacher : teachers) {
            writeString(teacher.getTitle());
            writeString(teacher.getFirstName());
            writeString(teacher.getLastName());
            writeUnsigned(teacher.getGradeLevel());
            writeString(teacher.getPassword());

            // Classroom students (just names)
            const auto& cls = teacher.getClassroomStudents();
            size_t clsSize = cls.size();
            outFile.write(reinterpret_cast<const char*>(&clsSize), sizeof(clsSize));
            for (const auto* sPtr : cls) {
                writeString(sPtr->getFirstName());
                writeString(sPtr->getLastName());
            }

            // Assignments
            const auto& assigns = teacher.getAssignments();
            size_t asz = assigns.size();
            outFile.write(reinterpret_cast<char*>(&asz), sizeof(asz));
            for (const auto& a : assigns) {
                writeString(a.getAssignmentName());
                writeFloat(a.getPointsPossible());
            }
        }

        outFile.close();
        std::cout << "Gradebook data saved to gradebook.dat.\n";
    }

    void Gradebook::deserializeAndLoad() {
        std::ifstream inFile("gradebook.dat", std::ios::binary);
        if (!inFile) {
            std::cerr << "Failed to load gradebook.dat.\n";
            return;
        }

        school.clear();
        students.clear();
        teachers.clear();

        auto readString = [&](std::ifstream& file) {
            size_t len = 0;
            file.read(reinterpret_cast<char*>(&len), sizeof(len));
            std::string str(len, '\0');
            file.read(&str[0], len);
            return str;
            };

        auto readUnsigned = [&](std::ifstream& file) {
            unsigned val = 0;
            file.read(reinterpret_cast<char*>(&val), sizeof(val));
            return val;
            };

        auto readFloat = [&](std::ifstream& file) {
            float val = 0.0f;
            file.read(reinterpret_cast<char*>(&val), sizeof(val));
            return val;
            };

        auto readChar = [&](std::ifstream& file) {
            char c = '\0';
            file.read(reinterpret_cast<char*>(&c), sizeof(c));
            return c;
            };

        // --- Load administrators ---
        size_t adminCount = 0;
        inFile.read(reinterpret_cast<char*>(&adminCount), sizeof(adminCount));
        for (size_t i = 0; i < adminCount; ++i) {
            Administrator admin;
            admin.setAdminTitle(readString(inFile));
            admin.setFirstName(readString(inFile));
            admin.setLastName(readString(inFile));
            admin.setSchoolName(readString(inFile));
            admin.setPassword(readString(inFile));
            school.push_back(std::move(admin));
        }

        // --- Load students ---
        size_t studentCount = 0;
        inFile.read(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
        for (size_t i = 0; i < studentCount; ++i) {
            Student s;
            s.setFirstName(readString(inFile));
            s.setLastName(readString(inFile));
            s.setPronouns(readString(inFile));
            s.setAge(readUnsigned(inFile));
            s.setGradeLevel(readUnsigned(inFile));
            s.setID(readUnsigned(inFile));
            s.setSeat(readString(inFile));
            s.setNotes(readString(inFile));
            s.setOverallGrade(readChar(inFile));
            s.setGradePercent(readFloat(inFile));

            // Assignment scores
            size_t mapSize = 0;
            inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
            for (size_t j = 0; j < mapSize; ++j) {
                std::string assignName = readString(inFile);
                float score = readFloat(inFile);
                s.setAssignmentScore(assignName, score);
            }

            students.push_back(std::move(s));
        }

        // --- Load teachers ---
        size_t teacherCount = 0;
        inFile.read(reinterpret_cast<char*>(&teacherCount), sizeof(teacherCount));
        for (size_t i = 0; i < teacherCount; ++i) {
            Teacher t;
            t.setTitle(readString(inFile));
            t.setFirstName(readString(inFile));
            t.setLastName(readString(inFile));
            t.setGradeLevel(readUnsigned(inFile));
            t.setPassword(readString(inFile));

            // Students
            size_t clsSize = 0;
            inFile.read(reinterpret_cast<char*>(&clsSize), sizeof(clsSize));
            for (size_t j = 0; j < clsSize; ++j) {
                Student* sPtr = new Student();
                sPtr->setFirstName(readString(inFile));
                sPtr->setLastName(readString(inFile));
                t.addStudentToClassroom(sPtr);
            }

            // Assignments
            size_t asz = 0;
            inFile.read(reinterpret_cast<char*>(&asz), sizeof(asz));
            for (size_t j = 0; j < asz; ++j) {
                Assignment a;
                a.setAssignmentName(readString(inFile));
                a.setPointsPossible(readFloat(inFile));
                t.getAssignments().push_back(a);
            }

            teachers.push_back(std::move(t));
        }

        inFile.close();
        std::cout << "Gradebook data loaded from gradebook.dat.\n";
    }

    void Gradebook::clearCachedData() {
        teachers.clear();
        students.clear();
        school.clear();
        std::cout << "All cached data cleared from memory.\n";
    }
}