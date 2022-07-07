#include "FilterController.h"

std::shared_ptr<Filter> CreateFilter(const FilterParams& parameter) {
    if (parameter.name == "-crop") {
        if (parameter.params.size() != 2) {
            throw std::runtime_error("crop has only two parameters");
        }
        return std::make_shared<Crop>(std::stoull(parameter.params[0]), std::stoull(parameter.params[1]));
    }
    if (parameter.name == "-gs") {
        if (parameter.params.size() != 0) {
            throw std::runtime_error("Grayscale has no parameters");
        }
        return std::make_shared<Grayscale>();
    }
    if (parameter.name == "-sharp") {
        if (parameter.params.size() != 0) {
            throw std::runtime_error("sharp has no parameters");
        }
        return std::make_shared<SharpeningFilter>();
    }
    if (parameter.name == "-edge") {
        if (parameter.params.size() != 1) {
            throw std::runtime_error("edhe has only one parameter");
        }
        return std::make_shared<EdgeThreshold>(std::stod(parameter.params[0]));
    }
    if (parameter.name == "-neg") {
        if (parameter.params.size() != 0) {
            throw std::runtime_error("negative has no parameters");
        }
        return std::make_shared<Negative>();
    }
    if (parameter.name == "-blur") {
        ;
        if (parameter.params.size() != 1) {
            throw std::runtime_error("blur has only one parameter");
        }
        return std::make_shared<GaussianBlur>(std::stod(parameter.params[0]));
    }
    if (parameter.name == "-HIBlur") {
        if (parameter.params.size() != 0) {
            throw std::runtime_error("integral blur has no parameters");
        }
        return std::make_shared<HorizontalIntegralBlur>();
    }
    throw std::runtime_error("No filter named " + parameter.name);
}

std::vector<std::shared_ptr<Filter>> CreateFilters(const std::vector<FilterParams>& parameters) {
    std::vector<std::shared_ptr<Filter>> filters;
    filters.reserve(parameters.size());
    for (const auto& parameter : parameters) {
        try {
            filters.push_back(CreateFilter(parameter));
        } catch (std::runtime_error const& exception) {
            std::cerr << exception.what() << std::endl;
            std::cerr << "filter you typed not added to filter queue" << std::endl;
        }
    }
    return filters;
}

Controller::Controller(int argc, const char** argv) {
    ParsingResults parse_res = ArgumentParser::Parse(argc, argv);
    path_in_ = parse_res.path_in;
    path_out_ = parse_res.path_out;
    filters_ = CreateFilters(parse_res.filters);
}

void Controller::ApplyFilters(Image& image) {
    for (const auto& filter : filters_) {
        filter->Apply(image);
    }
}

Image Controller::Read() {
    try {
        auto image = BMP::Read(path_in_);
        return image;
    } catch (std::runtime_error const& exception) {
        std::cerr << exception.what() << std::endl;
        return Image(10, 10);
    }
}

void Controller::Write(Image& image) {
    try {
        BMP::Write(path_out_, image);
    } catch (std::runtime_error const& exception) {
        std::cerr << exception.what() << std::endl;
    }
}

Controller::Controller(std::string& path_in, std::string& path_out, std::vector<std::shared_ptr<Filter>>& filters) {
    path_in_ = path_in;
    path_out_ = path_out;
    filters_ = filters;
}
