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

[[noreturn]] inline void    THROW_DBE
(
    const GpDbExceptionCode::EnumT  aCode,
    std::string_view                aMsg,
    const SourceLocationT&          aSourceLocation = SourceLocationT::current()
)
{
    throw GpDbException(aCode, aMsg, aSourceLocation);
}

inline void THROW_DBE_COND
(
    const bool                      aCondition,
    const GpDbExceptionCode::EnumT  aCode,
    std::string_view                aMsg,
    const SourceLocationT&          aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition)
    {
        throw GpDbException(aCode, aMsg, aSourceLocation);
    }
}

inline void THROW_DBE_COND
(
    const bool                      aCondition,
    const GpDbExceptionCode::EnumT  aCode,
    ThrowMsgGenT                    aMsgGenFn,
    const SourceLocationT&          aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition)
    {
        throw GpDbException(aCode, aMsgGenFn(), aSourceLocation);
    }
}

}//namespace GPlatform


