#pragma once
#include "Teacher.h"
#include <iostream>

namespace gradebook{
    class Administrator : public Classroom
    {
        private:
        std::string adminTitle;
        std::string adminLastName;
        std::string schoolName;
        std::string adminPassword;
        
        public:
        //Mutators
        void setAdminTitle(const std::string& entry);
        void setAdminLastName(const std::string& entry);
        void setSchoolName(const std::string& entry);
        void setAdminPassword(const std::string& entry);

        //Accessors
        std::string getAdminTitle() const;
        std::string getAdminLastName() const override;
        std::string getSchoolName() const;
        std::string getAdminPassword() const override;

        //Print
        void printSchoolReport() const;

        //Menu
        void menu(Gradebook& book) override;
        std::string getRole() const override { return "Administrator"; }
    };
}