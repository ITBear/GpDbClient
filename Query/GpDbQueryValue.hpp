#pragma once

#include "GpDbQueryValStrValue.hpp"
#include "GpDbQueryValStrName.hpp"
#include "GpDbQueryValStrJson.hpp"

namespace GPlatform {

using GpDbQueryValue = std::variant
<
    SInt64,                     //GpDbQueryValType::S_INT_64
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
