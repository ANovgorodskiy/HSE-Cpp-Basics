#include "ArgumentParser.h"
ParsingResults ArgumentParser::Parse(int argc, const char **argv) {
    if (argc > 2) {
        ParsingResults parse_results;
        parse_results.path_in = argv[1];
        parse_results.path_out = argv[2];
        int i = 3;
        while (i < argc) {
            if (argv[i][0] == '-') {
                FilterParams tmp_filter;
                tmp_filter.name = argv[i];
                ++i;
                while (i < argc && argv[i][0] != '-') {
                    tmp_filter.params.push_back(argv[i]);
                    ++i;
                }
                --i;
                parse_results.filters.push_back(tmp_filter);
            }
            ++i;
        }
        return parse_results;
    } else {
        throw std::exception();
    }
}
