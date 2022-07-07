#pragma once

#include <string>
#include <vector>
#include <ctime>
class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

private:
    struct CellStatus {
        bool mine_in = false;
        bool flag_in = false;
        bool opened = false;
    };
    std::pair<std::vector<Cell>, size_t> CheckNeighbours(Cell cell_to_check) const;
    bool CheckWin();
    std::vector<std::vector<CellStatus>> not_rendered_field_ = {};
    GameStatus game_status_ = GameStatus::NOT_STARTED;
    std::time_t start_time_ = 0;
};
