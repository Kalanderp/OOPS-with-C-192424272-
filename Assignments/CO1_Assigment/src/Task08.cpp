#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

enum class Zone { Inner, Middle, Outer };
constexpr long long MIN_BALANCE_PAISE = 5000;

void printMoney(long long paise) {
    std::cout << "Rs " << paise / 100 << '.'
              << std::setw(2) << std::setfill('0') << paise % 100
              << std::setfill(' ');
}

int main() {
    int hour = 0;
    while (true) {
        std::cout << "Enter hour (0-23): " << std::flush;
        std::cin >> hour;
        if (std::cin.fail()) {
            if (std::cin.eof()) {
                std::cout << "Input ended before a valid hour was entered." << std::endl;
                return 1;
            }
            std::cout << "Invalid hour text; please enter a numeric hour." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), static_cast<char>(10));
            continue;
        }
        if (hour < 0 || hour > 23) {
            std::cout << "Hour must be from 0 through 23. Try again." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), static_cast<char>(10));
            continue;
        }
        std::cout << std::endl;
        break;
    }

    std::string zoneText;
    std::cout << "Enter zone (Inner/Middle/Outer): " << std::flush;
    if (!(std::cin >> zoneText)) {
        std::cout << "Invalid or missing zone input." << std::endl;
        return 1;
    }
    Zone zone;
    if (zoneText == "Inner") zone = Zone::Inner;
    else if (zoneText == "Middle") zone = Zone::Middle;
    else if (zoneText == "Outer") zone = Zone::Outer;
    else {
        std::cout << "Invalid zone: enter Inner, Middle, or Outer." << std::endl;
        return 1;
    }
    std::cout << std::endl;

    int concessionInput = 0;
    std::cout << "Concession eligible? (0=no, 1=yes): " << std::flush;
    std::cin >> concessionInput;
    if (std::cin.fail() || (concessionInput != 0 && concessionInput != 1)) {
        if (!std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Invalid concession flag: enter 0 or 1." << std::endl;
        return 1;
    }
    std::cout << std::endl;

    long long balanceRupees = 0;
    std::cout << "Enter card balance (whole rupees): " << std::flush;
    std::cin >> balanceRupees;
    if (std::cin.fail()) {
        if (!std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), static_cast<char>(10));
        }
        std::cout << "Invalid balance: expected a whole-rupee number." << std::endl;
        return 1;
    }
    if (balanceRupees < 0 || balanceRupees > std::numeric_limits<long long>::max() / 100) {
        std::cout << "Invalid balance: amount is negative or too large." << std::endl;
        return 1;
    }
    std::cout << std::endl;

    const bool peak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    long long baseFarePaise = 0;
    switch (zone) {
        case Zone::Inner:  baseFarePaise = 1000; break;
        case Zone::Middle: baseFarePaise = 2000; break;
        case Zone::Outer:  baseFarePaise = 3000; break;
    }
    long long farePaise = baseFarePaise;
    if (concessionInput == 1) farePaise = farePaise * 50 / 100;
    if (peak) farePaise += (farePaise * 20 + 50) / 100;
    const long long startingBalancePaise = balanceRupees * 100;
    const bool approved = startingBalancePaise >= farePaise &&
                         startingBalancePaise - farePaise >= MIN_BALANCE_PAISE;
    const long long remainingBalancePaise = approved
        ? startingBalancePaise - farePaise : startingBalancePaise;

    std::cout << "================ Journey Report ================" << std::endl;
    std::cout << "Hour: " << hour << ":00 (" << (peak ? "Peak" : "Off-peak") << ')' << std::endl;
    std::cout << "Zone: " << zoneText << std::endl;
    std::cout << std::left << std::setw(20) << "Fare:";
    printMoney(farePaise);
    std::cout << std::endl << std::left << std::setw(20) << "Starting balance:";
    printMoney(startingBalancePaise);
    std::cout << std::endl << std::left << std::setw(20) << "Status:"
              << (approved ? "APPROVED" : "DENIED (Rs 50 minimum would not remain)") << std::endl;
    std::cout << std::left << std::setw(20) << "Remaining balance:";
    printMoney(remainingBalancePaise);
    std::cout << std::endl << "=================================================" << std::endl;
    return 0;
}
