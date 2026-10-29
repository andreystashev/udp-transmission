#ifndef UDP_TRANSMISSION_DATA_H
#define UDP_TRANSMISSION_DATA_H

#include <cstddef>
#include <cstring>

// This demo sends the native packed representation. Both peers must use
// compatible integer sizes, byte order, floating-point and bit-field layouts.
#pragma pack(push, 1)
struct DataStruct
{
    int hour, min, sec;
    double fuelWeight;
    union {
        unsigned int word;
        struct {
            unsigned char addr : 8;
            unsigned char reserve1 : 6;
            bool dataOk : 1;
            bool onLand : 1;
            unsigned int speed : 10;
            unsigned char reserve2 : 3;
            unsigned char matrix : 2;
            bool ctrlBit : 1;
        } data;
    } dataUnion;
    bool isCrashed;
};
#pragma pack(pop)

inline bool unpackData(const char *bytes, std::size_t size, DataStruct &result)
{
    if (size != sizeof(DataStruct))
        return false;

    // Reject invalid bool representations before accessing the decoded object.
    const bool falseValue = false;
    const bool trueValue = true;
    const char *crashed = bytes + offsetof(DataStruct, isCrashed);
    if (std::memcmp(crashed, &falseValue, sizeof(bool)) != 0 &&
        std::memcmp(crashed, &trueValue, sizeof(bool)) != 0)
        return false;

    std::memcpy(&result, bytes, sizeof(result));
    return true;
}

#endif
