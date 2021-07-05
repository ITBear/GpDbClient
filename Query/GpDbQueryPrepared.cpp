#include "GpDbQueryPrepared.hpp"

namespace GPlatform {

GpDbQueryPrepared::GpDbQueryPrepared (GpDbQuery::CSP aQuery) noexcept:
iQuery(std::move(aQuery))
{
}

GpDbQueryPrepared::~GpDbQueryPrepared (void) noexcept
{
}

}//namespace GPlatform
