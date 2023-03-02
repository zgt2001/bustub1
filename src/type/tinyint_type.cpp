//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// tinyint_type.cpp
//
// Identification: src/type/tinyint_type.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#include "common/exception.h"
#include "type/tinyint_type.h"

namespace bustub {
#define TINYINT_COMPARE_FUNC(OP)                                          \
  switch (right.GetTypeId()) {                                            \
    case TypeId::TINYINT:                                                 \
      return GetCmpBool(left.value_.tinyint_ OP right.GetAs<int8_t>());   \
    case TypeId::SMALLINT:                                                \
      return GetCmpBool(left.value_.tinyint_ OP right.GetAs<int16_t>());  \
    case TypeId::INTEGER:                                                 \
      return GetCmpBool(left.value_.tinyint_ OP right.GetAs<int32_t>());  \
    case TypeId::BIGINT:                                                  \
      return GetCmpBool(left.value_.tinyint_ OP right.GetAs<int64_t>());  \
    case TypeId::DECIMAL:                                                 \
      return GetCmpBool(left.value_.tinyint_ OP right.GetAs<double>());   \
    case TypeId::VARCHAR: {                                               \
      auto r_value = right.CastAs(TypeId::TINYINT);                       \
      return GetCmpBool(left.value_.tinyint_ OP r_value.GetAs<int8_t>()); \
    }                                                                     \
    default:                                                              \
      break;                                                              \
  }  // SWITCH

#define TINYINT_MODIFY_FUNC(METHOD, OP)                                             \
  switch (right.GetTypeId()) {                                                      \
    case TypeId::TINYINT:                                                           \
      /* NOLINTNEXTLINE */                                                          \
      return METHOD<int8_t, int8_t>(left, right);                                   \
    case TypeId::SMALLINT:                                                          \
      /* NOLINTNEXTLINE */                                                          \
      return METHOD<int8_t, int16_t>(left, right);                                  \
    case TypeId::INTEGER:                                                           \
      /* NOLINTNEXTLINE */                                                          \
      return METHOD<int8_t, int32_t>(left, right);                                  \
    case TypeId::BIGINT:                                                            \
      /* NOLINTNEXTLINE */                                                          \
      return METHOD<int8_t, int64_t>(left, right);                                  \
    case TypeId::DECIMAL:                                                           \
      return Value(TypeId::DECIMAL, left.value_.tinyint_ OP right.GetAs<double>()); \
    case TypeId::VARCHAR: {                                                         \
      auto r_value = right.CastAs(TypeId::TINYINT);                                 \
      /* NOLINTNEXTLINE  */                                                         \
      return METHOD<int8_t, int8_t>(left, r_value);                                 \
    }                                                                               \
    default:                                                                        \
      break;                                                                        \
  }  // SWITCH

TinyintType::TinyintType() : IntegerParentType(TINYINT) {}

bool TinyintType::IsZero(const Value &val) const { return (val.value_.tinyint_ == 0); }

Value TinyintType::Add(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));
  if (left.IsNull() || right.IsNull()) {
    return left.OperateNull(right);
  }

  TINYINT_MODIFY_FUNC(AddValue, +);  // NOLINT

  throw Exception("type error");
}

Value TinyintType::Subtract(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));
  if (left.IsNull() || right.IsNull()) {
    return left.OperateNull(right);
  }

  TINYINT_MODIFY_FUNC(SubtractValue, -);  // NOLINT

  throw Exception("type error");
}

Value TinyintType::Multiply(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));
  if (left.IsNull() || right.IsNull()) {
    return left.OperateNull(right);
  }

  TINYINT_MODIFY_FUNC(MultiplyValue, *);  // NOLINT

  throw Exception("type error");
}

Value TinyintType::Divide(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));
  if (left.IsNull() || right.IsNull()) {
    return left.OperateNull(right);
  }

  if (right.IsZero()) {
    throw Exception(ExceptionType::DIVIDE_BY_ZERO, "Division by zero on right-hand side");
  }

  TINYINT_MODIFY_FUNC(DivideValue, /);  // NOLINT

  throw Exception("type error");
}

Value TinyintType::Modulo(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));
  if (left.IsNull() || right.IsNull()) {
    return left.OperateNull(right);
  }

  if (right.IsZero()) {
    throw Exception(ExceptionType::DIVIDE_BY_ZERO, "Division by zero on right-hand side");
  }

  switch (right.GetTypeId()) {
    case TypeId::TINYINT:
      return ModuloValue<int8_t, int8_t>(left, right);
    case TypeId::SMALLINT:
      return ModuloValue<int8_t, int16_t>(left, right);
    case TypeId::INTEGER:
      return ModuloValue<int8_t, int32_t>(left, right);
    case TypeId::BIGINT:
      return ModuloValue<int8_t, int64_t>(left, right);
    case TypeId::DECIMAL:
      return Value(TypeId::DECIMAL, ValMod(left.value_.tinyint_, right.GetAs<double>()));
    case TypeId::VARCHAR: {
      auto r_value = right.CastAs(TypeId::TINYINT);
      return ModuloValue<int8_t, int8_t>(left, r_value);
    }
    default:
      break;
  }

  throw Exception("type error");
}

