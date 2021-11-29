#ifndef DATATYPE_H
#define DATATYPE_H

#include <QString>
#include "DataTypeAttributes.h"
#include "Magic.h"

struct DataType
{
    virtual ~DataType() = 0;
    virtual DataTypeAttributes getAttributes() = 0;
};

#endif
