#include "minesweeper.h"
#include <random>
#include <queue>
#include <iostream>
Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    game_status_ = GameStatus::NOT_STARTED;
    std::vector<std::pair<size_t, size_t>> positions;
    for (size_t i = 0; i < height; ++i) {
        std::vector<CellStatus> tmp_vec;
        for (size_t j = 0; j < width; ++j) {
            tmp_vec.push_back(CellStatus());  // be sure def construct working fine
        }
        not_rendered_field_.push_back(tmp_vec);
    }
    std::random_device random_device;
    std::mt19937 generator(random_device());
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            positions.emplace_back(i, j);
        }
    }
    std::shuffle(positions.begin(), positions.end(), generator);
    for (const auto& position : positions) {
        not_rendered_field_.at(position.second)[position.first].mine_in = true;
        --mines_count;
        if (mines_count == 0) {
            break;
        }
    }
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    game_status_ = GameStatus::NOT_STARTED;
    for (size_t i = 0; i < height; ++i) {
        std::vector<CellStatus> tmp_vec;
        for (size_t j = 0; j < width; ++j) {
            tmp_vec.push_back(CellStatus());
        }
        not_rendered_field_.push_back(tmp_vec);
    }
    for (const auto& [cell_y, cell_x] : cells_with_mines) {
        not_rendered_field_.at(cell_x)[cell_y].mine_in = true;
    }
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    not_rendered_field_.clear();
    game_status_ = GameStatus::NOT_STARTED;
    std::vector<std::pair<size_t, size_t>> positions;
    for (size_t i = 0; i < height; ++i) {
        std::vector<CellStatus> tmp_vec;
        for (size_t j = 0; j < width; ++j) {
            tmp_vec.push_back(CellStatus());  // be sure def construct working fine
        }
        not_rendered_field_.push_back(tmp_vec);
    }
    std::random_device random_device;
    std::mt19937 generator(random_device());
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            positions.emplace_back(i, j);
        }
    }
    std::shuffle(positions.begin(), positions.end(), generator);
    for (const auto& position : positions) {
        not_rendered_field_.at(position.second)[position.first].mine_in = true;
        --mines_count;
        if (mines_count == 0) {
            break;
        }
    }
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    game_status_ = GameStatus::NOT_STARTED;
    not_rendered_field_.clear();
    for (size_t i = 0; i < height; ++i) {
        std::vector<CellStatus> tmp_vec;
        for (size_t j = 0; j < width; ++j) {
            tmp_vec.push_back(CellStatus());
        }
        not_rendered_field_.push_back(tmp_vec);
    }
    for (const auto& [cell_y, cell_x] : cells_with_mines) {
        not_rendered_field_.at(cell_x)[cell_y].mine_in = true;
    }
}

std::pair<std::vector<Minesweeper::Cell>, size_t> Minesweeper::CheckNeighbours(Cell cell_to_check) const {
    std::vector<Minesweeper::Cell> good_neighbours;
    size_t true_cells_around = 0;
    for (int coord_y = static_cast<int>(cell_to_check.y) - 1; coord_y <= static_cast<int>(cell_to_check.y) + 1;
         ++coord_y) {
        if (coord_y >= 0 && coord_y < not_rendered_field_.size()) {
            for (int coord_x = static_cast<int>(cell_to_check.x) - 1; coord_x <= static_cast<int>(cell_to_check.x) + 1;
                 ++coord_x) {
                if (coord_x >= 0 && coord_x < not_rendered_field_.at(0).size()) {
                    ++true_cells_around;
                    if (not_rendered_field_.at(coord_y)[coord_x].mine_in == false) {
                        good_neighbours.push_back(
                            {.x = static_cast<size_t>(coord_x), .y = static_cast<size_t>(coord_y)});
                    }
                }
            }
        }
    }
    return std::make_pair(good_neighbours, true_cells_around);
}
void Minesweeper::MarkCell(const Cell& cell) {
    if (game_status_ != GameStatus::DEFEAT && game_status_ != GameStatus::VICTORY) {
        game_status_ = GameStatus::IN_PROGRESS;
        if (not_rendered_field_.at(cell.y)[cell.x].flag_in) {
            not_rendered_field_.at(cell.y)[cell.x].flag_in = false;
        } else {
            not_rendered_field_.at(cell.y)[cell.x].flag_in = true;
        }
    }
}

