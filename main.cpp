// ЛР 18, В 26, СІ-11
#include "database.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

namespace {

void setupConsoleForCyrillic() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
}

void printMenu() {
    cout << "\n===== База даних спортивного змагання =====\n"
         << "1. Створити новий файл БД\n"
         << "2. Додати запис з клавіатури\n"
         << "3. Вивести всі записи\n"
         << "4. Пошук за ID\n"
         << "5. Пошук за країною\n"
         << "6. Пошук за дисципліною\n"
         << "7. Складний пошук (дисципліна + макс. час + мін. очки)\n"
         << "8. Сортувати за очками та зберегти\n"
         << "9. Сортувати за результатом та зберегти\n"
         << "10. Змінити запис за ID\n"
         << "11. Видалити запис за ID\n"
         << "12. Підсумкова статистика\n"
         << "13. Заповнити БД демо-даними (20 записів)\n"
         << "14. Змінити ім'я файлу БД\n"
         << "0. Вихід\n"
         << "Ваш вибір: ";
}

string requestFilename() {
    string filename;
    cout << "Введіть ім'я файлу БД (наприклад, competition.dat): ";
    getline(cin, filename);
    return filename;
}

} // namespace

int main() {
    setupConsoleForCyrillic();

    cout << "Лабораторна робота №18, варіант 26\n";
    cout << "Тема: База даних спортивного змагання (легка атлетика)\n";

    string filename = requestFilename();

    int choice = -1;
    while (choice != 0) {
        printMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            if (db::createDatabase(filename)) {
                cout << "Файл БД створено: " << filename << '\n';
            } else {
                cout << "Помилка створення файлу.\n";
            }
        } else if (choice == 2) {
            AthleteRecord record = db::makeRecordFromInput();
            if (db::appendRecord(filename, record)) {
                cout << "Запис додано.\n";
            } else {
                cout << "Помилка додавання запису.\n";
            }
        } else if (choice == 3) {
            db::printRecordsTable(db::readAll(filename));
        } else if (choice == 4) {
            int id;
            cout << "Введіть ID: ";
            cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            db::printRecordsTable(db::searchById(db::readAll(filename), id));
        } else if (choice == 5) {
            string country;
            cout << "Введіть країну: ";
            getline(cin, country);
            db::printRecordsTable(db::searchByCountry(db::readAll(filename), country));
        } else if (choice == 6) {
            string discipline;
            cout << "Введіть дисципліну: ";
            getline(cin, discipline);
            db::printRecordsTable(db::searchByDiscipline(db::readAll(filename), discipline));
        } else if (choice == 7) {
            string discipline;
            double maxSeconds;
            int minPoints;
            cout << "Введіть дисципліну: ";
            getline(cin, discipline);
            cout << "Максимальний результат (сек): ";
            cin >> maxSeconds;
            cout << "Мінімальні очки: ";
            cin >> minPoints;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            db::printRecordsTable(db::searchComplex(db::readAll(filename), discipline, maxSeconds, minPoints));
        } else if (choice == 8) {
            vector<AthleteRecord> records = db::readAll(filename);
            db::sortByPoints(records);
            cout << (db::overwriteAll(filename, records) ? "Відсортовано за очками та збережено.\n"
                                                        : "Помилка збереження.\n");
        } else if (choice == 9) {
            vector<AthleteRecord> records = db::readAll(filename);
            db::sortByResult(records);
            cout << (db::overwriteAll(filename, records) ? "Відсортовано за результатом та збережено.\n"
                                                        : "Помилка збереження.\n");
        } else if (choice == 10) {
            int id;
            cout << "Введіть ID запису для зміни: ";
            cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            vector<AthleteRecord> records = db::readAll(filename);
            AthleteRecord updated = db::makeRecordFromInput();
            if (db::updateById(records, id, updated) && db::overwriteAll(filename, records)) {
                cout << "Запис змінено.\n";
            } else {
                cout << "Запис не знайдено або помилка збереження.\n";
            }
        } else if (choice == 11) {
            int id;
            cout << "Введіть ID запису для видалення: ";
            cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            vector<AthleteRecord> records = db::readAll(filename);
            if (db::removeById(records, id) && db::overwriteAll(filename, records)) {
                cout << "Запис видалено.\n";
            } else {
                cout << "Запис не знайдено або помилка збереження.\n";
            }
        } else if (choice == 12) {
            db::printStatistics(db::readAll(filename));
        } else if (choice == 13) {
            if (db::createDatabase(filename)) {
                db::fillDemoData(filename);
                cout << "БД заповнено 20 демонстраційними записами.\n";
            } else {
                cout << "Не вдалося підготувати файл БД.\n";
            }
        } else if (choice == 14) {
            filename = requestFilename();
            cout << "Поточний файл БД: " << filename << '\n';
        } else if (choice == 0) {
            cout << "Завершення роботи.\n";
        } else {
            cout << "Невідомий пункт меню.\n";
        }
    }

    return 0;
}
