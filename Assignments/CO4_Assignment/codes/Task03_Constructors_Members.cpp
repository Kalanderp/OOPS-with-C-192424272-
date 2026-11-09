#include <iostream>
#include <string>
#include <utility>

class Address {
    std::string city_;
    int pin_;
public:
    Address(std::string city, int pin) : city_(std::move(city)), pin_(pin) {
        std::cout << "Address+ ";
    }
    ~Address() { std::cout << "~Address "; }
    std::string str() const { return city_ + "-" + std::to_string(pin_); }
};

class Course {
    std::string title_;
public:
    explicit Course(std::string title) : title_(std::move(title)) {
        std::cout << "Course+ ";
    }
    ~Course() { std::cout << "~Course "; }
};

class Person {
protected:
    std::string name_;
    int age_;
    Address address_;
public:
    Person(std::string name, int age, std::string city, int pin)
        : name_(std::move(name)), age_(age), address_(std::move(city), pin) {
        std::cout << "Person+ ";
    }
    virtual ~Person() { std::cout << "~Person "; }
};

class Student : public Person {
    long long regNo_;
    Course course_;
public:
    Student(std::string name, int age, long long regNo,
            std::string city, int pin, std::string course)
        : Person(std::move(name), age, std::move(city), pin),
          regNo_(regNo), course_(std::move(course)) {
        std::cout << "Student+ ";
    }
    ~Student() override { std::cout << "~Student "; }
    void print() const {
        std::cout << "\n" << regNo_ << " " << name_ << " (" << age_
                  << ") " << address_.str() << '\n';
    }
};

struct Engine {
    Engine() { std::cout << "Engine "; }
    ~Engine() { std::cout << "~Engine "; }
};
struct Vehicle {
    Vehicle() { std::cout << "Vehicle "; }
    ~Vehicle() { std::cout << "~Vehicle "; }
};
struct Car : Vehicle {
    Engine e;
    Car() { std::cout << "Car "; }
    ~Car() { std::cout << "~Car "; }
};

int main() {
    std::cout << "Student construction/destruction trace:\n";
    {
        Student s("Kavya", 19, 192311001, "Salem", 636001, "C++");
        s.print();
    }
    std::cout << "\nVehicle construction/destruction prediction:\n";
    {
        Car c;
        std::cout << "| ";
    }
    std::cout << '\n';
}
