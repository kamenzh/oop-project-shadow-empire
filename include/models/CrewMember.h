#ifndef SHADOW_EMPIRE_CREW_MEMBER_H
#define SHADOW_EMPIRE_CREW_MEMBER_H

#include <string>

class CrewMember {
protected:
    int id;
    std::string name;
    int hirePrice;
    int weeklySalary;

public:
    CrewMember(int id, const std::string& name, int hirePrice, int weeklySalary);
    virtual ~CrewMember() = default;

    virtual std::string getRole() const = 0;

    int getId() const;
    std::string getName() const;
    int getHirePrice() const;
    int getWeeklySalary() const;
};

#endif
