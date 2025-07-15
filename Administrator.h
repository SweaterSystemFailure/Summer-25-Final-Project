#pragma once
#include "Classroom.h"
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
        std::string getAdminLastName() const;
        std::string getSchoolName() const;
        std::string getAdminPassword() const;

        //Print
        void printSchoolReport() const;

        //Menu
        void adminMainMenu() const override;
    };
}