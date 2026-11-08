#include <cassert>
#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };
constexpr int BASE_PAISE[] = {1000, 2000, 3000}; // Rs 10, Rs 20, Rs 30
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50 must remain on the card

bool isPeakHour(int hour)
{
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

long long fare(Zone z, bool peak, bool concession = false)
{
    int base = 0;

    switch (z) {
    case Zone::Inner:
        base = BASE_PAISE[0];
        break;
    case Zone::Middle:
        base = BASE_PAISE[1];
        break;
    case Zone::Outer:
        base = BASE_PAISE[2];
        break;
    default:
        return -1; // Invalid enum value.
    }

    long long amount = base;
    if (concession) amount = amount * 50 / 100; // concession applies first
    if (peak) {
        const long long surcharge =
            (amount * PEAK_SURCHARGE_PERCENT + 50) / 100;
        amount += surcharge;
    }
    return amount;
}

bool canTravel(long long balance, long long f)
{
    return f >= 0 && balance >= f &&
           balance - f >= MIN_BALANCE_PAISE;
}

int main()
{
    // Boundary checks: 08:00 and 17:00 are peak; 10:00 and 20:00 are not.
    assert(isPeakHour(8));
    assert(!isPeakHour(10));
    assert(isPeakHour(17));
    assert(!isPeakHour(20));
    assert(fare(Zone::Middle, false, true) == 1000);
    assert(fare(Zone::Middle, true, true) == 1200);

    long long balance = 10000; // Rs 100.00 in paise
    struct Trip { Zone zone; bool peak; };
    Trip trips[] = {
        {Zone::Inner, false},
        {Zone::Outer, true},
        {Zone::Middle, false}
    };

    for (const Trip& t : trips) {
        const long long f = fare(t.zone, t.peak);
        if (canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare Rs " << f / 100 << '.' << std::setw(2)
                      << std::setfill('0') << f % 100 << std::setfill(' ');
        } else {
            std::cout << "Denied (low balance)";
        }

        std::cout << " balance Rs " << balance / 100 << '\n';
    }
}
