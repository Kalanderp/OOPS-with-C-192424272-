#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

class BillingError : public std::runtime_error { public: using std::runtime_error::runtime_error; };
class InvalidCharge : public BillingError { public: using BillingError::BillingError; };
class ClaimError : public BillingError { public: using BillingError::BillingError; };
class UnknownPackage : public BillingError { public: using BillingError::BillingError; };
class MissingPatient : public BillingError { public: using BillingError::BillingError; };

class Bill {
protected:
    std::string patientId;
    std::string patientName;
    static void requireNonNegative(double x, const std::string& field) {
        if (x < 0) throw InvalidCharge(field + " cannot be negative");
    }
public:
    Bill(std::string id, std::string name) : patientId(std::move(id)), patientName(std::move(name)) {
        if (patientId.empty()) throw MissingPatient("missing patient record");
    }
    virtual ~Bill() noexcept = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
    virtual void print() const {
        std::cout << type() << " | " << patientName << " | Rs. " << std::fixed
                  << std::setprecision(2) << total() << '\n';
    }
    const std::string& id() const noexcept { return patientId; }
};

class OutPatientBill final : public Bill {
    double consultation, tests;
public:
    OutPatientBill(std::string id, std::string n, double c, double t)
      : Bill(std::move(id), std::move(n)), consultation(c), tests(t) {
        requireNonNegative(c, "consultation"); requireNonNegative(t, "tests");
    }
    double total() const override { return consultation + tests; }
    std::string type() const override { return "Outpatient"; }
};
class InPatientBill final : public Bill {
    int days; double perDay, procedures, medicines;
public:
    InPatientBill(std::string id, std::string n, int d, double rate, double p, double m)
      : Bill(std::move(id), std::move(n)), days(d), perDay(rate), procedures(p), medicines(m) {
        if (d < 0) throw InvalidCharge("days cannot be negative");
        requireNonNegative(rate,"room rate"); requireNonNegative(p,"procedures"); requireNonNegative(m,"medicines");
    }
    double total() const override { return days * perDay + procedures + medicines; }
    std::string type() const override { return "Inpatient"; }
};
class DayCareBill final : public Bill {
    std::string packageCode; double packagePrice;
public:
    DayCareBill(std::string id, std::string n, std::string code, const std::unordered_map<std::string,double>& rates)
      : Bill(std::move(id), std::move(n)), packageCode(std::move(code)), packagePrice(0) {
        auto it = rates.find(packageCode);
        if (it == rates.end()) throw UnknownPackage("unknown package code: " + packageCode);
        requireNonNegative(it->second,"package price"); packagePrice = it->second;
    }
    double total() const override { return packagePrice; }
    std::string type() const override { return "Day-care"; }
};
class EmergencyBill final : public Bill {
    double baseCharge, hourlyCharge; int hours;
public:
    EmergencyBill(std::string id, std::string n, double base, double rate, int h)
      : Bill(std::move(id), std::move(n)), baseCharge(base), hourlyCharge(rate), hours(h) {
        requireNonNegative(base,"base charge"); requireNonNegative(rate,"hourly charge");
        if (h < 0) throw InvalidCharge("hours cannot be negative");
    }
    double total() const override { return baseCharge + hourlyCharge * hours; }
    std::string type() const override { return "Emergency"; }
};

// Extension example: new bill type added without modifying Bill or existing derived classes.
class DiagnosticBill final : public Bill {
    int scans; double scanPrice, reportFee;
public:
    DiagnosticBill(std::string id, std::string n, int s, double price, double fee)
      : Bill(std::move(id), std::move(n)), scans(s), scanPrice(price), reportFee(fee) {
        if (s < 0) throw InvalidCharge("scan count cannot be negative");
        requireNonNegative(price,"scan price"); requireNonNegative(fee,"report fee");
    }
    double total() const override { return scans * scanPrice + reportFee; }
    std::string type() const override { return "Diagnostic"; }
};

class Payer {
public:
    virtual ~Payer() noexcept = default;
    virtual double amountCovered(const Bill& bill) const = 0;
    virtual std::string name() const = 0;
};
class SelfPay final : public Payer {
public:
    double amountCovered(const Bill& b) const override { return b.total(); }
    std::string name() const override { return "SelfPay"; }
};
class Insurance final : public Payer {
    double limit, coPayPercent;
public:
    Insurance(double l, double c) : limit(l), coPayPercent(c) {
        if (l < 0 || c < 0 || c > 100) throw InvalidCharge("invalid insurance policy");
    }
    double amountCovered(const Bill& b) const override {
        return std::min(b.total() * (1.0 - coPayPercent/100.0), limit);
    }
    std::string name() const override { return "Insurance"; }
};
class GovernmentScheme final : public Payer {
    std::unordered_map<std::string,double> packageRates;
public:
    explicit GovernmentScheme(std::unordered_map<std::string,double> r) : packageRates(std::move(r)) {}
    double amountCovered(const Bill& b) const override {
        auto it = packageRates.find(b.type());
        return it == packageRates.end() ? 0.0 : std::min(b.total(), it->second);
    }
    std::string name() const override { return "GovernmentScheme"; }
};
// Extension example: corporate payer is independent of existing payer classes.
class CorporatePayer final : public Payer {
    double coveragePercent;
public:
    explicit CorporatePayer(double p) : coveragePercent(p) {
        if (p < 0 || p > 100) throw InvalidCharge("invalid corporate coverage");
    }
    double amountCovered(const Bill& b) const override { return b.total() * coveragePercent / 100.0; }
    std::string name() const override { return "CorporatePayer"; }
};

int main() {
    const std::unordered_map<std::string,double> rates{{"DC-ORTHO",18000.0},{"DC-EYE",9000.0}};
    std::vector<std::unique_ptr<Bill>> bills;
    bills.emplace_back(std::make_unique<OutPatientBill>("P001","Ravi",500,1200));
    bills.emplace_back(std::make_unique<InPatientBill>("P002","Meena",3,2500,40000,0));
    bills.emplace_back(std::make_unique<DayCareBill>("P003","Asha","DC-ORTHO",rates));
    bills.emplace_back(std::make_unique<EmergencyBill>("P004","Imran",2000,750,4));
    bills.emplace_back(std::make_unique<DiagnosticBill>("P005","Leela",2,3500,500));

    SelfPay self;
    Insurance insurer(50000,20);
    GovernmentScheme govt({{"Outpatient",2000},{"Inpatient",45000},{"Day-care",18000},{"Emergency",5000},{"Diagnostic",7000}});
    CorporatePayer corporate(70);
    std::vector<const Payer*> payers{&self,&insurer,&govt,&corporate};

    double dayTotal = 0;
    for (const auto& b : bills) { b->print(); dayTotal += b->total(); }
    std::cout << "DAY TOTAL | Rs. " << dayTotal << "\n";
    for (const auto* payer : payers) {
        double covered = 0;
        for (const auto& bill : bills) covered += payer->amountCovered(*bill);
        std::cout << payer->name() << " covers Rs. " << covered << '\n';
    }
}
