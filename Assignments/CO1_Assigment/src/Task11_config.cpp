#include <climits>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

constexpr long long PEAK_SURCHARGE_PERCENT = 20;
std::vector<long long> loadFares(const std::string& path, std::size_t zoneCount) {
    if (zoneCount == 0) throw std::runtime_error("no zones configured");
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open fare configuration");
    std::vector<long long> fares(zoneCount);
    std::vector<bool> seen(zoneCount, false);
    for (std::size_t row = 0; row < zoneCount; ++row) {
        std::size_t id{};
        long long paise{};
        if (!(in >> id >> paise) || id >= zoneCount || seen[id] || paise <= 0 ||
            paise > LLONG_MAX / (100 + PEAK_SURCHARGE_PERCENT))
            throw std::runtime_error("invalid fare configuration");
        fares[id] = paise;
        seen[id] = true;
    }
    std::string extra;
    if (in >> extra) throw std::runtime_error("unexpected extra configuration data");
    return fares;
}
int main() {
    try {
        const auto fares = loadFares("Task11/fares.cfg", 3);
        std::cout << "Loaded " << fares.size() << " fares (paise):";
        for (long long fare : fares) std::cout << ' ' << fare;
        std::cout << "\nLookup complexity after startup: O(1)\n";
    } catch (const std::exception& e) {
        std::cerr << "Configuration error: " << e.what() << '\n';
        return 1;
    }
}
