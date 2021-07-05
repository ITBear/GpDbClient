#include "GpDbDriver.hpp"

#include <iostream>

namespace GPlatform {

static int _GpDbDriver_counter = 0;

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
    _GpDbDriver_counter++;
    std::cout << "[GpDbDriver::GpDbDriver]: counter = " << _GpDbDriver_counter << std::endl;
}

GpDbDriver::~GpDbDriver (void) noexcept
{
    _GpDbDriver_counter--;
    std::cout << "[GpDbDriver::~GpDbDriver]: counter = " << _GpDbDriver_counter << std::endl;
}

}//GPlatform
