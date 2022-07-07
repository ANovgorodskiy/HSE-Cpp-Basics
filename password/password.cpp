#include "password.h"
#include <cctype>
#include <numeric>
#include <vector>

bool ValidatePassword(const std::string& password) {
    std::vector<bool> classes = {false, false, false, false};
    const std::size_t min_passw_length = 8;
    const std::size_t max_passw_length = 14;
    const int min_sym_val = 33;
    const int max_sym_val = 126;
    const int min_diff_classes = 3;
    if (password.length() < min_passw_length || password.length() > max_passw_length) {
        return false;
    }
    for (auto symbol : password) {
        if (static_cast<int>(symbol) < min_sym_val || static_cast<int>(symbol) > max_sym_val) {
            return false;
        }
        if (std::islower(symbol)) {
            classes[0] = true;
        } else if (std::isupper(symbol)) {
            classes[1] = true;
        } else if (std::isdigit(symbol)) {
            classes[2] = true;
        } else {
            classes[3] = true;
        }
    }
    return (std::accumulate(classes.begin(), classes.end(), 0) >= min_diff_classes);
}
