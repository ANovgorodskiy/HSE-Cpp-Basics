#include "sort_students.h"
#include <tuple>
bool DateCompare(const Student& first, const Student& second);

bool NameCompare(const Student& first, const Student& second){
    if (std::tie(first.last_name, first.name) != std::tie(second.last_name, second.name)){
        return std::tie(first.last_name, first.name) < std::tie(second.last_name, second.name);
    } else{
        return DateCompare(first, second);
    }
}

bool DateCompare(const Student& first, const Student& second){
    if (std::tie(first.birth_date.year, first.birth_date.month,first.birth_date.day) !=
        std::tie(second.birth_date.year, second.birth_date.month,second.birth_date.day)){
        return std::tie(first.birth_date.year, first.birth_date.month,first.birth_date.day) <
                    std::tie(second.birth_date.year, second.birth_date.month,second.birth_date.day);
    } else{
        return NameCompare(first, second);
    }
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    if (sortKind == SortKind::Date){
        std::sort(students.begin(), students.end(), DateCompare);

    } else{
        std::sort(students.begin(), students.end(), NameCompare);

    }
}
