#ifndef DATABASE_H
#define DATABASE_H

#include "record.h"

#include <string>
#include <vector>

namespace db {

bool createDatabase(const std::string& filename);
bool appendRecord(const std::string& filename, const AthleteRecord& record);
std::vector<AthleteRecord> readAll(const std::string& filename);
bool overwriteAll(const std::string& filename, const std::vector<AthleteRecord>& records);

std::vector<AthleteRecord> searchById(const std::vector<AthleteRecord>& records, int id);
std::vector<AthleteRecord> searchByCountry(const std::vector<AthleteRecord>& records, const std::string& country);
std::vector<AthleteRecord> searchByDiscipline(const std::vector<AthleteRecord>& records, const std::string& discipline);
std::vector<AthleteRecord> searchComplex(const std::vector<AthleteRecord>& records, const std::string& discipline,
                                         double maxSeconds, int minPoints);

void sortByPoints(std::vector<AthleteRecord>& records);
void sortByResult(std::vector<AthleteRecord>& records);

bool updateById(std::vector<AthleteRecord>& records, int id, const AthleteRecord& updated);
bool removeById(std::vector<AthleteRecord>& records, int id);

void printRecord(const AthleteRecord& record);
void printRecordsTable(const std::vector<AthleteRecord>& records);

void printStatistics(const std::vector<AthleteRecord>& records);

AthleteRecord makeRecordFromInput();
void fillDemoData(const std::string& filename);

} // namespace db

#endif
