#pragma once

#include "GpDbClient_global.hpp"

namespace GPlatform {

GP_ENUM(GPDBCLIENT_API, GpDbTransactionIsolation,
    SERIALIZABLE,
    REPEATABLE_READ,
    READ_COMMITTED,
    READ_UNCOMMITTED
);

}//namespace GPlatform
