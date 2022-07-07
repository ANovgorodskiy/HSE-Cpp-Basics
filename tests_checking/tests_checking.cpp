#include "tests_checking.h"
#include <deque>
#include <string>
std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<const std::string*> papers;
    std::vector<std::string> students_needed;
    for (const auto& student : student_actions) {
        if (student.side == Side::Top) {
            papers.push_front(&student.name);
        } else {
            papers.push_back(&student.name);
        }
    }
    for (const auto& query : queries) {
        students_needed.push_back(*(papers[query - 1]));
    }
    return students_needed;
}
