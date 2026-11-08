#include <iomanip>
#include <iostream>

// Shared fare model: Rs 10/20/30 in paise; 20% peak surcharge; Rs 50 minimum.
constexpr long long minimum = 5000;
constexpr long long BASE_FARES_PAISE[] = {1000, 2000, 3000};

bool isPeakHour(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

long long fareFor(int zone, bool peak, bool concession) {
    if (zone < 1 || zone > 3) return -1;
    long long amount = BASE_FARES_PAISE[zone - 1];
    if (concession) amount = amount * 50 / 100;
    if (peak) amount = amount + (amount * 20 + 50) / 100;
    return amount;
}

bool canTravel(long long balance, long long fare) {
    return balance >= 0 && fare >= 0 && balance >= fare && balance - fare >= minimum;
}

bool deduct(long long& balance, long long fare) {
    if (!canTravel(balance, fare)) return false;
    balance -= fare;
    return true;
}

bool readInteger(long long& value) {
    if (!(std::cin >> value)) {
        std::cerr << "Invalid input: expected an integer.\n";
        return false;
    }
    return true;
}

void printMoney(long long paise) {
    std::cout << "Rs " << paise / 100 << '.' << std::setw(2) << std::setfill('0')
              << paise % 100 << std::setfill(' ');
}

int main() {
    long long balanceInput, zoneInput, hourInput, concessionInput;
    if (!readInteger(balanceInput) || !readInteger(zoneInput) ||
        !readInteger(hourInput) || !readInteger(concessionInput)) return 1;
    if (balanceInput < 0 || zoneInput < 1 || zoneInput > 3 || hourInput < 0 || hourInput > 23 ||
        (concessionInput != 0 && concessionInput != 1)) {
        std::cerr << "Invalid input: balance must be nonnegative, zone 1-3, and hour 0-23.\n";
        return 1;
    }

    long long balance = balanceInput;
    const long long fare = fareFor(static_cast<int>(zoneInput), isPeakHour(static_cast<int>(hourInput)), concessionInput == 1);
    std::cout << "Fare: ";
    printMoney(fare);
    std::cout << '\n';
    if (deduct(balance, fare)) std::cout << "Journey accepted.\n";
    else std::cout << "Journey denied: Rs 50.00 must remain.\n";
    std::cout << "Balance: ";
    printMoney(balance);
    std::cout << '\n';
}
