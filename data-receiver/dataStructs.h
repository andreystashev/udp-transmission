#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H
#include <QString>



#pragma pack(push,1)
struct DataStruct
{
    int hour, min, sec; // время
    double fuelWeight;  // вес топлива, кг
    union {
        unsigned int word; // слово данных
        struct {
            unsigned char addr : 8;     // адрес (в 16-тиричном представлении)
            unsigned char reserve1 : 6; // резерв
            bool dataOk : 1;            // признак корректности данных
            bool onLand : 1;            // признак нахождения на земле
            unsigned int speed : 10;    // скорость, км/ч (цена старшего разряда - 512)
            unsigned char reserve2 : 3; // резерв
            unsigned char matrix : 2;   // матрица состояния(00 - отказ, 01 – нет данных, 10 – тест, 11 - нормальная работа)
            bool ctrlBit : 1;           // бит контроля чётности
        } data;
    } dataUnion;
    bool isCrashed;     // признак разрушения
};
#pragma pack(pop)


QString nameMassive[] = {"Часы","Минуты","Секунды","Вес топлива, кг","слово данных","адрес (в 16-тиричном представлении)",
"какой-то резерв, не нужно для использования","признак корректности данных","признак нахождения на земле",
"скорость, км/ч (цена старшего разряда - 512)","какой-то резерв, не нужно для использования",
"матрица состояния(00 - отказ, 01 – нет данных, 10 – тест, 11 - нормальная работа)","бит контроля чётности","признак разрушения"};



#endif // DATASTRUCTS_H
