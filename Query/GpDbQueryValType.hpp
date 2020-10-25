#pragma once

#include "../GpDbClient_global.hpp"

namespace GPlatform {

GP_ENUM(GPDBCLIENT_API, GpDbQueryValType,
    INT_64,
    STRING_VALUE,
    STRING_NAME,
    STRING_JSON,
    UUID,
    BLOB,
    BOOLEAN,
    NULL_VAL
);

}//namespace GPlatform
