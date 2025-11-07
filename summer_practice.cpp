/************************************************************************************************
*                                 ОЗНАКОМИТЕЛЬНАЯ ПРАКТИКА                                      *
*************************************************************************************************
* Project type  : Win64 Console Application                                                     *
* Project name  : Practice                                                                      *
* File name     : practice.cpp                                                                  *
* Language      : CPP                                                                           *
* Programmers   : Тимошин Антон Алексеевич, Воробьев Андрей Николаевич                          *
* Modified By   :                                                                               *
* Created       : 10.06.2024                                                                    *
* Last Revision : 14.06.2024                                                                    *
* Comment       :                                                                               *
************************************************************************************************/

#include <iostream>
#include "fstream"


using namespace std;

/**************************************************************
*                      ВХОДНЫЕ ДАННЫЕ                         *
**************************************************************/

//const string path = "non_existent_file.txt";       // несуществующий файл
//const string path = "empty_file.txt";              // пустой файл
//const string path = "info.txt";                    // файл с корректными исходными данными
//const string path = "incorrect.txt";               // файл с некорректными исходными данными

/**************************************************************
*                         КОНСТАНТЫ                           *
**************************************************************/

const int max_len = 100;        // максимальное количество элементов в массиве

/**************************************************************
*                         СТРУКТУРЫ                           *
**************************************************************/

struct Aircraft     // Структура для хранения данных о приземлившихся самолетах
{
    string flight_number;                  // номер рейса
    string tail_number;                    // бортовой номер
    string cargo_weight;                   // вес груза
    string box_quantity;                   // количество контейнеров
    int id;                                // индекс самолета
};

struct indexSort            // Структура для индексной сортировки
{
    int index;              // индекс сортировки (позиция в исходной структуре)
    int flight_number;      // номер рейса (поле, которое нужно отсортировать)
};

/**************************************************************
*                     ПРОТОТИПЫ ФУНКЦИЙ                       *
**************************************************************/
int checkTailNumber(const string &tailNumber, int massiveLen);

int checkInteger(string &line, int massiveLen);

int checkDouble(const string &line, int massiveLen);

void readDataFromFile(Aircraft &plane,
                      Aircraft *planes,
                      int &massiveLen,
                      int &errorCode); // функция чтения данных из файла

void echoPrint(Aircraft *planes,
               int massiveLen); // функция эхопечати исходной структуры данных

int boxCount(Aircraft *planes, int massiveLen); // функция, определяющая суммарное количество контейнеров

void printUpperPart();

void printMiddlePart();

void printLowerPart();

void sortByFlightNumber(Aircraft *planes, int massiveLen, indexSort massiveSort[max_len]);

void printResult(Aircraft *planes, indexSort *massiveSort, int index);


/**************************************************************
*                     ОСНОВНАЯ ПРОГРАММА                      *
**************************************************************/

int main() {
    Aircraft plane;                     // переменная для хранения информации об одном самолёте
    Aircraft planes[max_len];           // массив для хранения информации о всех самолётах
    indexSort massiveSort[max_len];     // массив для индексной сортировки

    int massiveLen = 0; // так как в массивах отсчет элементов начинается с 0
    int errorCode = 0; // переменная-флаг ошибок

    readDataFromFile(plane, planes, massiveLen, errorCode);

    if (errorCode != 0) {
        return 0;
    }

    echoPrint(planes, massiveLen);
    sortByFlightNumber(planes, massiveLen, massiveSort);
    int containers = boxCount(planes, massiveLen);
    cout << "Суммарное количество контейнеров: " << containers;

    return 0;
}

/**************************************************************
*                    ОПРЕДЕЛЕНИЕ ФУНКЦИЙ                      *
**************************************************************/

int checkTailNumber(const string &tailNumber, int massiveLen) {
    if (tailNumber.size() != 6) {
        cout << "Ошибка в " << massiveLen << " строке: в бортовом номере не совпадает количество символов." << endl;
        return 1;
    }
    if ((tailNumber[0] >= 'A' && tailNumber[0] <= 'Z') == 0) {
        cout << tailNumber[1];
        cout << "Ошибка в " << massiveLen << " строке: в бортовом номере первый символ - не буква латинского алфавита."
             << endl;
        return 1;
    }
    if (tailNumber[1] != '-') {
        cout << "Ошибка в " << massiveLen << " строке: в бортовом номере пропущен дефис." << endl;
        return 1;
    }
    for (int i = 2; i < 5; i++) { // Проверяем остальные элементы на цифры
        if (!isdigit(tailNumber[i])) {
            cout << "Ошибка в " << massiveLen << " строке: в бортовом номере не целое число после дефиса." << endl;
            return 1;
        }
    }
    return 0;
}

int checkInteger(string &line, int massiveLen) {
    for (char c: line) {
        if (!isdigit(c)) {
            cout << "Ошибка в " << massiveLen << " строке: " << line << " не является целым числом." << endl;
            return 1;
        }
    }
    return 0;
}

