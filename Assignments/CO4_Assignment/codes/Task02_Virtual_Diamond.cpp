#include <iostream>
#include <string>

struct Person {
    std::string name;
    explicit Person(std::string n = "Unknown") : name(std::move(n)) {
        std::cout << "Person ";
    }
    virtual ~Person() = default;
};

struct Student : virtual Person {
    Student() { std::cout << "Student "; }
};

struct Staff : virtual Person {
    Staff() { std::cout << "Staff "; }
};

struct TeachingAssistant : Student, Staff {
    TeachingAssistant() : Person("Kavya") { std::cout << "TA "; }
};

struct StudentPart : Person {
    StudentPart() : Person("Student-name") {}
};
struct StaffPart : Person {
    StaffPart() : Person("Staff-name") {}
};
struct TAComposition {
    StudentPart student;
    StaffPart staff;
};

int main() {
    std::cout << "Virtual inheritance construction: ";
    TeachingAssistant ta;
    std::cout << "\nShared name: " << ta.name << '\n';

    TAComposition composed;
    std::cout << "Composition contains two Person subobjects: "
              << composed.student.name << " / " << composed.staff.name << '\n';
    std::cout << "sizeof(TeachingAssistant): " << sizeof(TeachingAssistant) << '\n';
    std::cout << "sizeof(TAComposition): " << sizeof(TAComposition) << '\n';
}
