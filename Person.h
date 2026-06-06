#pragma once
#include "Date.h"
#include <fstream>
#include <string>

class Person {
protected:
    std::string lastName;
    std::string firstName;
    std::string middleName;
    Person();
public:
    Person(const std::string& ln, const std::string& fn, const std::string& mn);
    virtual ~Person() {}

    virtual std::string getRole() const = 0;

    const std::string& getLastName()   const { return lastName; }
    const std::string& getFirstName()  const { return firstName; }
    const std::string& getMiddleName() const { return middleName; }
    std::string getFullName() const;
};

class Reader : public Person {
private:
    int id;
    std::string ticketNumber;
    Date registrationDate;
    int activeLoansCount;
public:
    static const int MAX_ACTIVE_LOANS = 5;
    Reader();
    Reader(int id, const std::string& ln, const std::string& fn, const std::string& mn,
        const std::string& ticket, const Date& regDate);
    std::string getRole() const override { return "Читатель"; }
    int getId() const { return id; }
    const std::string& getTicketNumber()     const { return ticketNumber; }
    const Date& getRegistrationDate() const { return registrationDate; }
    int  getActiveLoansCount() const { return activeLoansCount; }
    void incrementLoans();
    void decrementLoans();
    void writeTo(std::ofstream& out) const;
    void readFrom(std::ifstream& in);
};
