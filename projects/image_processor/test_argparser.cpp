#include <catch.hpp>
#include "ArgumentParser.h"

TEST_CASE("Argument parsing"){
    int argnum = 6;
    const char* arguments[7] = {"command_name",
                                "/dir_in",
                                "/dir_out",
                                "-gs",
                                "-blur",
                                "3",
                                 "-HIBlur"};
    ParsingResults results;
    results = ArgumentParser::Parse(argnum, arguments);
    FilterParams gs;
    gs.name = "-gs";
    REQUIRE("/dir_in" == results.path_in);
    REQUIRE("/dir_out" == results.path_out);
    REQUIRE("3" == results.filters[1].params[0]);
    REQUIRE(gs.name == results.filters[0].name);
}