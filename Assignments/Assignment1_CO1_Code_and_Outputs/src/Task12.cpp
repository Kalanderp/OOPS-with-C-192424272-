#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>

namespace metro {

constexpr long long MIN_BALANCE_PAISE = 5000;
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int WEEKEND_DISCOUNT_PERCENT = 10;
constexpr int CONCESSION_PERCENT = 50;

enum class Zone { Inner, Middle, Outer, Airport };
enum class PassTerm { Month, Year };

constexpr std::array<long long, 4> BASE_FARE_PAISE{{1000, 2000, 3000, 6000}};

const char* zoneName(Zone zone) {
    switch (zone) {
        case Zone::Inner: return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer: return "Outer";
        case Zone::Airport: return "Airport";
    }
    throw std::invalid_argument("Unknown zone");
}

std::size_t zoneIndex(Zone zone) {
    switch (zone) {
        case Zone::Inner: return 0;
        case Zone::Middle: return 1;
        case Zone::Outer: return 2;
        case Zone::Airport: return 3;
    }
    throw std::invalid_argument("Unknown zone");
}

bool isLeapYear(int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int daysInMonth(int year, int month) {
    static constexpr std::array<int, 12> DAYS{{31, 28, 31, 30, 31, 30,
                                               31, 31, 30, 31, 30, 31}};
    if (month < 1 || month > 12) throw std::invalid_argument("Invalid month");
    return month == 2 && isLeapYear(year) ? 29 : DAYS[static_cast<std::size_t>(month - 1)];
}

struct Date {
    int year;
    int month;
    int day;

    Date(int y, int m, int d) : year(y), month(m), day(d) {
        if (year < 1 || year > 9999 || month < 1 || month > 12 ||
            day < 1 || day > daysInMonth(year, month)) {
            throw std::invalid_argument("Invalid Gregorian date");
        }
    }

    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator==(const Date& other) const {
        return year == other.year && month == other.month && day == other.day;
    }

    bool operator>(const Date& other) const { return other < *this; }

    int weekdaySundayZero() const {
        static constexpr std::array<int, 12> offsets{{0, 3, 2, 5, 0, 3,
                                                      5, 1, 4, 6, 2, 4}};
        int adjustedYear = year;
        if (month < 3) --adjustedYear;
        return (adjustedYear + adjustedYear / 4 - adjustedYear / 100 +
                adjustedYear / 400 + offsets[static_cast<std::size_t>(month - 1)] + day) % 7;
    }

    bool isWeekend() const {
        const int weekday = weekdaySundayZero();
        return weekday == 0 || weekday == 6;
    }

    Date addMonthsClamped(int count) const {
        const int totalMonths = (year - 1) * 12 + (month - 1) + count;
        const int newYear = totalMonths / 12 + 1;
        const int newMonth = totalMonths % 12 + 1;
        if (newYear > 9999) throw std::invalid_argument("Pass expiry out of range");
        return Date(newYear, newMonth, std::min(day, daysInMonth(newYear, newMonth)));
    }

    Date addYearsClamped(int count) const {
        const int newYear = year + count;
        if (newYear > 9999) throw std::invalid_argument("Pass expiry out of range");
        return Date(newYear, month, std::min(day, daysInMonth(newYear, month)));
    }
};

struct TimeOfDay {
    int hour;
    int minute;

    TimeOfDay(int h, int m) : hour(h), minute(m) {
        if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
            throw std::invalid_argument("Invalid time");
    }
};

bool isPeak(const TimeOfDay& time) {
    const int minutes = time.hour * 60 + time.minute;
    return (minutes >= 8 * 60 && minutes < 10 * 60) ||
           (minutes >= 17 * 60 && minutes < 20 * 60);
}

long long farePaise(Zone zone, const Date& date, const TimeOfDay& time,
                    bool hasConcession) {
    long long fare = BASE_FARE_PAISE[zoneIndex(zone)];
    if (hasConcession) fare = fare * CONCESSION_PERCENT / 100;

    // Weekend discount replaces, rather than combines with, the weekday peak surcharge.
    if (date.isWeekend()) {
        fare = fare * (100 - WEEKEND_DISCOUNT_PERCENT) / 100;
    } else if (isPeak(time)) {
        fare = fare * (100 + PEAK_SURCHARGE_PERCENT) / 100;
    }
    return fare;
}

class ZonePass {
public:
    ZonePass(Zone first, Zone second, Date start, PassTerm term)
        : first_(first), second_(second), start_(start), term_(term) {
        if (first == second) throw std::invalid_argument("Pass requires two distinct zones");
        (void)zoneIndex(first_);
        (void)zoneIndex(second_);
    }

    Date expiryDate() const {
        return term_ == PassTerm::Month ? start_.addMonthsClamped(1)
                                        : start_.addYearsClamped(1);
    }

    bool covers(Zone zone, const Date& travelDate) const {
        (void)zoneIndex(zone);
        const bool selectedZone = zone == first_ || zone == second_;
        return selectedZone && !(travelDate < start_) && !(expiryDate() < travelDate);
    }

private:
    Zone first_;
    Zone second_;
    Date start_;
    PassTerm term_;
};

enum class JourneyStatus { Paid, PassCovered, Denied };

struct JourneyResult {
    JourneyStatus status;
    long long farePaid;
    long long balanceAfter;
};

class SmartCard {
public:
    explicit SmartCard(long long initialBalance) : balance_(initialBalance) {
        if (initialBalance < 0)
            throw std::invalid_argument("Initial balance cannot be negative");
    }

    JourneyResult travel(Zone zone, const Date& date, const TimeOfDay& time,
                         bool hasConcession, const ZonePass* pass = nullptr) {
        if (pass != nullptr && pass->covers(zone, date))
            return {JourneyStatus::PassCovered, 0, balance_};

        const long long fare = farePaise(zone, date, time, hasConcession);
        if (balance_ - fare < MIN_BALANCE_PAISE)
            return {JourneyStatus::Denied, 0, balance_};

        balance_ -= fare;
        return {JourneyStatus::Paid, fare, balance_};
    }

    long long balancePaise() const { return balance_; }

private:
    long long balance_;
};

void printPassBoundary(const std::string& label, const ZonePass& pass,
                       const Date& expiry, const Date& nextDay) {
    std::cout << label << " expiry=" << expiry.year << '-';
    if (expiry.month < 10) std::cout << '0';
    std::cout << expiry.month << '-';
    if (expiry.day < 10) std::cout << '0';
    std::cout << expiry.day << ": on-expiry="
              << (pass.covers(Zone::Middle, expiry) ? "YES" : "NO")
              << "; next-day="
              << (pass.covers(Zone::Middle, nextDay) ? "YES" : "NO") << '\n';
}

} // namespace metro

int main() {
    using namespace metro;
    const Date weekday(2025, 3, 3); // Monday
    const Date saturday(2025, 3, 1);
    const Date sunday(2025, 3, 2);
    const TimeOfDay offPeak(12, 0);
    const TimeOfDay peak(8, 0);

    std::cout << "Airport weekday off-peak: "
              << farePaise(Zone::Airport, weekday, offPeak, false) << " paise\n";
    std::cout << "Airport weekday peak: "
              << farePaise(Zone::Airport, weekday, peak, false) << " paise\n";
    std::cout << "Inner weekend: "
              << farePaise(Zone::Inner, saturday, offPeak, false) << " paise\n";
    std::cout << "Outer Sunday base discount: "
              << farePaise(Zone::Outer, sunday, offPeak, false) << " paise\n";
    std::cout << "Concession then weekday peak (Airport): "
              << farePaise(Zone::Airport, weekday, peak, true) << " paise\n";

    std::cout << "Weekday peak boundaries (08:00, 10:00, 17:00, 20:00): ";
    for (const TimeOfDay time : {TimeOfDay(8, 0), TimeOfDay(10, 0),
                                 TimeOfDay(17, 0), TimeOfDay(20, 0)}) {
        std::cout << (isPeak(time) ? 'P' : 'O') << (time.hour == 20 ? '\n' : ',');
    }

    const ZonePass marchPass(Zone::Inner, Zone::Middle, Date(2025, 3, 1), PassTerm::Month);
    SmartCard coveredCard(10000);
    const JourneyResult covered = coveredCard.travel(
        Zone::Middle, sunday, offPeak, false, &marchPass);
    std::cout << "Covered Middle trip: PASS; fare=" << covered.farePaid
              << " paise; balance=" << covered.balanceAfter << " paise\n";

    SmartCard outerCard(10000);
    const JourneyResult outer = outerCard.travel(Zone::Outer, weekday, peak, false);
    std::cout << "Uncovered Outer peak: PAID; fare=" << outer.farePaid
              << " paise; balance=" << outer.balanceAfter << " paise\n";

    SmartCard exactMinimumCard(11000);
    const JourneyResult exactMinimum = exactMinimumCard.travel(
        Zone::Airport, weekday, offPeak, false);
    std::cout << "Exactly minimum remains: "
              << (exactMinimum.status == JourneyStatus::Paid ? "PAID" : "DENIED")
              << "; balance=" << exactMinimum.balanceAfter << " paise\n";
    SmartCard protectedCard(10999);
    const JourneyResult protectedResult = protectedCard.travel(
        Zone::Airport, weekday, offPeak, false);
    std::cout << "Below minimum would remain: "
              << (protectedResult.status == JourneyStatus::Denied ? "DENIED" : "PAID")
              << "; balance=" << protectedResult.balanceAfter << " paise\n";

    const ZonePass jan2025(Zone::Inner, Zone::Middle, Date(2025, 1, 31), PassTerm::Month);
    printPassBoundary("Monthly Jan-31-2025", jan2025, Date(2025, 2, 28), Date(2025, 3, 1));
    const ZonePass jan2024(Zone::Inner, Zone::Middle, Date(2024, 1, 31), PassTerm::Month);
    printPassBoundary("Leap monthly Jan-31-2024", jan2024, Date(2024, 2, 29), Date(2024, 3, 1));
    const ZonePass leapYear(Zone::Inner, Zone::Middle, Date(2024, 2, 29), PassTerm::Year);
    printPassBoundary("Yearly Feb-29-2024", leapYear, Date(2025, 2, 28), Date(2025, 3, 1));

    return 0;
}
