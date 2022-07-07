#include "poly.h"
#include <math.h>
#include <iostream>

Poly::Poly(const std::vector<int64_t>& init_vec) {
    for (size_t i = 0; i < init_vec.size(); ++i) {
        if (init_vec[i] != 0) {
            represent_[i] = init_vec[i];
        }
    }
}
Poly::Poly() {
    represent_ = {};
}
Poly::Poly(const std::vector<std::pair<size_t, int64_t>>& init_vec2_d) {
    represent_.clear();
    for (const auto& poly_element : init_vec2_d) {
        represent_[poly_element.first] = poly_element.second;
    }
}
int64_t Poly::operator()(int64_t val) const {
    return GetValue(val);
}

int64_t Poly::GetValue(int64_t val) const {
    int64_t value = 0;
    for (const auto& [deg_of_x, koef] : represent_) {
        value += koef * static_cast<int64_t>(std::pow(val, deg_of_x));
    }
    return value;
}

bool Poly::operator==(const Poly& poly1) const {

    return this->represent_ == poly1.represent_;
}

bool Poly::operator!=(const Poly& poly1) const {
    return !(*this == poly1);
}

void Poly::operator+=(const Poly& poly1) {
    for (const auto& [x_deg, koef] : poly1.represent_) {
        if (this->represent_.contains(x_deg)) {
            if (this->represent_[x_deg] + koef != 0) {
                this->represent_[x_deg] += koef;
            } else {
                this->represent_.erase(x_deg);
            }
        } else {
            this->represent_[x_deg] = koef;
        }
    }
}

Poly Poly::operator+(const Poly& poly1) const {
    Poly sum_prod = *this;
    sum_prod += poly1;
    return sum_prod;
}

Poly Poly::operator-() const {
    Poly negative;
    for (const auto& [x_deg, koef] : this->represent_) {
        negative.represent_[x_deg] = -koef;
    }
    return negative;
}

Poly Poly::operator-(const Poly& poly1) const {
    return *this + -poly1;
}

void Poly::operator-=(const Poly& poly1) {
    *this += (-poly1);
}

Poly Poly::operator*(const Poly& poly1) const {
    Poly product;
    for (const auto [x_deg_1, koef_1] : poly1.represent_) {
        Poly tmp_poly;
        for (const auto [x_deg_2, koef_2] : this->represent_) {
            if (koef_1 * koef_2 != 0) {
                tmp_poly.represent_[x_deg_1 + x_deg_2] = koef_1 * koef_2;
            }
        }
        product += tmp_poly;
    }
    return product;
}

std::ostream& operator<<(std::ostream& out, const Poly& p1) {
    out << "y =";
    if (p1.represent_.empty()) {
        out << " 0";
        return out;
    }
    for (std::map<size_t, int64_t, std::greater<size_t>>::const_iterator it = p1.represent_.begin();
         it != p1.represent_.end(); ++it) {
        if (it->first != 0) {
            out << " " << it->second << "x^" << it->first;
        } else {
            out << " " << it->second;
        }
        auto it_checker = it;
        ++it_checker;
        if (it_checker != p1.represent_.end()) {
            out << " +";
        }
    }

    return out;
}
