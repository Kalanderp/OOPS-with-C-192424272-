#include <cstdint>
#include <iostream>

enum class Zone { Inner, Middle, Outer };
namespace config {
constexpr long long INNER_FARE_PAISE = 1000;
constexpr long long MIDDLE_FARE_PAISE = 2000;
constexpr long long OUTER_FARE_PAISE = 3000;
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int CONCESSION_PERCENT = 50;
constexpr long long MIN_BALANCE_PAISE = 5000;
}
constexpr long long baseFare(Zone zone) {
    switch (zone) {
        case Zone::Inner: return config::INNER_FARE_PAISE;
        case Zone::Middle: return config::MIDDLE_FARE_PAISE;
        case Zone::Outer: return config::OUTER_FARE_PAISE;
    }
    return 0;
}
int main() {
    std::cout << "Inner=" << baseFare(Zone::Inner) << " paise\n"
              << "Middle=" << baseFare(Zone::Middle) << " paise\n"
              << "Outer=" << baseFare(Zone::Outer) << " paise\n"
              << "Peak surcharge=" << config::PEAK_SURCHARGE_PERCENT << "%\n"
              << "Concession=" << config::CONCESSION_PERCENT << "%\n"
              << "Minimum remaining=" << config::MIN_BALANCE_PAISE << " paise\n";
}
