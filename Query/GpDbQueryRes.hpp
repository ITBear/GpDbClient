#pragma once

#include "GpDbQueryResState.hpp"

namespace GPlatform {

class GPDBCLIENT_API GpDbQueryRes
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpDbQueryRes)
    CLASS_DECLARE_DEFAULTS(GpDbQueryRes)

    using StateTE = GpDbQueryResState::EnumT;

public:
                                    GpDbQueryRes    (void) noexcept;
    virtual                         ~GpDbQueryRes   (void) noexcept;

    virtual void                    Clear           (void) = 0;

    [[nodiscard]]
    virtual StateTE                 State           (void) const = 0;

    [[nodiscard]]
    virtual count_t                 RowsCount       (void) const = 0;

    [[nodiscard]]
    virtual count_t                 ColumnsCount    (void) const = 0;

    [[nodiscard]]
    virtual SInt64                  GetInt64        (const count_t          aRowId,
                                                     const count_t          aColId,
                                                     std::optional<SInt64>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::string_view        GetStr          (const count_t                      aRowId,
                                                     const count_t                      aColId,
                                                     std::optional<std::string_view>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual GpRawPtrCharRW          GetStrRW        (const count_t                  aRowId,
                                                     const count_t                  aColId,
                                                     std::optional<GpRawPtrCharRW>  aOnNullValue) = 0;

    [[nodiscard]]
    virtual const GpVector<std::string>&
                                    GetStrArray     (const count_t                      aRowId,
                                                     const count_t                      aColId,
                                                     std::optional<std::string_view>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual std::string_view        GetJsonStr      (const count_t                      aRowId,
                                                     const count_t                      aColId,
                                                     std::optional<std::string_view>    aOnNullValue) const = 0;

    [[nodiscard]]
    virtual GpRawPtrCharRW          GetJsonStrRW    (const count_t                  aRowId,
                                                     const count_t                  aColId,
                                                     std::optional<GpRawPtrCharRW>  aOnNullValue) = 0;

    [[nodiscard]]
    virtual GpUUID                  GetUUID         (const count_t          aRowId,
                                                     const count_t          aColId,
                                                     std::optional<GpUUID>  aOnNullValue) const = 0;

    [[nodiscard]]
    virtual GpRawPtrByteR           GetBLOB         (const count_t                  aRowId,
                                                     const count_t                  aColId,
                                                     std::optional<GpRawPtrByteR>   aOnNullValue) const = 0;

    [[nodiscard]]
    virtual bool                    GetBoolean      (const count_t          aRowId,
                                                     const count_t          aColId,
                                                     std::optional<bool>    aOnNullValue) const = 0;

    template<typename T>
    [[nodiscard]] typename T::EnumT GetEnum         (const count_t                      aRowId,
                                                     const count_t                      aColId,
                                                     std::optional<typename T::EnumT>   aOnNullValue) const;
};

template<typename T>
[[nodiscard]] typename T::EnumT GpDbQueryRes::GetEnum (const count_t                    aRowId,
                                                       const count_t                    aColId,
                                                       std::optional<typename T::EnumT> aOnNullValue) const
{
    std::string_view strVal = GetStr(aRowId, aColId, ""_sv);

    if (strVal.length() == 0)
    {
        THROW_GPE_COND_CHECK_M(aOnNullValue.has_value(), "Value on ["_sv + aRowId + ", "_sv + aColId + "] is empty"_sv);
        return aOnNullValue.value();
    }

    return T::SFromString(strVal);
}

}//GPlatform
