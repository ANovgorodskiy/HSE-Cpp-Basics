#include "search.h"
#include <iostream>
#include <math.h>

bool CompareRelevants(const Relevant& rel1, const Relevant& rel2) {
    return std::tie(rel1.rel, rel2.string_num) > std::tie(rel2.rel, rel1.string_num);
}

void SearchEngine::BuildIndex(std::string_view text) {
    original_text_ = text;
    text_index_.clear();
    line_starters_.clear();
    std::vector<std::string_view> most_relevant;
    size_t pos_endl = 0;
    size_t position_endl = 0;

    while (pos_endl != std::string::npos) {
        if (pos_endl != 0) {
            pos_endl += 1;
        }
        position_endl = text.find("\n", pos_endl);
        std::string_view temporary_string = text.substr(pos_endl, position_endl - pos_endl);
        size_t pos_space = 0;
        size_t position_space = 0;
        std::vector<std::string> vector_of_line_words;
        while (pos_space != std::string::npos) {
            pos_space += 1;
            position_space = temporary_string.find(" ", pos_space);
            std::string_view temporary_word = temporary_string.substr(pos_space - 1, position_space - pos_space + 1);
            std::string word;
            size_t pushed_back = 0;
            for (auto const& sym : temporary_word) {
                if (std::isalpha(sym)) {
                    word.push_back(std::tolower(sym));
                    ++pushed_back;
                }
            }
            if (pushed_back != 0) {
                vector_of_line_words.push_back(word);
            }
            pos_space = position_space;
        }
        if (!vector_of_line_words.empty()) {
            text_index_.push_back(vector_of_line_words);
            line_starters_.push_back(pos_endl);
        }
        pos_endl = position_endl;
    }
}

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    size_t pos_space = 0;
    size_t position_space = 0;
    std::vector<std::string> query_splitted;
    std::unordered_map<std::string, WordCharact> num_lines_with_word;
    std::vector<std::string_view> most_relevant;
    while (pos_space != std::string::npos) {
        pos_space += 1;
        position_space = query.find(" ", pos_space);
        std::string_view temporary_query = query.substr(pos_space - 1, position_space - pos_space + 1);
        std::string word;
        for (const auto& sym : temporary_query) {
            if (std::isalpha(sym)) {
                word.push_back(std::tolower(sym));
            }
        }
        query_splitted.push_back(word);
        pos_space = position_space;
    }
    for (const auto& word : query_splitted) {
        num_lines_with_word[word].times_in_lines = 0;
        for (size_t t = 0; t < text_index_.size(); ++t) {
            num_lines_with_word[word].times_in_line.push_back(0);
        }
    }
    bool not_added = true;
    size_t line_num = 0;
    for (const auto& line : text_index_) {
        not_added = true;
        for (const auto& word : line) {
            if (num_lines_with_word.find(word) != num_lines_with_word.end()) {
                if (not_added) {
                    ++num_lines_with_word[word].times_in_lines;
                    not_added = false;
                }
                ++num_lines_with_word[word].times_in_line[line_num];
            }
        }
        ++line_num;
    }
    std::vector<Relevant> relevants;
    for (size_t i = 0; i < text_index_.size(); ++i) {
        double relevance = 0;
        for (auto const& [query_word, query_word_character] : num_lines_with_word) {
            if (query_word_character.times_in_line[i] != 0) {
                double tf = (static_cast<double>(query_word_character.times_in_line[i]) /
                             static_cast<double>(text_index_[i].size()));
                double idf = (std::log(static_cast<double>(text_index_.size()) /
                                       static_cast<double>(query_word_character.times_in_lines)));
                relevance += tf * idf;
            }
        }
        relevants.emplace_back(Relevant(i, relevance));
    }
    std::sort(relevants.begin(), relevants.end(), CompareRelevants);
    for (size_t i = 0; i < results_count; ++i) {
        if (relevants[i].rel == 0) {
            break;
        }
        size_t finder = original_text_.find("\n", line_starters_[relevants[i].string_num]);
        if (finder != std::string::npos) {
            most_relevant.push_back(original_text_.substr(line_starters_[relevants[i].string_num],
                                                          finder - line_starters_[relevants[i].string_num]));
        } else {
            most_relevant.push_back(
                original_text_.substr(line_starters_[relevants[i].string_num],
                                      original_text_.size() - line_starters_[relevants[i].string_num]));
        }
    }
    return most_relevant;
}