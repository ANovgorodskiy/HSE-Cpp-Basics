#include "scorer.h"
#include <unordered_map>
#include <iostream>
struct Checker {
    bool succesfully_checked = false;
    bool merge_opened = false;
};

bool CompareEventByTime(const Event* const event1, const Event* const event2) {
    return event1->time < event2->time;
}

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::vector<const Event*> events_sorted;
    for (const auto& event : events) {
        events_sorted.push_back(&event);
    }
    std::sort(events_sorted.begin(), events_sorted.end(), CompareEventByTime);

    std::map<StudentName, std::set<TaskName>> score_table;
    std::unordered_map<StudentName, std::unordered_map<TaskName, Checker>> table;
    for (const auto& event : events_sorted) {
        if (event->time <= score_time) {
            if (event->event_type == EventType::MergeRequestOpen) {
                table[event->student_name][event->task_name].merge_opened = true;
            } else if (event->event_type == EventType::MergeRequestClosed) {
                table[event->student_name][event->task_name].merge_opened = false;
            } else if (event->event_type == EventType::CheckSuccess) {
                table[event->student_name][event->task_name].succesfully_checked = true;
            } else {
                table[event->student_name][event->task_name].succesfully_checked = false;
            }
        } else {
            break;
        }
    }
    for (const auto& [student_name, tasks] : table) {
        for (const auto& task : tasks) {
            if (task.second.succesfully_checked && !task.second.merge_opened) {
                score_table[student_name].insert(task.first);
            }
        }
    }
    return score_table;
}
