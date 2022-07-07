#include "scorer.h"
void Scorer::OnCheckFailed(const StudentName &student_name, const TaskName &task_name) {
    score_table_with_succes_info_[student_name][task_name].succesfully_checked = false;
}
void Scorer::OnCheckSuccess(const StudentName &student_name, const TaskName &task_name) {
    score_table_with_succes_info_[student_name][task_name].succesfully_checked = true;
}
void Scorer::OnMergeRequestClosed(const StudentName &student_name, const TaskName &task_name) {
    score_table_with_succes_info_[student_name][task_name].merge_opened = false;
}
void Scorer::OnMergeRequestOpen(const StudentName &student_name, const TaskName &task_name) {
    score_table_with_succes_info_[student_name][task_name].merge_opened = true;
}
void Scorer::Reset() {
    score_table_with_succes_info_.clear();
}

ScoreTable Scorer::GetScoreTable() const {
    ScoreTable score_table;
    for (const auto &[student_name, tasks] : score_table_with_succes_info_) {
        for (const auto &[task_name, checker] : tasks) {
            if (!checker.merge_opened && checker.succesfully_checked) {
                score_table[student_name].insert(task_name);
            }
        }
    }
    return score_table;
}