#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Vehicle {
    std::string registration_;
    double dailyRate_;
    bool rented_ = false;
public:
    Vehicle(std::string registration, double rate)
        : registration_(std::move(registration)), dailyRate_(rate) {}
    virtual ~Vehicle() = default;

    const std::string& registration() const { return registration_; }
    double dailyRate() const { return dailyRate_; }
    bool isRented() const { return rented_; }
    bool rent() {
        if (rented_) return false;
        rented_ = true;
        return true;
    }
    void giveBack() { rented_ = false; }

    virtual double charge(int days, int lateDays = 0) const = 0;
    virtual std::string type() const = 0;
    virtual void serviceInfo() const = 0;
};

class Bicycle : public Vehicle {
public:
    using Vehicle::Vehicle;
    double charge(int days, int lateDays = 0) const override {
        return dailyRate() * (days + lateDays);
    }
    std::string type() const override { return "Bicycle"; }
    void serviceInfo() const override { std::cout << "No fuel/charge record\n"; }
};

class MotorVehicle : public Vehicle {
    double fuelOrChargeLevel_;
    std::string serviceDue_;
public:
    MotorVehicle(std::string reg, double rate, double level, std::string due)
        : Vehicle(std::move(reg), rate), fuelOrChargeLevel_(level),
          serviceDue_(std::move(due)) {}
    void serviceInfo() const override {
        std::cout << "Level: " << fuelOrChargeLevel_
                  << ", service due: " << serviceDue_ << '\n';
    }
};

class TwoWheeler : public MotorVehicle {
public:
    using MotorVehicle::MotorVehicle;
    double charge(int days, int lateDays = 0) const override {
        return dailyRate() * days + dailyRate() * 1.25 * lateDays;
    }
    std::string type() const override { return "TwoWheeler"; }
};

class Car : public MotorVehicle {
    int seats_;
public:
    Car(std::string reg, double rate, double level, std::string due, int seats)
        : MotorVehicle(std::move(reg), rate, level, std::move(due)), seats_(seats) {}
    double charge(int days, int lateDays = 0) const override {
        return dailyRate() * days + dailyRate() * 1.5 * lateDays;
    }
    std::string type() const override { return "Car"; }
    void serviceInfo() const override {
        MotorVehicle::serviceInfo();
        std::cout << "Seats: " << seats_ << '\n';
    }
};

class ElectricCar : public Car {
    double batteryKWh_;
    double kWhRate_;
public:
    ElectricCar(std::string reg, double rate, double level, std::string due,
                int seats, double battery, double kWhRate)
        : Car(std::move(reg), rate, level, std::move(due), seats),
          batteryKWh_(battery), kWhRate_(kWhRate) {}
    double charge(int days, int lateDays = 0) const override {
        const double rental = Car::charge(days, lateDays);
        return rental + batteryKWh_ * kWhRate_;
    }
    std::string type() const override { return "ElectricCar"; }
    void serviceInfo() const override {
        Car::serviceInfo();
        std::cout << "Battery capacity: " << batteryKWh_ << " kWh\n";
    }
};

// New type added by extension: existing Vehicle/Fleet code is unchanged.
class ElectricBus : public Vehicle {
    int seats_;
    double batteryKWh_;
    double kWhRate_;
public:
    ElectricBus(std::string reg, double rate, int seats, double battery, double kWhRate)
        : Vehicle(std::move(reg), rate), seats_(seats),
          batteryKWh_(battery), kWhRate_(kWhRate) {}
    double charge(int days, int lateDays = 0) const override {
        return dailyRate() * (days + lateDays) + batteryKWh_ * kWhRate_;
    }
    std::string type() const override { return "ElectricBus"; }
    void serviceInfo() const override {
        std::cout << "Seats: " << seats_ << ", battery: " << batteryKWh_ << " kWh\n";
    }
};

int main() {
    std::vector<std::unique_ptr<Vehicle>> fleet;
    fleet.push_back(std::make_unique<Bicycle>("BI-101", 100));
    fleet.push_back(std::make_unique<TwoWheeler>("TW-202", 300, 75, "2026-12-01"));
    fleet.push_back(std::make_unique<Car>("CA-303", 1200, 60, "2026-12-10", 5));
    fleet.push_back(std::make_unique<ElectricCar>("EV-404", 1500, 80, "2026-12-15", 5, 50, 8));
    fleet.push_back(std::make_unique<ElectricBus>("EB-505", 2500, 30, 120, 7));

    std::cout << std::fixed << std::setprecision(2);
    for (const auto& v : fleet) {
        const bool accepted = v->rent();
        std::cout << v->type() << " " << v->registration()
                  << " rent=" << (accepted ? "accepted" : "denied") << '\n';
        v->serviceInfo();
        std::cout << "Charge for 2 days + 1 late day: "
                  << v->charge(2, 1) << '\n';
        v->giveBack();
        std::cout << "Returned: " << (v->isRented() ? "no" : "yes") << "\n\n";
    }
}
