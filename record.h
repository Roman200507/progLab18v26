#ifndef RECORD_H
#define RECORD_H

#include <cstddef>

constexpr std::size_t NAME_SIZE = 50;
constexpr std::size_t COUNTRY_SIZE = 30;
constexpr std::size_t DISCIPLINE_SIZE = 40;
constexpr std::size_t MEDAL_SIZE = 12;

struct AthleteRecord {
    int id;
    char name[NAME_SIZE];
    char country[COUNTRY_SIZE];
    char discipline[DISCIPLINE_SIZE];
    double resultSeconds;
    int penalties;
    int points;
    char medal[MEDAL_SIZE];
};

#endif
