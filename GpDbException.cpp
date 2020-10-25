#include "GpDbException.hpp"

namespace GPlatform {

GpDbException::GpDbException (GpDbException&& aException) noexcept:
GpException(std::move(aException)),
iCode(std::move(aException.iCode))
{
}

GpDbException::GpDbException (CodeTE                    aCode,
                              std::string_view          aMsg,
                              const SourceLocationT&    aSourceLocation) noexcept:
GpException(aMsg, aSourceLocation),
iCode(aCode)
{
}

GpDbException::~GpDbException (void) noexcept
{
}

}//namespace GPlatform
