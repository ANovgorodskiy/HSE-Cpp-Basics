#include "admission.h"
#include <vector>
#include <unordered_map>
#include <iostream>
bool CompareApplicants(const Applicant* const a1, const Applicant* const a2) {
    auto comp1 = std::tie(a2->points, a1->student.birth_date.year, a1->student.birth_date.month,
                          a1->student.birth_date.day, a1->student.name);
    auto comp2 = std::tie(a1->points, a2->student.birth_date.year, a2->student.birth_date.month,
                          a2->student.birth_date.day, a2->student.name);
    return comp1 < comp2;
}

bool CompareStudents(const Student* student1, const Student* student2) {
    auto bday_and_name_1 =
        std::tie(student1->name, student1->birth_date.year, student1->birth_date.month, student1->birth_date.day);
    auto bday_and_name_2 =
        std::tie(student2->name, student2->birth_date.year, student2->birth_date.month, student2->birth_date.day);
    return bday_and_name_1 < bday_and_name_2;
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> applicants_to_sort;
    std::unordered_map<std::string, std::vector<const Student*>> admission_table;
    std::unordered_map<std::string, int> least_university_places;
    for (const auto& uni : universities) {
        least_university_places[uni.name] = uni.max_students;
        admission_table[uni.name] = {};
    }
    for (const auto& appl : applicants) {
        applicants_to_sort.push_back(&appl);
    }
    std::sort(applicants_to_sort.begin(), applicants_to_sort.end(), CompareApplicants);
    for (auto appl : applicants_to_sort) {
        for (auto wish : appl->wish_list) {
            if (least_university_places[wish] > 0) {
                admission_table[wish].push_back(&(appl->student));
                --least_university_places[wish];
                break;
            }
        }
    }
    for (auto& key_val : admission_table) {
        std::sort(admission_table[key_val.first].begin(), admission_table[key_val.first].end(), CompareStudents);
    }
    return admission_table;
}