Value TinyintType::Sqrt(const Value &val) const {
  assert(val.CheckInteger());
  if (val.IsNull()) {
    return Value(TypeId::DECIMAL, BUSTUB_DECIMAL_NULL);
  }

  if (val.value_.tinyint_ < 0) {
    throw Exception(ExceptionType::DECIMAL, "Cannot take square root of a negative number.");
  }
  return Value(TypeId::DECIMAL, std::sqrt(val.value_.tinyint_));

  throw Exception("type error");
}

Value TinyintType::OperateNull(const Value &left[[maybe_unused]], const Value &right) const {
  switch (right.GetTypeId()) {
    case TypeId::TINYINT:
      return Value(TypeId::TINYINT, static_cast<int8_t>(BUSTUB_INT8_NULL));
    case TypeId::SMALLINT:
      return Value(TypeId::SMALLINT, static_cast<int16_t>(BUSTUB_INT16_NULL));
    case TypeId::INTEGER:
      return Value(TypeId::INTEGER, static_cast<int32_t>(BUSTUB_INT32_NULL));
    case TypeId::BIGINT:
      return Value(TypeId::BIGINT, static_cast<int64_t>(BUSTUB_INT64_NULL));
    case TypeId::DECIMAL:
      return Value(TypeId::DECIMAL, static_cast<double>(BUSTUB_DECIMAL_NULL));
    default:
      break;
  }
  throw Exception("type error");
}

CmpBool TinyintType::CompareEquals(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));

  if (left.IsNull() || right.IsNull()) {
    return CmpBool::CmpNull;
  }

  TINYINT_COMPARE_FUNC(==);  // NOLINT

  throw Exception("type error");
}

CmpBool TinyintType::CompareNotEquals(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));
  if (left.IsNull() || right.IsNull()) {
    return CmpBool::CmpNull;
  }

  TINYINT_COMPARE_FUNC(!=);  // NOLINT

  throw Exception("type error");
}

CmpBool TinyintType::CompareLessThan(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));
  if (left.IsNull() || right.IsNull()) {
    return CmpBool::CmpNull;
  }

  TINYINT_COMPARE_FUNC(<);  // NOLINT

  throw Exception("type error");
}

CmpBool TinyintType::CompareLessThanEquals(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));
  if (left.IsNull() || right.IsNull()) {
    return CmpBool::CmpNull;
  }

  TINYINT_COMPARE_FUNC(<=);  // NOLINT

  throw Exception("type error");
}

CmpBool TinyintType::CompareGreaterThan(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));
  if (left.IsNull() || right.IsNull()) {
    return CmpBool::CmpNull;
  }

  TINYINT_COMPARE_FUNC(>);  // NOLINT

  throw Exception("type error");
}

CmpBool TinyintType::CompareGreaterThanEquals(const Value &left, const Value &right) const {
  assert(left.CheckInteger());
  assert(left.CheckComparable(right));
  if (left.IsNull() || right.IsNull()) {
    return CmpBool::CmpNull;
  }

  TINYINT_COMPARE_FUNC(>=);  // NOLINT

  throw Exception("type error");
}

std::string TinyintType::ToString(const Value &val) const {
  assert(val.CheckInteger());
  if (val.IsNull()) {
    return "tinyint_null";
  }
  return std::to_string(val.value_.tinyint_);
}

void TinyintType::SerializeTo(const Value &val, char *storage) const {
  *reinterpret_cast<int8_t *>(storage) = val.value_.tinyint_;
}

// Deserialize a value of the given type from the given storage space.
Value TinyintType::DeserializeFrom(const char *storage) const {
  int8_t val = *reinterpret_cast<const int8_t *>(storage);
  return Value(type_id_, val);
}

Value TinyintType::Copy(const Value &val) const {
  assert(val.CheckInteger());
  return Value(TypeId::TINYINT, val.value_.tinyint_);
}

Value TinyintType::CastAs(const Value &val, const TypeId type_id) const {
  switch (type_id) {
    case TypeId::TINYINT: {
      if (val.IsNull()) {
        return Value(type_id, BUSTUB_INT8_NULL);
      }
      return Copy(val);
    }
    case TypeId::SMALLINT: {
      if (val.IsNull()) {
        return Value(type_id, BUSTUB_INT16_NULL);
      }
      return Value(type_id, static_cast<int16_t>(val.GetAs<int8_t>()));
    }
    case TypeId::INTEGER: {
      if (val.IsNull()) {
        return Value(type_id, BUSTUB_INT32_NULL);
      }
      return Value(type_id, static_cast<int32_t>(val.GetAs<int8_t>()));
    }
    case TypeId::BIGINT: {
      if (val.IsNull()) {
        return Value(type_id, BUSTUB_INT64_NULL);
      }
      return Value(type_id, static_cast<int64_t>(val.GetAs<int8_t>()));
    }
    case TypeId::DECIMAL: {
      if (val.IsNull()) {
        return Value(type_id, BUSTUB_DECIMAL_NULL);
      }
      return Value(type_id, static_cast<double>(val.GetAs<int8_t>()));
    }
    case TypeId::VARCHAR: {
      if (val.IsNull()) {
        return Value(TypeId::VARCHAR, nullptr, 0, false);
      }
      return Value(TypeId::VARCHAR, val.ToString());
    }
    default:
      break;
  }
  throw Exception("tinyint is not coercable to " + Type::TypeIdToString(type_id));
}
}  // namespace bustub
