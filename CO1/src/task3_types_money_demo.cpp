#include <climits>
#include <iomanip>
#include <iostream>
#include <string>

enum class Zone : int { Inner, Middle, Outer };
constexpr long long MIN_BALANCE_PAISE = 5000;

long long farePaise(int basePaise, bool peak, bool concession) {
    long long fare = basePaise;
    if (concession) fare = fare * 50 / 100; // apply concession to base first
    if (peak) fare = (fare * 120 + 50) / 100; // then peak surcharge, half-up
    return fare;
}

bool canTravel(long long balancePaise, long long fare) {
    return balancePaise >= fare + MIN_BALANCE_PAISE;
}

int main() {
    auto inferred = 1LL;
    std::cout << "sizeof(bool,char,int,unsigned int,long long,float,double,Zone,string,auto-long-long)="
              << sizeof(bool) << ',' << sizeof(char) << ',' << sizeof(int) << ','
              << sizeof(unsigned int) << ',' << sizeof(long long) << ',' << sizeof(float) << ','
              << sizeof(double) << ',' << sizeof(Zone) << ',' << sizeof(std::string) << ','
              << sizeof(inferred) << " bytes\n";

    constexpr long long inner = 1'200'000;
    constexpr long long middle = 1'000'000;
    constexpr long long outer = 800'000;
    // Per zone: 56% off-peak/no concession, 14% off-peak/concession,
    //           24% peak/no concession, 6% peak/concession.
    struct Group { long long count; int base; bool peak; bool concession; };
    const Group groups[] = {
        {inner * 56 / 100, 1000, false, false}, {inner * 14 / 100, 1000, false, true},
        {inner * 24 / 100, 1000, true, false},  {inner * 6 / 100, 1000, true, true},
        {middle * 56 / 100, 2000, false, false}, {middle * 14 / 100, 2000, false, true},
        {middle * 24 / 100, 2000, true, false},  {middle * 6 / 100, 2000, true, true},
        {outer * 56 / 100, 3000, false, false}, {outer * 14 / 100, 3000, false, true},
        {outer * 24 / 100, 3000, true, false},  {outer * 6 / 100, 3000, true, true}
    };
    long long totalPaise = 0;
    long long totalJourneys = 0;
    for (const Group& g : groups) {
        totalPaise += g.count * farePaise(g.base, g.peak, g.concession);
        totalJourneys += g.count;
    }
    std::cout << "journeys=" << totalJourneys << '\n';
    std::cout << "network_total_paise=" << totalPaise << '\n';
    std::cout << "network_total_rupees=" << totalPaise / 100 << '\n';
    std::cout << "INT_MAX=" << INT_MAX << '\n';
    std::cout << "paise_total_exceeds_INT_MAX=" << std::boolalpha
              << (totalPaise > INT_MAX) << " (do not accumulate this in int)\n";

    constexpr int n = 10'000'000;
    double sumRupees = 0.0;
    for (int i = 0; i < n; ++i) sumRupees += 0.10;
    const long long exactPaise = static_cast<long long>(n) * 10LL;
    const double exactRupees = static_cast<double>(exactPaise) / 100.0;
    std::cout << std::setprecision(17);
    std::cout << "double_sum_rupees=" << sumRupees << '\n';
    std::cout << "exact_sum_rupees=" << exactRupees << '\n';
    std::cout << "absolute_error_rupees=" << (sumRupees - exactRupees) << '\n';

    std::cout << "can_travel_balance_10000_fare_5000="
              << canTravel(10000, 5000) << '\n';
}
