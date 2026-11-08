#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>

struct Measurement {
    long long paise_total;
    double double_total_rupees;
    double paise_ms;
    double double_ms;
};

Measurement measure(long long count)
{
    const auto paise_start = std::chrono::steady_clock::now();
    long long paise_total = 0;
    for (long long i = 0; i < count; ++i) {
        paise_total += 1001;
    }
    const auto paise_end = std::chrono::steady_clock::now();

    const auto double_start = std::chrono::steady_clock::now();
    double double_total_rupees = 0.0;
    for (long long i = 0; i < count; ++i) {
        double_total_rupees += 10.01;
    }
    const auto double_end = std::chrono::steady_clock::now();

    const std::chrono::duration<double, std::milli> paise_elapsed = paise_end - paise_start;
    const std::chrono::duration<double, std::milli> double_elapsed = double_end - double_start;
    return {paise_total, double_total_rupees, paise_elapsed.count(), double_elapsed.count()};
}

int main()
{
    const long long journey_counts[] = {20000, 500000, 3000000};
    constexpr long long fare_paise = 1001;

    std::cout << std::setprecision(17);
    std::cout << "Fare per journey: " << fare_paise << " paise (Rs 10.01)\n";
    std::cout << "Times below are local elapsed milliseconds; they vary by run and machine.\n\n";

    for (long long count : journey_counts) {
        const Measurement result = measure(count);
        const long long exact_paise = count * fare_paise;
        const long double exact_rupees = static_cast<long double>(exact_paise) / 100.0L;
        const long double double_error =
            static_cast<long double>(result.double_total_rupees) - exact_rupees;
        const bool int_would_overflow =
            exact_paise > static_cast<long long>(std::numeric_limits<int>::max());

        std::cout << "Journeys: " << count << '\n';
        std::cout << "  integer paise total: " << result.paise_total << '\n';
        std::cout << "  double repeat-add total (Rs): " << std::defaultfloat << std::setprecision(17)
                  << result.double_total_rupees << '\n';
        std::cout << "  double error (Rs): " << std::scientific << std::setprecision(12)
                  << double_error << '\n';
        std::cout << "  int would overflow: " << (int_would_overflow ? "yes" : "no") << '\n';
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "  measured time (ms), paise/double: " << result.paise_ms << "/"
                  << result.double_ms << "\n\n";
    }

    const long long int_max = std::numeric_limits<int>::max();
    const long long first_overflow_count = int_max / fare_paise + 1;
    std::cout << "INT_MAX: " << int_max << " paise\n";
    std::cout << "First repeat count whose paise sum exceeds INT_MAX: "
              << first_overflow_count << '\n';
}
