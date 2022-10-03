#ifndef CONVERTER_H
#define CONVERTER_H

#include "base_types.h"

class Converter
{
public:
    static Signal convert(const Frame& frame, ReadStatus status);
};

#endif // CONVERTER_H