void Minesweeper::OpenCell(const Cell& cell) {
    if (game_status_ == GameStatus::NOT_STARTED) {
        start_time_ = std::time(nullptr);
    }
    if (game_status_ != GameStatus::DEFEAT && game_status_ != GameStatus::VICTORY) {
        if (not_rendered_field_.at(cell.y)[cell.x].flag_in == false) {
            if (not_rendered_field_.at(cell.y)[cell.x].mine_in == true) {
                game_status_ = GameStatus::DEFEAT;
                for (auto& line : not_rendered_field_) {
                    for (auto& cell_to_lose : line) {
                        cell_to_lose.opened = true;
                    }
                }
            } else {
                auto check_result = CheckNeighbours({cell});
                std::vector<Cell> not_mined_neighbours = check_result.first;
                size_t amount_of_cells_around = check_result.second;
                if (not_mined_neighbours.size() != amount_of_cells_around) {
                    not_rendered_field_.at(cell.y)[cell.x].opened = true;
                } else {
                    std::queue<Cell> bfs_q;
                    for (const auto& neighbour : not_mined_neighbours) {
                        if (!not_rendered_field_.at(neighbour.y)[neighbour.x].opened &&
                            !not_rendered_field_.at(neighbour.y)[neighbour.x].flag_in) {
                            bfs_q.push(neighbour);
                        }
                    }
                    while (!bfs_q.empty()) {
                        Cell tmp_cell = bfs_q.front();
                        auto tmp_check_result = CheckNeighbours(tmp_cell);
                        std::vector<Cell> tmp_not_mined_cells;
                        tmp_not_mined_cells = tmp_check_result.first;
                        size_t tmp_amount_of_cells = tmp_check_result.second;
                        if (tmp_not_mined_cells.size() == tmp_amount_of_cells) {
                            for (const auto& good_cell : tmp_not_mined_cells) {
                                if (!not_rendered_field_.at(good_cell.y)[good_cell.x].opened &&
                                    !not_rendered_field_.at(good_cell.y)[good_cell.x].flag_in) {
                                    bfs_q.push(good_cell);
                                }
                            }
                        }
                        not_rendered_field_.at(tmp_cell.y)[tmp_cell.x].opened = true;
                        bfs_q.pop();
                    }
                }
                if (CheckWin()) {
                    game_status_ = GameStatus::VICTORY;
                } else {
                    game_status_ = GameStatus::IN_PROGRESS;
                }
            }
        }
    }
}

bool Minesweeper::CheckWin() {
    for (const auto& line : not_rendered_field_) {
        for (const auto& cell : line) {
            if (!cell.mine_in && !cell.opened) {
                return false;
            }
        }
    }
    return true;
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    RenderedField rendered_field;
    size_t y_coord = 0;
    for (const auto& line : not_rendered_field_) {
        std::string line_of_field;
        size_t x_coord = 0;
        for (const auto& cell : line) {
            if (cell.flag_in) {
                line_of_field += '?';
            } else if (cell.opened) {
                if (cell.mine_in) {
                    line_of_field += '*';
                } else {
                    auto check_result = CheckNeighbours({.x = x_coord, .y = y_coord});
                    size_t neighs = check_result.first.size();
                    if (check_result.second - neighs != 0) {
                        line_of_field += std::to_string(check_result.second - neighs);
                    } else {
                        line_of_field += '.';
                    }
                }
            } else {
                line_of_field += '-';
            }
            ++x_coord;
        }
        rendered_field.push_back(line_of_field);
        ++y_coord;
    }
    return rendered_field;
}

time_t Minesweeper::GetGameTime() const {
    return std::time(nullptr) - start_time_;
}
