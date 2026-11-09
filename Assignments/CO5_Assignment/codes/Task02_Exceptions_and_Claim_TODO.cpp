#include <iostream>
#include <new>
#include <stdexcept>
#include <string>
#include <vector>
class ClaimError : public std::runtime_error { public: using std::runtime_error::runtime_error; };
double approve(double billed, double cover) {
    if (billed < 0 || cover < 0) throw std::invalid_argument("negative amount");
    if (billed > cover) throw ClaimError("exceeds cover");
    return billed;
}
double settle(double billed, double cover) {
    try { return approve(billed, cover); }
    catch (const ClaimError& e) {
        std::cout << "audit: " << e.what() << '\n';
        throw; // preserves the original dynamic exception type
    }
}
struct ResourceGuard {
    std::string name;
    explicit ResourceGuard(std::string n) : name(std::move(n)) {}
    ~ResourceGuard() noexcept { std::cout << "destroyed " << name << '\n'; }
};
int main() {
    double cases[][2] = {{40000,50000},{60000,50000},{-1,50000}};
    for (auto& c : cases) {
        try {
            ResourceGuard g("claim");
            double paid = settle(c[0],c[1]);
            std::cout << "approved " << paid << '\n';
        } catch (const ClaimError& e) {
            std::cout << "claim rejected: " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "error: " << e.what() << '\n';
        } catch (...) {
            std::cout << "unknown error\n";
        }
    }
    try {
        std::vector<int> huge;
        huge.reserve(huge.max_size()); // may throw length_error or bad_alloc, implementation dependent
    } catch (const std::bad_alloc& e) {
        std::cout << "allocation failed: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cout << "allocation request rejected: " << e.what() << '\n';
    }
}
