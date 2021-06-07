#pragma once

#include "GpDbQueryValStrValue.hpp"
#include "GpDbQueryValStrName.hpp"
#include "GpDbQueryValStrJson.hpp"

namespace GPlatform {

using GpDbQueryValue = std::variant
<
    SInt16,                     //GpDbQueryValType::S_INT_16
    SInt32,                     //GpDbQueryValType::S_INT_32
    SInt64,                     //GpDbQueryValType::S_INT_64
    double,                     //GpDbQueryValType::DOUBLE
    float,                      //GpDbQueryValType::FLOAT
    GpDbQueryValStrValue,       //GpDbQueryValType::STRING_VALUE
    GpVector<std::string>,      //GpDbQueryValType::STRING_VALUE_ARRAY
    GpDbQueryValStrName,        //GpDbQueryValType::STRING_NAME
    GpDbQueryValStrJson,        //GpDbQueryValType::STRING_JSON
    GpUUID,                     //GpDbQueryValType::UUID
    GpBytesArray,               //GpDbQueryValType::BLOB
    bool,                       //GpDbQueryValType::BOOLEAN
    std::nullopt_t              //GpDbQueryValType::NULL_VAL
>;

}//namespace GPlatform
