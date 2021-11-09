#include "GpDbDriver.hpp"

namespace GPlatform {

GpDbDriver::GpDbDriver
(
    std::string_view                aName,
    const GpDbConnectionMode::EnumT aMode,
    GpIOEventPoller::WP             aEventPoller
):
iName(aName),
iMode(aMode),
iEventPoller(std::move(aEventPoller))
{
}

GpDbDriver::~GpDbDriver (void) noexcept
{
}

}//GPlatform
