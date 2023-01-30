#pragma once

template<typename StatusEnum>
constexpr bool isSuccessful(StatusEnum status) {
  return status == StatusEnum::SUCCESS;
}

template<typename StatusEnum>
constexpr bool hasError(StatusEnum status) {
  return status != StatusEnum::SUCCESS;
}




template<typename StatusEnum, typename ValueType>
class StatusResult
{
public:

  constexpr StatusResult(StatusEnum status, ValueType value = {})
    : m_status(status), m_value(value)
  {
  }

  constexpr static StatusResult success(ValueType value) {
    return StatusResult(StatusEnum::SUCCESS, value);
  }

  constexpr static StatusResult error(StatusEnum status = StatusEnum::Error) {
    return StatusResult(status);
  }

  constexpr bool isSuccess() const {
    return m_status == StatusEnum::SUCCESS;
  }

  constexpr bool hasError() const {
    return m_status != StatusEnum::SUCCESS;
  }

  constexpr ValueType getValue() const {
    return m_value;
  }

  constexpr StatusEnum getStatus() const {
    return m_status;
  }

  constexpr bool operator==(StatusEnum other) const {
    return m_status == other;
  }

  constexpr bool operator!=(StatusEnum other) const {
    return !operator==(other);
  }

private:
  StatusEnum m_status;
  ValueType m_value;
};



template<typename StatusEnum, typename ValueType>
constexpr bool isSuccessful(const StatusResult<StatusEnum, ValueType>& result) {
  return result.isSuccessful();
}

template<typename StatusEnum, typename ValueType>
constexpr bool hasError(const StatusResult<StatusEnum, ValueType>& result) {
  return result.hasError();
}

