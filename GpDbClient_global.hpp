#pragma once

#include "../GpCore2/GpCore.hpp"
#include "../GpJson/GpJson.hpp"
#include "../GpNetwork/GpNetwork.hpp"

#if defined(GPDBCLIENT_LIBRARY)
    #define GPDBCLIENT_API GP_DECL_EXPORT
#else
    #define GPDBCLIENT_API GP_DECL_IMPORT
#endif
