#pragma once

#include "GpDbExceptionCode.hpp"

namespace GPlatform {

class GPDBCLIENT_API GpDbException final: public GpException
{
public:
    using CodeT     = GpDbExceptionCode;
    using CodeTE    = CodeT::EnumT;

private:
                            GpDbException       (void) noexcept = delete;
                            GpDbException       (const GpDbException&   aException) noexcept = delete;

    GpDbException&          operator=           (const GpDbException& aException) noexcept = delete;
    GpDbException&          operator=           (GpDbException&& aException) noexcept = delete;

public:
                            GpDbException       (GpDbException&& aException) noexcept;
                            GpDbException       (CodeTE                 aCode,
                                                 std::string_view       aMsg,
                                                 const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;
    virtual                 ~GpDbException      (void) noexcept override final;

    CodeTE                  Code                (void) const noexcept {return iCode;}

private:
    const CodeTE            iCode;
};

}//namespace GPlatform

#define THROW_DBE(CODE)                         throw ::GPlatform::GpDbException((CODE), ""_sv)
#define THROW_DBE_COND_CHECK(COND, CODE)        if (!(COND)) throw ::GPlatform::GpDbException((CODE), ("Condition not met: "#COND))
#define THROW_DBE_COND_CHECK_M(COND, CODE, MSG) if (!(COND)) throw ::GPlatform::GpDbException((CODE), (MSG))