int checkDouble(const string &line, int massiveLen) {
    for (char c: line) {
        if (!isdigit(c) && c != '.') {
            cout << "Ошибка в " << massiveLen << " строке: " << line << " не является вещественным числом." << endl;
            return 1;
        }
    }
    return 0;
}

void readDataFromFile(Aircraft &plane,
                      Aircraft *planes,
                      int &massiveLen,
                      int &errorCode) { // функция чтения из файла

    ifstream file(path);

    if (!file) {
        cout << "Файл " << path << " не существует" << endl;
        errorCode++;
    } else if (file.fail()) {
        cout << "Не удалось открыть файл " << path << endl;
        errorCode++;
    } else if (file.peek() == -1) {
        cout << "Файл " << path << " пустой" << endl;
        errorCode++;
    }

    while (!file.eof()) {
        massiveLen++;
        plane.id = massiveLen;
        file >> plane.flight_number >> plane.tail_number >> plane.cargo_weight >> plane.box_quantity;

        errorCode =
                checkInteger(plane.flight_number, massiveLen) +
                checkTailNumber(plane.tail_number, massiveLen) +
                checkDouble(plane.cargo_weight, massiveLen) +
                checkInteger(plane.box_quantity, massiveLen);

        planes[massiveLen - 1] = plane; // так как в массивах отсчет элементов начинается с 0
    }
}

void echoPrint(Aircraft *planes,
               int massiveLen) { // функция эхопечати данных
    cout << "Исходная структура данных: " << endl;
    printUpperPart();
    for (int i = 0; i < massiveLen; i++) {
        printMiddlePart();
        cout << "║" << setw(12) << planes[i].flight_number << setw(11) << "║"
             << setw(13) << planes[i].tail_number << setw(10) << "║"
             << setw(12) << planes[i].cargo_weight << setw(11) << "║"
             << setw(11) << planes[i].box_quantity << setw(12) << "║"
             << setw(11) << planes[i].id << setw(12) << "║" << endl;
    }
    printLowerPart();
}

int boxCount(Aircraft *planes, int massiveLen) {
    int boxSum = 0;         // счетчик контейнеров
    for (int i = 0; i < massiveLen; i++) {
        boxSum += stoi(planes[i].box_quantity);
    }
    return boxSum;
}

void printUpperPart() {
    cout << "╔════════════════════╦════════════════════╦════════════════════╦════════════════════╦════════════════════╗"
         << endl;
    cout << "║    НОМЕР РЕЙСА     ║   БОРТОВОЙ НОМЕР   ║      ВЕС ГРУЗА     ║ КОЛ-ВО КОНТЕЙНЕРОВ ║       ИНДЕКС       ║"
         << endl;
}

void printMiddlePart() {
    cout << "╠════════════════════╬════════════════════╬════════════════════╬════════════════════╣════════════════════╣"
         << endl;
}

void printLowerPart() {
    cout << "╚════════════════════╩════════════════════╩════════════════════╩════════════════════╩════════════════════╝"
         << endl;
}

void sortByFlightNumber(Aircraft *planes, int massiveLen, indexSort massiveSort[max_len]) {
    indexSort Temporary;
    int index = 0;

    cout << "Сортировка по номеру рейса: " << endl;

    for (int i = 0; i < massiveLen; i++) {
        massiveSort[index].index = i + 1;
        massiveSort[index].flight_number = stoi(planes[i].flight_number);
        index++;
    }

    int min;                // минимальное значение
    int minIndex;           // минимальный индекс
    int j = 0;              // индекс начала поисков

    while (j < index) {
        min = massiveSort[j].flight_number;
        minIndex = j;

        for (int i = j; i < index; i++) {
            if (massiveSort[i].flight_number < min) {
                min = massiveSort[i].flight_number;
                minIndex = i;
            }
        }
        // замена местами наименьшего и стоящего в начале элемента
        Temporary = massiveSort[j];
        massiveSort[j] = massiveSort[minIndex];
        massiveSort[minIndex] = Temporary;

        j++;
    }
    printResult(planes, massiveSort, index);
}

void printResult(Aircraft *planes, indexSort *massiveSort, int index) {
    printUpperPart();
    for (int i = 0; i < index; i++) {
        printMiddlePart();
        // выводим элементы исходного массива, опираясь на индексы, полученные в результате индексной сортировки
        cout << "║" << setw(12) << planes[massiveSort[i].index - 1].flight_number << setw(11) << "║"
             << setw(13) << planes[massiveSort[i].index - 1].tail_number << setw(10) << "║"
             << setw(12) << planes[massiveSort[i].index - 1].cargo_weight << setw(11) << "║"
             << setw(11) << planes[massiveSort[i].index - 1].box_quantity << setw(12) << "║"
             << setw(11) << planes[massiveSort[i].index - 1].id << setw(12) << "║" << endl;
    }
    printLowerPart();
}
