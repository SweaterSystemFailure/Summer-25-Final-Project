#include "Gradebook.h"
#include "Administrator.h"
#include "Student.h"
#include "Teacher.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <iomanip>


namespace gradebook {
    //Accessors
    std::vector<Teacher>& Gradebook::getTeachers() {
        return teachers;
    }

    const std::vector<Teacher>& Gradebook::getTeachers() const {
        return teachers;
    }

    std::vector<Student>& Gradebook::getStudents() {
        return students;
    }
    const std::vector<Student>& Gradebook::getStudents() const; {
        return students;
    }

    bool Gradebook::getAutosaveEnabled() const {
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
            myAdmin.setAdminFirstName(stringValidator("What is your first name? "));
            myAdmin.setAdminLastName(stringValidator("What is your last name? "));
            myAdmin.setSchoolName(stringValidator("What is the name of your school? "));
        } while (!userCheck("Does this look right to you? [Y/N] ",
            "Great! Let's continue.",
            "That's okay. Let's try again."));

        do {
            password = stringValidator("Please select a strong password that is at least 8 characters long, and that contains a mix of upper and lower case characters, numbers, and special symbols.");
        } while (!isStrongPassword(password));

        myAdmin.setPassword(password);

        school.push_back(myAdmin);

        std::cout << "School and administrator successfully created." << std::endl;
    }


    //Login Functions
    bool handlePassword(User& user) {
        std::string enteredPassword;

        if (user.getPassword().empty()) {
            std::cout << "No password set for your account. Please create one now." << std::endl;
            do {
                enteredPassword = stringValidator("Enter new password: ");
            } while (!isStrongPassword(enteredPassword));

            user.setPassword(enteredPassword);
            std::cout << "Password set successfully." << std::endl;
            return true;
        }
        else {
            enteredPassword = stringValidator("Enter your password: ");
            if (enteredPassword == user.getPassword()) {
                std::cout << "Password accepted." << std::endl;
                return true;
            }
            else {
                std::cout << "Incorrect password." << std::endl;
                return false;
            }
        }
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

        // Save number of administrators
        size_t adminCount = school.size();
        outFile.write(reinterpret_cast<char*>(&adminCount), sizeof(adminCount));
        for (const auto& admin : school) {
            size_t len;

            len = admin.getAdminTitle().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(admin.getAdminTitle().c_str(), len);

            len = admin.getFirstName().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(admin.getFirstName().c_str(), len);

            len = admin.getLastName().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(admin.getLastName().c_str(), len);

            len = admin.getSchoolName().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(admin.getSchoolName().c_str(), len);

            len = admin.getPassword().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(admin.getPassword().c_str(), len);
        }

        // Save number of students
        size_t studentCount = students.size();
        outFile.write(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
        for (const auto& student : students) {
            size_t len;

            len = student.getFirstName().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(student.getFirstName().c_str(), len);

            len = student.getLastName().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(student.getLastName().c_str(), len);

            len = student.getPronouns().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(student.getPronouns().c_str(), len);

            outFile.write(reinterpret_cast<const char*>(&student.getAge()), sizeof(unsigned));
            outFile.write(reinterpret_cast<const char*>(&student.getGradeLevel()), sizeof(unsigned));
            outFile.write(reinterpret_cast<const char*>(&student.getID()), sizeof(unsigned));

            len = student.getSeat().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(student.getSeat().c_str(), len);

            len = student.getNotes().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(student.getNotes().c_str(), len);

            outFile.write(reinterpret_cast<const char*>(&student.getOverallGrade()), sizeof(char));
            outFile.write(reinterpret_cast<const char*>(&student.getGradePercent()), sizeof(float));

            // Save assignment scores
            const auto& scores = student.getAssignmentScores();
            size_t mapSize = scores.size();
            outFile.write(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
            for (const auto& [name, score] : scores) {
                len = name.size();
                outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
                outFile.write(name.c_str(), len);
                outFile.write(reinterpret_cast<const char*>(&score), sizeof(float));
            }
        }

        // Save number of teachers
        size_t teacherCount = teachers.size();
        outFile.write(reinterpret_cast<char*>(&teacherCount), sizeof(teacherCount));
        for (const auto& teacher : teachers) {
            size_t len;

            len = teacher.getTitle().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(teacher.getTitle().c_str(), len);

            len = teacher.getFirstName().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(teacher.getFirstName().c_str(), len);

            len = teacher.getLastName().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(teacher.getLastName().c_str(), len);

            outFile.write(reinterpret_cast<const char*>(&teacher.getGradeLevel()), sizeof(unsigned));

            len = teacher.getPassword().size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(teacher.getPassword().c_str(), len);

            // Save classroom students
            const auto& classroom = teacher.getClassroomStudents();
            size_t classSize = classroom.size();
            outFile.write(reinterpret_cast<char*>(&classSize), sizeof(classSize));
            for (const auto& student : classroom) {
                len = student.getFirstName().size();
                outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
                outFile.write(student.getFirstName().c_str(), len);

                len = student.getLastName().size();
                outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
                outFile.write(student.getLastName().c_str(), len);
            }

            // Save assignments (by name only)
            const auto& assignments = teacher.getAssignments();
            size_t assignSize = assignments.size();
            outFile.write(reinterpret_cast<char*>(&assignSize), sizeof(assignSize));
            for (const auto& assignment : assignments) {
                len = assignment.getAssignmentName().size();
                outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
                outFile.write(assignment.getAssignmentName().c_str(), len);
                float totalPoints = assignment.getTotalPoints();
                outFile.write(reinterpret_cast<const char*>(&totalPoints), sizeof(float));
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

        size_t count;
        size_t len;
        std::string tempStr;

        // === Load Administrators ===
        inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
        for (size_t i = 0; i < count; ++i) {
            Administrator admin;

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); admin.setAdminTitle(tempStr);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); admin.setAdminFirstName(tempStr);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); admin.setAdminLastName(tempStr);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); admin.setSchoolName(tempStr);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); admin.setPassword(tempStr);

            school.push_back(admin);
        }

        // === Load Students ===
        inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
        for (size_t i = 0; i < count; ++i) {
            Student student;

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); student.setFirstName(tempStr);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); student.setLastName(tempStr);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); student.setPronouns(tempStr);

            unsigned tempUInt;
            inFile.read(reinterpret_cast<char*>(&tempUInt), sizeof(tempUInt)); student.setAge(tempUInt);
            inFile.read(reinterpret_cast<char*>(&tempUInt), sizeof(tempUInt)); student.setGradeLevel(tempUInt);
            inFile.read(reinterpret_cast<char*>(&tempUInt), sizeof(tempUInt)); student.setID(tempUInt);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); student.setSeat(tempStr);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); student.setNotes(tempStr);

            char grade;
            float percent;
            inFile.read(reinterpret_cast<char*>(&grade), sizeof(grade)); student.setOverallGrade(grade);
            inFile.read(reinterpret_cast<char*>(&percent), sizeof(percent)); student.setGradePercent(percent);

            size_t mapSize;
            inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
            for (size_t j = 0; j < mapSize; ++j) {
                float score;
                inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
                tempStr.resize(len); inFile.read(&tempStr[0], len);
                inFile.read(reinterpret_cast<char*>(&score), sizeof(score));
                student.setAssignmentScore(tempStr, score);
            }

            students.push_back(student);
        }

        // === Load Teachers ===
        inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
        for (size_t i = 0; i < count; ++i) {
            Teacher teacher;

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); teacher.setTitle(tempStr);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); teacher.setTeacherFirstName(tempStr);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); teacher.setTeacherLastName(tempStr);

            unsigned grade;
            inFile.read(reinterpret_cast<char*>(&grade), sizeof(grade)); teacher.setGradeLevel(grade);

            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            tempStr.resize(len); inFile.read(&tempStr[0], len); teacher.setPassword(tempStr);

            // Load classroom students (name only)
            size_t classSize;
            inFile.read(reinterpret_cast<char*>(&classSize), sizeof(classSize));
            for (size_t j = 0; j < classSize; ++j) {
                Student student;
                inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
                tempStr.resize(len); inFile.read(&tempStr[0], len); student.setFirstName(tempStr);

                inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
                tempStr.resize(len); inFile.read(&tempStr[0], len); student.setLastName(tempStr);

                teacher.addStudentToClassroom(student);
            }

            size_t assignCount;
            inFile.read(reinterpret_cast<char*>(&assignCount), sizeof(assignCount));
            for (size_t j = 0; j < assignCount; ++j) {
                Assignment a;
                float points;
                inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
                tempStr.resize(len); inFile.read(&tempStr[0], len); a.setAssignmentName(tempStr);
                inFile.read(reinterpret_cast<char*>(&points), sizeof(points));
                a.setTotalPoints(points);
                teacher.addAssignment(a);
            }

            teachers.push_back(teacher);
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


   