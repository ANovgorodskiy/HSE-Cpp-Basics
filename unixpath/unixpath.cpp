#include "unixpath.h"
#include <vector>

std::vector<std::string> SplitPath(std::string_view path_to_split) {
    std::vector<std::string> path_splitted;
    size_t current_splitter_position = 0;
    while (true) {
        size_t slash_pos = path_to_split.find('/', current_splitter_position);
        if (slash_pos == std::string::npos) {
            if (path_to_split.size() - current_splitter_position > 0) {
                std::string part_of_path = (static_cast<std::string>(
                    path_to_split.substr(current_splitter_position, path_to_split.size() - current_splitter_position)));
                if (part_of_path != ".") {
                    path_splitted.push_back(part_of_path);
                }
            }
            return path_splitted;
        }
        if (slash_pos - current_splitter_position > 0) {
            std::string part_of_path = (static_cast<std::string>(
                path_to_split.substr(current_splitter_position, slash_pos - current_splitter_position)));

            if (part_of_path != ".") {
                path_splitted.push_back(part_of_path);
            }
        }
        current_splitter_position = slash_pos + 1;
    }
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::vector<std::string> path_splitted = SplitPath(path);
    std::vector<std::string> current_dir_splitted = SplitPath(current_working_dir);
    std::string normalized_path;
    bool absolute = false;
    if (path.starts_with('/')) {
        absolute = true;
    }
    size_t skip_counter = 0;
    for (size_t j = path_splitted.size(); j > 0; --j) {
        if (path_splitted[j - 1] == "..") {
            ++skip_counter;
        } else {
            if (skip_counter == 0) {
                normalized_path = '/' + path_splitted[j - 1] + normalized_path;
            } else {
                --skip_counter;
            }
        }
    }
    if (!absolute) {
        for (size_t j = current_dir_splitted.size(); j > 0; --j) {
            if (skip_counter == 0) {
                normalized_path = '/' + current_dir_splitted[j - 1] + normalized_path;
            } else {
                --skip_counter;
            }
        }
    }
    if (normalized_path.ends_with('/')) {
        normalized_path.pop_back();
    }
    if (normalized_path.empty()) {
        return "/";
    }
    return normalized_path;
}
