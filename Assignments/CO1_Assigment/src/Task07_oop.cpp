#include <iomanip>
#include <iostream>
#include <stdexcept>

// Same shared fare model as the procedural version; money is stored in paise.
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

class SmartCard {
public:
    explicit SmartCard(long long initialBalance) : balance_(initialBalance) {
        if (initialBalance < 0) throw std::invalid_argument("Balance cannot be negative");
    }

    bool canTravel(long long fare) const {
        return fare >= 0 && balance_ >= fare && balance_ - fare >= minimum;
    }

    bool deduct(long long fare) {
        if (!canTravel(fare)) return false;
        balance_ -= fare;
        return true;
    }

    long long balance() const { return balance_; }

private:
    long long balance_;
};

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
    long long initialBalance, zoneInput, hourInput, concessionInput;
    if (!readInteger(initialBalance) || !readInteger(zoneInput) ||
        !readInteger(hourInput) || !readInteger(concessionInput)) return 1;
    if (initialBalance < 0 || zoneInput < 1 || zoneInput > 3 || hourInput < 0 || hourInput > 23 ||
        (concessionInput != 0 && concessionInput != 1)) {
        std::cerr << "Invalid input: balance must be nonnegative, zone 1-3, and hour 0-23.\n";
        return 1;
    }

    SmartCard card(initialBalance);
    const long long fare = fareFor(static_cast<int>(zoneInput), isPeakHour(static_cast<int>(hourInput)), concessionInput == 1);
    std::cout << "Fare: ";
    printMoney(fare);
    std::cout << '\n';
    if (card.deduct(fare)) std::cout << "Journey accepted.\n";
    else std::cout << "Journey denied: Rs 50.00 must remain.\n";
    std::cout << "Balance: ";
    printMoney(card.balance());
    std::cout << '\n';
}
