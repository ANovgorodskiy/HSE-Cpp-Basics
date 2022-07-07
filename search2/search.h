#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
struct Relevant {
    Relevant(size_t string_num, double rel) : string_num(string_num), rel(rel) {
    }
    size_t string_num = 0;
    double rel = 0.00;
};

struct WordCharact {
    int times_in_lines;
    std::vector<int> times_in_line;
};

class SearchEngine {
public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;

private:
    std::string_view original_text_;
    std::vector<std::vector<std::string>> text_index_;
    std::vector<size_t> line_starters_;
};