#pragma once

#include "GpDbClient_global.hpp"

namespace GPlatform {

GP_ENUM(GPDBCLIENT_API, GpDbExceptionCode,
    CONNECTION_ERROR,
    CONNECTION_LIMIT_EXCEEDED,
    QUERY_ERROR,
    QUERY_RESULT_COUNT_LOW
);

}//namespace GPlatform
