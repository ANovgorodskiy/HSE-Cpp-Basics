#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <functional>

using PolyRepr = std::map<size_t, int64_t, std::greater<size_t>>;

class Poly {
public:
    friend std::ostream& operator<<(std::ostream&, const Poly& p1);
    explicit Poly();

    Poly(const std::vector<int64_t>& init_vec);

    Poly(const std::vector<std::pair<size_t, int64_t>>& init_vec2_d);

    int64_t operator()(int64_t val) const;

    bool operator==(const Poly& poly1) const;
    bool operator!=(const Poly& poly1) const;
    Poly operator+(const Poly& poly1) const;
    Poly operator-(const Poly& poly1) const;
    Poly operator*(const Poly& poly1) const;
    Poly operator-() const;
    void operator-=(const Poly& poly1);
    void operator+=(const Poly& poly1);

private:
    PolyRepr represent_;
    int64_t GetValue(int64_t val) const;
};
