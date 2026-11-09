#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Staff {
private:
    std::string name_;
    int id_;
    double basic_;

protected:
    double basicPay() const { return basic_; }

public:
    Staff(std::string name, int id, double basic)
        : name_(std::move(name)), id_(id), basic_(basic) {}
    virtual ~Staff() = default;

    const std::string& name() const { return name_; }
    int id() const { return id_; }
    virtual double pay() const = 0;
    virtual const char* category() const = 0;
};

class Professor : public Staff {
private:
    double research_;

public:
    Professor(std::string name, int id, double basic, double research)
        : Staff(std::move(name), id, basic), research_(research) {}
    double pay() const override { return basicPay() * 1.42 + research_; }
    const char* category() const override { return "Professor"; }
};

class LabAssistant : public Staff {
public:
    LabAssistant(std::string name, int id, double basic)
        : Staff(std::move(name), id, basic) {}
    double pay() const override { return basicPay() * 1.42; }
    const char* category() const override { return "LabAssistant"; }
};

class GuestFaculty : public Staff {
private:
    double hours_;
    double rate_;

public:
    GuestFaculty(std::string name, int id, double hours, double rate)
        : Staff(std::move(name), id, 0.0), hours_(hours), rate_(rate) {}
    double pay() const override { return hours_ * rate_; }
    const char* category() const override { return "GuestFaculty"; }
};

int main() {
    std::vector<std::unique_ptr<Staff>> payroll;
    payroll.push_back(std::make_unique<Professor>("Anita", 101, 50000, 8000));
    payroll.push_back(std::make_unique<LabAssistant>("Bala", 102, 30000));
    payroll.push_back(std::make_unique<GuestFaculty>("Charan", 103, 20, 1000));

    std::cout << std::fixed << std::setprecision(2);
    double total = 0.0;
    for (const auto& member : payroll) {
        const double amount = member->pay();
        total += amount;
        std::cout << member->category() << " " << member->name()
                  << " (" << member->id() << "): " << amount << '\n';
    }
    std::cout << "Total payroll: " << total << '\n';
}
