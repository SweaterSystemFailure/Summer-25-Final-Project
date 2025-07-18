#pragma once
#include "User.h"
#include <iostream>

namespace gradebook{
    class Administrator : public User
    {
        private:
        std::string adminTitle;
        std::string adminFirstName;
        std::string adminLastName;
        std::string schoolName;
        std::string password;
        

        public:
        //Mutators
        void setAdminTitle(const std::string& entry);
        void setAdminFirstName(const std::string& entry);
        void setAdminLastName(const std::string& entry);
        void setSchoolName(const std::string& entry);
        void setPassword(const std::string& entry);
       

        //Accessors
        std::string getAdminTitle() const;
        std::string getFirstName() const;
        std::string getLastName() const override;
        std::string getSchoolName() const;
        std::string getPassword() const override;

        //Add Students & Teachers
        void createClassroom(Gradebook& gradebook);
        void addStudent (Gradebook& gradebook);

        //Print
        void printSchoolReport(Gradebook& gradebook) const;
        void saveSchoolReportToCSV(Gradebook& gradebook);

        //Menu
        void menu(Gradebook& book) override;
        std::string getRole() const override { return "Administrator"; }
    };
}