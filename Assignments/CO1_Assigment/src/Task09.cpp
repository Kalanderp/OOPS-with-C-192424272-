#include <functional>
#include <limits>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

constexpr long long BASE_FARES_PAISE[] = {1000, 2000, 3000};
constexpr long long PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

bool isPeakHour(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

std::optional<long long> calculateFarePaise(int zone, int hour, bool concession = false) {
    if (hour < 0 || hour > 23 || zone < 1 || zone > 3) return std::nullopt;
    long long amount = BASE_FARES_PAISE[zone - 1];
    if (concession) amount = amount * 50 / 100;
    if (!isPeakHour(hour)) return amount;
    // Integer-only half-up rounding, after any concession.
    return (amount * (100 + PEAK_SURCHARGE_PERCENT) + 50) / 100;
}

bool parseHour(const std::string& text, int& hour) {
    std::istringstream input(text);
    int parsed = 0;
    char extra = '\0';
    if (!(input >> parsed) || (input >> extra) || parsed < 0 || parsed > 23) return false;
    hour = parsed;
    return true;
}

class SmartCard {
public:
    explicit SmartCard(long long initialBalance) : balance_(initialBalance) {
        if (initialBalance < 0) throw std::invalid_argument("Negative initial balance");
    }

    long long balance() const { return balance_; }

    bool canTravel(long long fare) const {
        return fare >= 0 && balance_ >= MIN_BALANCE_PAISE &&
               fare <= balance_ - MIN_BALANCE_PAISE;
    }

    bool deduct(long long fare) {
        if (!canTravel(fare)) return false;
        balance_ -= fare;
        return true;
    }

    bool recharge(long long amount) {
        if (amount < 0 || amount > std::numeric_limits<long long>::max() - balance_) return false;
        balance_ += amount;
        return true;
    }

private:
    long long balance_;
};

struct Trip {
    int zone;
    int hour;
};

std::string fareText(int zone, int hour, bool concession = false) {
    const auto fare = calculateFarePaise(zone, hour, concession);
    return fare ? std::to_string(*fare) : "INVALID";
}

long long fiveTripTotalPaise() {
    const Trip trips[] = {{1, 7}, {2, 12}, {3, 15}, {1, 8}, {3, 17}};
    long long total = 0;
    for (const Trip& trip : trips) {
        const auto fare = calculateFarePaise(trip.zone, trip.hour);
        if (!fare) return -1;
        total += *fare;
    }
    return total;
}

struct TestCase {
    std::string name;
    std::string expected;
    std::function<std::string()> actual;
};

int main() {
    std::vector<TestCase> tests = {
        {"Inner off-peak fare (zone 1, 07:00)", "1000", [] { return fareText(1, 7); }},
        {"Inner peak fare (zone 1, 08:00)", "1200", [] { return fareText(1, 8); }},
        {"Middle off-peak fare (zone 2, 12:00)", "2000", [] { return fareText(2, 12); }},
        {"Middle peak fare (zone 2, 09:00)", "2400", [] { return fareText(2, 9); }},
        {"Outer off-peak fare (zone 3, 16:00)", "3000", [] { return fareText(3, 16); }},
        {"Outer peak fare (zone 3, 18:00)", "3600", [] { return fareText(3, 18); }},
        {"Inner student concession off-peak", "500", [] { return fareText(1, 7, true); }},
        {"Middle senior concession peak", "1200", [] { return fareText(2, 9, true); }},
        {"Boundary 08:00 is peak", "PEAK", [] { return isPeakHour(8) ? "PEAK" : "OFF-PEAK"; }},
        {"Boundary 10:00 is off-peak", "OFF-PEAK", [] { return isPeakHour(10) ? "PEAK" : "OFF-PEAK"; }},
        {"Boundary 17:00 is peak", "PEAK", [] { return isPeakHour(17) ? "PEAK" : "OFF-PEAK"; }},
        {"Boundary 20:00 is off-peak", "OFF-PEAK", [] { return isPeakHour(20) ? "PEAK" : "OFF-PEAK"; }},
        {"Fare leaves exactly Rs 50", "ALLOWED; remaining=5000", [] {
             SmartCard card(6000);
             const auto fare = calculateFarePaise(1, 7);
             const bool allowed = fare && card.deduct(*fare);
             return std::string(allowed ? "ALLOWED" : "DENIED") +
                    "; remaining=" + std::to_string(card.balance());
         }},
        {"Fare leaves less than Rs 50", "DENIED; remaining=5999", [] {
             SmartCard card(5999);
             const auto fare = calculateFarePaise(1, 7);
             const bool allowed = fare && card.deduct(*fare);
             return std::string(allowed ? "ALLOWED" : "DENIED") +
                    "; remaining=" + std::to_string(card.balance());
         }},
        {"Unknown zone rejected", "INVALID", [] { return fareText(4, 12); }},
        {"Negative recharge rejected; balance unchanged", "REJECTED; balance=10000", [] {
             SmartCard card(10000);
             const bool accepted = card.recharge(-500);
             return std::string(accepted ? "ACCEPTED" : "REJECTED") +
                    "; balance=" + std::to_string(card.balance());
         }},
        {"Letters for hour rejected", "INVALID HOUR", [] {
             int hour = 0;
             return parseHour("abc", hour) ? "VALID HOUR" : "INVALID HOUR";
         }},
        {"Five manually calculated trip fares total (paise)", "10800",
         [] { return std::to_string(fiveTripTotalPaise()); }}
    };

    std::cout << "Task 9 fare logic tests (money values in paise)\n";
    std::cout << std::left << std::setw(48) << "Test"
              << std::setw(28) << "Expected"
              << std::setw(32) << "Actual" << "Result\n";
    std::cout << std::string(118, '-') << '\n';

    bool allPassed = true;
    for (const TestCase& test : tests) {
        const std::string actual = test.actual();
        const bool passed = actual == test.expected;
        allPassed = allPassed && passed;
        std::cout << std::left << std::setw(48) << test.name
                  << std::setw(28) << test.expected
                  << std::setw(32) << actual
                  << (passed ? "PASS" : "FAIL") << '\n';
    }

    std::cout << (allPassed ? "All tests passed." : "One or more tests failed.") << '\n';
    return allPassed ? 0 : 1;
}
