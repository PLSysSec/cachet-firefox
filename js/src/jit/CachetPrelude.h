/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_CachetPrelude_h
#define jit_CachetPrelude_h

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <utility>
#include <variant>

namespace cachet {

namespace prelude {

template <typename T>
struct PrimitiveType {
  using Val = T;
  using Local = Val;
  using Ref = Val;
  using MutRef = Val&;

  template <typename U>
  static U&& ToVal(U&& x) {
    return std::forward<U>(x);
  }

  template <typename C>
  static Local EmptyLocal(C cx) {
    return T();
  }

  template <typename C, typename U>
  static U&& ToLocal(C cx, U&& x) {
    return std::forward<U>(x);
  }

  template <typename U>
  static U&& ToRef(U&& x) {
    return std::forward<U>(x);
  }

  template <typename U>
  static U&& Fields(U&& x) {
    return std::forward<U>(x);
  }

  static MutRef ToMutRef(Local& local) {
    return local;
  }

  template <typename U>
  static void SetMutRef(MutRef lhs, U&& rhs) {
    lhs = std::forward<U>(rhs);
  }

  static bool Eq(Ref lhs, Ref rhs) {
    return lhs == rhs;
  }

  static bool Neq(Ref lhs, Ref rhs) {
    return lhs != rhs;
  }
};

template <typename T>
struct NumericType : public PrimitiveType<T> {
  using Val = typename PrimitiveType<T>::Val;
  using Local = typename PrimitiveType<T>::Local;
  using Ref = typename PrimitiveType<T>::Ref;
  using MutRef = typename PrimitiveType<T>::MutRef;

  static bool Lte(Ref lhs, Ref rhs) {
    return lhs <= rhs;
  }

  static bool Gte(Ref lhs, Ref rhs) {
    return lhs >= rhs;
  }

  static bool Lt(Ref lhs, Ref rhs) {
    return lhs < rhs;
  }

  static bool Gt(Ref lhs, Ref rhs) {
    return lhs > rhs;
  }

  static Val Negate(Ref val) {
    return -val;
  }

  static Val Add(Ref lhs, Ref rhs) {
    return lhs + rhs;
  }

  static Val Sub(Ref lhs, Ref rhs) {
    return lhs - rhs;
  }

  static Val Mul(Ref lhs, Ref rhs) {
    return lhs * rhs;
  }

  static Val Div(Ref lhs, Ref rhs) {
    return lhs / rhs;
  }

  static Val Mod(Ref lhs, Ref rhs) {
    return lhs % rhs;
  }

};

template <typename T>
struct IntegralType : public NumericType<T> {
  using Val = typename NumericType<T>::Val;
  using Local = typename NumericType<T>::Local;
  using Ref = typename NumericType<T>::Ref;
  using MutRef = typename NumericType<T>::MutRef;

  static Val Shl(Ref lhs, Ref rhs) {
    return lhs << rhs;
  }

  static Val Shr(Ref lhs, Ref rhs) {
    return lhs >> rhs;
  }

  static Val BitNot(Ref val) {
    return ~val;
  }

  static Val BitAnd(Ref lhs, Ref rhs) {
    return lhs & rhs;
  }

  static Val BitOr(Ref lhs, Ref rhs) {
    return lhs | rhs;
  }

  static Val Xor(Ref lhs, Ref rhs) {
    return lhs ^ rhs;
  }
};


using Type_Unit = PrimitiveType<std::monostate>;
using Type_Bool = NumericType<bool>;
using Type_Double = NumericType<double>;
using Type_Int16 = IntegralType<int16_t>;
using Type_Int32 = IntegralType<int32_t>;
using Type_Int64 = IntegralType<int64_t>;
using Type_UInt16 = IntegralType<uint16_t>;
using Type_UInt32 = IntegralType<uint32_t>;
using Type_UInt64 = IntegralType<uint64_t>;

namespace Impl_Double {
  template <typename T>
  inline Type_Double::Val Fn_is_nan(T ctx, Type_Double::Ref v) {
    return std::isnan(v);
  }

  template <typename T>
  inline Type_UInt64::Val Fn_bits(T ctx, Type_Double::Ref v) {
    Type_UInt64::Val out;
    memcpy(&out, &v, sizeof(Type_Double::Ref));
    return out;
  }

  template <typename T>
  inline Type_Double::Val Fn_ceil(T ctx, Type_Double::Ref v) {
    return std::ceil(v);
  }

  template <typename T>
  inline Type_Double::Val Var_INFINITY(T ctx) {
    return std::numeric_limits<double>::infinity();
  }

  template <typename T>
  inline Type_Double::Val Var_NEG_INFINITY(T ctx) {
    return std::numeric_limits<double>::infinity();
  }
}

namespace Impl_Int32 {
  template <typename T>
  inline Type_Int32::Val Fn_abs(T ctx, Type_Int32::Ref v) {
    return std::abs(v);
  }
}

inline Type_Unit::Ref Var_unit() {
  return std::monostate();
}

template <typename T>
inline Type_Bool::Ref Var_true(T ctx) {
  return true;
}

template <typename T>
inline Type_Bool::Ref Var_false(T ctx) {
  return false;
}

};  // namespace prelude

};  // namespace cachet

#endif /* jit_CachetPrelude_h */
