#include "database.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

namespace {

void copyString(char* destination, std::size_t size, const std::string& source) {
    std::strncpy(destination, source.c_str(), size - 1);
    destination[size - 1] = '\0';
}

std::string toString(const char* value) {
    return std::string(value);
}

bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) {
        return false;
    }

    for (std::size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

AthleteRecord buildRecord(int id, const std::string& name, const std::string& country, const std::string& discipline,
                          double resultSeconds, int penalties, int points, const std::string& medal) {
    AthleteRecord record{};
    record.id = id;
    copyString(record.name, NAME_SIZE, name);
    copyString(record.country, COUNTRY_SIZE, country);
    copyString(record.discipline, DISCIPLINE_SIZE, discipline);
    record.resultSeconds = resultSeconds;
    record.penalties = penalties;
    record.points = points;
    copyString(record.medal, MEDAL_SIZE, medal);
    return record;
}

} // namespace

namespace db {

bool createDatabase(const std::string& filename) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    return static_cast<bool>(out);
}

bool appendRecord(const std::string& filename, const AthleteRecord& record) {
    std::ofstream out(filename, std::ios::binary | std::ios::app);
    if (!out) {
        return false;
    }
    out.write(reinterpret_cast<const char*>(&record), sizeof(AthleteRecord));
    return static_cast<bool>(out);
}

std::vector<AthleteRecord> readAll(const std::string& filename) {
    std::vector<AthleteRecord> records;
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        return records;
    }

    AthleteRecord record{};
    while (in.read(reinterpret_cast<char*>(&record), sizeof(AthleteRecord))) {
        records.push_back(record);
    }

    return records;
}

bool overwriteAll(const std::string& filename, const std::vector<AthleteRecord>& records) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out) {
        return false;
    }

    for (const auto& record : records) {
        out.write(reinterpret_cast<const char*>(&record), sizeof(AthleteRecord));
    }

    return static_cast<bool>(out);
}

std::vector<AthleteRecord> searchById(const std::vector<AthleteRecord>& records, int id) {
    std::vector<AthleteRecord> result;
    for (const auto& record : records) {
        if (record.id == id) {
            result.push_back(record);
        }
    }
    return result;
}

std::vector<AthleteRecord> searchByCountry(const std::vector<AthleteRecord>& records, const std::string& country) {
    std::vector<AthleteRecord> result;
    for (const auto& record : records) {
        if (equalsIgnoreCase(toString(record.country), country)) {
            result.push_back(record);
        }
    }
    return result;
}

std::vector<AthleteRecord> searchByDiscipline(const std::vector<AthleteRecord>& records, const std::string& discipline) {
    std::vector<AthleteRecord> result;
    for (const auto& record : records) {
        if (equalsIgnoreCase(toString(record.discipline), discipline)) {
            result.push_back(record);
        }
    }
    return result;
}

std::vector<AthleteRecord> searchComplex(const std::vector<AthleteRecord>& records, const std::string& discipline,
                                         double maxSeconds, int minPoints) {
    std::vector<AthleteRecord> result;
    for (const auto& record : records) {
        if (equalsIgnoreCase(toString(record.discipline), discipline) &&
            record.resultSeconds <= maxSeconds &&
            record.points >= minPoints) {
            result.push_back(record);
        }
    }
    return result;
}

void sortByPoints(std::vector<AthleteRecord>& records) {
    std::sort(records.begin(), records.end(), [](const AthleteRecord& a, const AthleteRecord& b) {
        if (a.points == b.points) {
            return a.resultSeconds < b.resultSeconds;
        }
        return a.points > b.points;
    });
}

void sortByResult(std::vector<AthleteRecord>& records) {
    std::sort(records.begin(), records.end(), [](const AthleteRecord& a, const AthleteRecord& b) {
        return a.resultSeconds < b.resultSeconds;
    });
}

bool updateById(std::vector<AthleteRecord>& records, int id, const AthleteRecord& updated) {
    for (auto& record : records) {
        if (record.id == id) {
            record = updated;
            return true;
        }
    }
    return false;
}

bool removeById(std::vector<AthleteRecord>& records, int id) {
    auto oldSize = records.size();
    records.erase(
        std::remove_if(records.begin(), records.end(), [id](const AthleteRecord& record) { return record.id == id; }),
        records.end());
    return records.size() != oldSize;
}

void printRecord(const AthleteRecord& record) {
    std::cout << std::left
              << std::setw(5) << record.id
              << std::setw(24) << record.name
              << std::setw(14) << record.country
              << std::setw(18) << record.discipline
              << std::setw(12) << std::fixed << std::setprecision(2) << record.resultSeconds
              << std::setw(10) << record.penalties
              << std::setw(9) << record.points
              << std::setw(10) << record.medal
              << '\n';
}

