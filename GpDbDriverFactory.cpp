#include "GpDbDriverFactory.hpp"

namespace GPlatform {

GpDbDriverFactory::GpDbDriverFactory (std::string_view aName):
iName(aName)
{
}

GpDbDriverFactory::~GpDbDriverFactory (void) noexcept
{
}

}//namespace GPlatform
