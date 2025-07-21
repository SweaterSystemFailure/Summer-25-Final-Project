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
            myAdmin.setFirstName(stringValidator("What is your first name? "));
            myAdmin.setLastName(stringValidator("What is your last name? "));
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
    bool Gradebook::handlePassword(User& user) {
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

        // --- Save administrators ---
        size_t adminCount = school.size();
        outFile.write(reinterpret_cast<char*>(&adminCount), sizeof(adminCount));
        for (const auto& admin : school) {
            size_t len;

            // Title
            std::string title = admin.getAdminTitle();
            len = title.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(title.c_str(), len);

            // First name
            std::string first = admin.getFirstName();
            len = first.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(first.c_str(), len);

            // Last name
            std::string last = admin.getLastName();
            len = last.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(last.c_str(), len);

            // School name
            std::string schoolName = admin.getSchoolName();
            len = schoolName.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(schoolName.c_str(), len);

            // Password
            std::string pwd = admin.getPassword();
            len = pwd.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(pwd.c_str(), len);
        }

        // --- Save students ---
        size_t studentCount = students.size();
        outFile.write(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
        for (const auto& student : students) {
            size_t len;

            // First name
            std::string fn = student.getFirstName();
            len = fn.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(fn.c_str(), len);

            // Last name
            std::string ln = student.getLastName();
            len = ln.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(ln.c_str(), len);

            // Pronouns
            std::string pron = student.getPronouns();
            len = pron.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(pron.c_str(), len);

            // Age, grade level, ID
            unsigned age = student.getAge();
            unsigned grade = student.getGradeLevel();
            unsigned id = student.getID();
            outFile.write(reinterpret_cast<char*>(&age), sizeof(age));
            outFile.write(reinterpret_cast<char*>(&grade), sizeof(grade));
            outFile.write(reinterpret_cast<char*>(&id), sizeof(id));

            // Seat
            std::string seat = student.getSeat();
            len = seat.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(seat.c_str(), len);

            // Notes
            std::string notes = student.getNotes();
            len = notes.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(notes.c_str(), len);

            // Overall grade & percent
            char og = student.getOverallGrade();
            float gp = student.getGradePercent();
            outFile.write(reinterpret_cast<char*>(&og), sizeof(og));
            outFile.write(reinterpret_cast<char*>(&gp), sizeof(gp));

            // Assignment scores map
            const auto& scores = student.getAssignmentScores();
            size_t mapSize = scores.size();
            outFile.write(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
            for (const auto& [name, score] : scores) {
                len = name.size();
                outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
                outFile.write(name.c_str(), len);
                outFile.write(reinterpret_cast<const char*>(&score), sizeof(score));
            }
        }

        // --- Save teachers ---
        size_t teacherCount = teachers.size();
        outFile.write(reinterpret_cast<char*>(&teacherCount), sizeof(teacherCount));
        for (const auto& teacher : teachers) {
            size_t len;

            // Title
            std::string tt = teacher.getTitle();
            len = tt.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(tt.c_str(), len);

            // First name
            std::string tf = teacher.getFirstName();
            len = tf.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(tf.c_str(), len);

            // Last name
            std::string tl = teacher.getLastName();
            len = tl.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(tl.c_str(), len);

            // Grade level
            unsigned gl = teacher.getGradeLevel();
            outFile.write(reinterpret_cast<char*>(&gl), sizeof(gl));

            // Password
            std::string tp = teacher.getPassword();
            len = tp.size();
            outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
            outFile.write(tp.c_str(), len);

            // Classroom students
            const auto& cls = teacher.getClassroomStudents();
            size_t clsSize = cls.size();
            outFile.write(reinterpret_cast<char*>(&clsSize), sizeof(clsSize));
            for (const auto& s : cls) {
                std::string fn2 = s.getFirstName();
                len = fn2.size();
                outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
                outFile.write(fn2.c_str(), len);

                std::string ln2 = s.getLastName();
                len = ln2.size();
                outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
                outFile.write(ln2.c_str(), len);
            }

            // Assignments
            const auto& assigns = teacher.getAssignment();
            size_t asz = assigns.size();
            outFile.write(reinterpret_cast<char*>(&asz), sizeof(asz));
            for (const auto& a : assigns) {
                std::string an = a.getAssignmentName();
                len = an.size();
                outFile.write(reinterpret_cast<char*>(&len), sizeof(len));
                outFile.write(an.c_str(), len);

                float pts = a.getPointsPossible();
                outFile.write(reinterpret_cast<char*>(&pts), sizeof(pts));
            }
        }

        outFile.close();
        std::cout << "Gradebook data saved to gradebook.dat.\n";
    }

    void Gradebook::deserializeAndLoad() {
        std::ifstream inFile("gradebook.dat", std::ios::binary);
        if (!inFile) {
            std::cerr << "Failed to load gradebook.dat.";
                return;
        }

        // Clear current data
        school.clear();
        students.clear();
        teachers.clear();

        // --- Load administrators ---
        size_t adminCount = 0;
        inFile.read(reinterpret_cast<char*>(&adminCount), sizeof(adminCount));
        for (size_t i = 0; i < adminCount; ++i) {
            Administrator admin;
            size_t len;
            std::string str;

            // Title
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            admin.setAdminTitle(str);

            // First name
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            admin.setFirstName(str);

            // Last name
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            admin.setLastName(str);

            // School name
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            admin.setSchoolName(str);

            // Password
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            admin.setPassword(str);

            school.push_back(std::move(admin));
        }

        // --- Load students ---
        size_t studentCount = 0;
        inFile.read(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
        for (size_t i = 0; i < studentCount; ++i) {
            Student student;
            size_t len; unsigned u;
            std::string str;

            // First name
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            student.setFirstName(str);

            // Last name
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            student.setLastName(str);

            // Pronouns
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            student.setPronouns(str);

            // Age, grade, ID
            inFile.read(reinterpret_cast<char*>(&u), sizeof(u)); student.setAge(u);
            inFile.read(reinterpret_cast<char*>(&u), sizeof(u)); student.setGradeLevel(u);
            inFile.read(reinterpret_cast<char*>(&u), sizeof(u)); student.setID(u);

            // Seat
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            student.setSeat(str);

            // Notes
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            student.setNotes(str);

            // Overall grade & percent
            char c; float f;
            inFile.read(reinterpret_cast<char*>(&c), sizeof(c)); student.setOverallGrade(c);
            inFile.read(reinterpret_cast<char*>(&f), sizeof(f)); student.setGradePercent(f);

            // Assignment scores map
            size_t mapSize = 0;
            inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
            for (size_t j = 0; j < mapSize; ++j) {
                inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
                str.resize(len); inFile.read(&str[0], len);
                inFile.read(reinterpret_cast<char*>(&f), sizeof(f));
                student.setAssignmentScore(str, f);
            }

            students.push_back(std::move(student));
        }

        // --- Load teachers ---
        size_t teacherCount = 0;
        inFile.read(reinterpret_cast<char*>(&teacherCount), sizeof(teacherCount));
        for (size_t i = 0; i < teacherCount; ++i) {
            Teacher teacher;
            size_t len;
            unsigned u;
            std::string str;

            // Title
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            teacher.setTitle(str);

            // First name
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            teacher.setFirstName(str);

            // Last name
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            teacher.setLastName(str);

            // Grade level
            inFile.read(reinterpret_cast<char*>(&u), sizeof(u));
            teacher.setGradeLevel(u);

            // Password
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            str.resize(len); inFile.read(&str[0], len);
            teacher.setPassword(str);

            // Classroom students
            size_t clsSize = 0;
            inFile.read(reinterpret_cast<char*>(&clsSize), sizeof(clsSize));
            for (size_t j = 0; j < clsSize; ++j) {
                Student s;
                inFile.read(reinterpret_cast<char*>(&len), sizeof(len)); str.resize(len); inFile.read(&str[0], len); s.setFirstName(str);
                inFile.read(reinterpret_cast<char*>(&len), sizeof(len)); str.resize(len); inFile.read(&str[0], len); s.setLastName(str);
                teacher.addStudentToClassroom(s);
            }

            // Assignments
// …
            size_t asz = 0;
            inFile.read(reinterpret_cast<char*>(&asz), sizeof(asz));
            for (size_t j = 0; j < asz; ++j) {
                Assignment a;

                // read the assignment name
                inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
                str.resize(len);
                inFile.read(&str[0], len);
                a.setAssignmentName(str);

                // read the pointsPossible
                float points = 0.0f; 
                inFile.read(reinterpret_cast<char*>(&points), sizeof(points));
                a.setPointsPossible(points);

                teacher.addAssignment(a, *this);
            }


            teachers.push_back(std::move(teacher));
        }

        inFile.close();
        std::cout << "Gradebook data loaded from gradebook.dat.";
    }

    void Gradebook::clearCachedData() {
        teachers.clear();
        students.clear();
        school.clear();
        std::cout << "All cached data cleared from memory.\n";
    }
}