void printRecordsTable(const std::vector<AthleteRecord>& records) {
    if (records.empty()) {
        std::cout << "База порожня або записи не знайдені.\n";
        return;
    }

    std::cout << std::left
              << std::setw(5) << "ID"
              << std::setw(24) << "Спортсмен"
              << std::setw(14) << "Країна"
              << std::setw(18) << "Дисципліна"
              << std::setw(12) << "Час(с)"
              << std::setw(10) << "Штрафи"
              << std::setw(9) << "Очки"
              << std::setw(10) << "Медаль"
              << '\n';
    std::cout << std::string(102, '-') << '\n';

    for (const auto& record : records) {
        printRecord(record);
    }
}

void printStatistics(const std::vector<AthleteRecord>& records) {
    if (records.empty()) {
        std::cout << "Немає даних для обчислення статистики.\n";
        return;
    }

    double totalPoints = 0.0;
    double totalTime = 0.0;
    int gold = 0;
    for (const auto& record : records) {
        totalPoints += record.points;
        totalTime += record.resultSeconds;
        if (equalsIgnoreCase(toString(record.medal), "Gold")) {
            ++gold;
        }
    }

    std::cout << "Кількість записів: " << records.size() << '\n';
    std::cout << "Середній результат (сек): " << std::fixed << std::setprecision(2)
              << (totalTime / records.size()) << '\n';
    std::cout << "Середні очки: " << std::fixed << std::setprecision(2)
              << (totalPoints / records.size()) << '\n';
    std::cout << "Кількість золотих медалей: " << gold << '\n';
}

AthleteRecord makeRecordFromInput() {
    AthleteRecord record{};
    std::string line;

    std::cout << "ID: ";
    std::cin >> record.id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "ПІБ спортсмена: ";
    std::getline(std::cin, line);
    copyString(record.name, NAME_SIZE, line);

    std::cout << "Країна: ";
    std::getline(std::cin, line);
    copyString(record.country, COUNTRY_SIZE, line);

    std::cout << "Дисципліна: ";
    std::getline(std::cin, line);
    copyString(record.discipline, DISCIPLINE_SIZE, line);

    std::cout << "Результат у секундах: ";
    std::cin >> record.resultSeconds;

    std::cout << "К-сть штрафів: ";
    std::cin >> record.penalties;

    std::cout << "Очки: ";
    std::cin >> record.points;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Медаль (Gold/Silver/Bronze/None): ";
    std::getline(std::cin, line);
    copyString(record.medal, MEDAL_SIZE, line);

    return record;
}

void fillDemoData(const std::string& filename) {
    const std::vector<AthleteRecord> demo = {
        buildRecord(1, "Andriy Kovalenko", "Ukraine", "100m Sprint", 10.34, 0, 980, "Gold"),
        buildRecord(2, "Olena Melnuk", "Ukraine", "200m Sprint", 22.75, 0, 970, "Silver"),
        buildRecord(3, "John Smith", "USA", "100m Sprint", 10.41, 1, 940, "Bronze"),
        buildRecord(4, "Marie Dubois", "France", "400m", 50.88, 0, 965, "Gold"),
        buildRecord(5, "Lukas Meyer", "Germany", "400m", 51.22, 0, 948, "None"),
        buildRecord(6, "Kenji Tanaka", "Japan", "800m", 107.45, 0, 951, "Silver"),
        buildRecord(7, "Marta Rossi", "Italy", "1500m", 239.18, 0, 959, "Gold"),
        buildRecord(8, "Ana Silva", "Portugal", "1500m", 242.03, 1, 930, "Bronze"),
        buildRecord(9, "Nora Jensen", "Denmark", "5000m", 892.64, 0, 962, "Gold"),
        buildRecord(10, "Iryna Bondar", "Ukraine", "5000m", 899.27, 0, 951, "Silver"),
        buildRecord(11, "David Brown", "UK", "Marathon", 7662.10, 0, 968, "Gold"),
        buildRecord(12, "Paolo Ricci", "Italy", "Marathon", 7721.52, 0, 955, "Silver"),
        buildRecord(13, "Sofia Novak", "Poland", "110m Hurdles", 12.95, 0, 976, "Gold"),
        buildRecord(14, "Carlos Diaz", "Spain", "110m Hurdles", 13.18, 1, 939, "Bronze"),
        buildRecord(15, "Emma Wilson", "Canada", "Long Jump", 6.91, 0, 972, "Gold"),
        buildRecord(16, "Danylo Petrenko", "Ukraine", "Long Jump", 6.83, 0, 957, "Silver"),
        buildRecord(17, "Ali Hassan", "Egypt", "Shot Put", 21.44, 0, 966, "Gold"),
        buildRecord(18, "Mia Larsen", "Norway", "Shot Put", 20.91, 0, 949, "Bronze"),
        buildRecord(19, "Rita Costa", "Brazil", "High Jump", 1.96, 0, 963, "Silver"),
        buildRecord(20, "Yaroslav Hnat", "Ukraine", "High Jump", 2.01, 0, 978, "Gold")
    };

    overwriteAll(filename, demo);
}

} // namespace db
