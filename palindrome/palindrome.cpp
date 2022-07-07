#include "palindrome.h"
#include <algorithm>
#include <string>
bool IsPalindrome(const std::string& str) {
    std::string no_space;
    for (auto sym: str){
        if (sym != ' '){
            no_space += sym;
        }
    }
    std::string no_space_reversed = no_space;
    std::reverse(no_space.begin(), no_space.end());
    return (no_space_reversed == no_space);
}
