#include <iomanip>
#include <iostream>
#include <string>

class Cart {
    std::string items_[10];
    double prices_[10]{};
    int n_ = 0;
    double discount_ = 0.0;

public:
    Cart& add(const std::string& item, double price) {
        if (n_ < 10 && price >= 0.0) {
            items_[n_] = item;
            prices_[n_] = price;
            ++n_;
        }
        return *this;
    }

    Cart& setDiscount(double discount) {
        if (discount >= 0.0 && discount <= 100.0)
            this->discount_ = discount;
        return *this;
    }

    double total() const {
        double sum = 0.0;
        for (int i = 0; i < n_; ++i) sum += prices_[i];
        return sum * (100.0 - discount_) / 100.0;
    }
};

int main() {
    Cart direct;
    direct.add("Rice", 60).add("Dal", 120).setDiscount(10);

    Cart* pointer = &direct;
    pointer->add("Oil", 180);

    Cart referenceCart;
    Cart& reference = referenceCart;
    reference.add("Tea", 40);

    Cart* dynamicCart = new Cart;
    dynamicCart->add("Book", 100);
    std::cout << "Dynamic cart total: " << dynamicCart->total() << '\n';
    delete dynamicCart;

    Cart first, second, third;
    first.add("A", 50);
    second.add("B", 300);
    third.add("C", 150);
    Cart* active[] = {&first, &second, &third};
    Cart* highest = active[0];
    for (Cart** p = active; p != active + 3; ++p)
        if ((*p)->total() > highest->total()) highest = *p;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Chained cart total: " << pointer->total() << '\n';
    std::cout << "Highest active cart total: " << highest->total() << '\n';
}
