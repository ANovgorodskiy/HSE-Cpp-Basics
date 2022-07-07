#pragma once
#include <iostream>
#include <string>
#include <vector>
struct FilterParams {
    std::string name;
    std::vector<std::string> params;
};

struct ParsingResults {
    std::string path_in;
    std::string path_out;
    std::vector<FilterParams> filters;
};

class ArgumentParser {
public:
    static ParsingResults Parse(int argc, const char *argv[]);
};