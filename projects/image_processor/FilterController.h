#include "Image.h"
#include "Filters.h"
#include "ArgumentParser.h"
#include <memory>

std::shared_ptr<Filter> CreateFilter(const FilterParams& parameter);

std::vector<std::shared_ptr<Filter>> CreateFilters(const std::vector<FilterParams>& parameters);

class Controller {
public:
    Controller(int argc, const char** argv);
    Controller(std::string& path_in, std::string& path_out, std::vector<std::shared_ptr<Filter>>& filters);
    void ApplyFilters(Image& image);
    Image Read();
    void Write(Image& image);

private:
    std::string path_in_;
    std::string path_out_;
    std::vector<std::shared_ptr<Filter>> filters_;
};