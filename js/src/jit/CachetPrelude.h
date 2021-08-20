/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_CachetPrelude_h
#define jit_CachetPrelude_h

#include <utility>
#include <variant>

namespace cachet {

namespace prelude {

struct Cachet_Bail {};

template <typename T>
struct PrimitiveType {
  using Val = T;
  using Local = T;
  using Ref = const T&;
  using OutRef = T&;

  static inline bool CompareEq(Val lhs, Val rhs) { return lhs == rhs; }

  static inline bool CompareNeq(Val lhs, Val rhs) { return lhs != rhs; }

  static inline Val ToVal(Ref ref) { return ref; }

  template <typename Cachet_Context>
  static inline Local EmptyLocal(Cachet_Context cx) {
    return T();
  }

  template <typename Cachet_Context>
  static inline Local ToLocal(Cachet_Context cx, Val&& val) {
    return std::move(val);
  }

  template <typename Cachet_Context>
  static inline Local ToLocal(Cachet_Context cx, Ref ref) {
    return ref;
  }

  static inline Ref ToRef(const Val& val) { return val; }

  static inline OutRef ToOutRef(Local& local) { return local; }

  static inline Val SetOutRef(OutRef out, Val&& in) {
    out = std::move(in);
    return out;
  }

  static inline Val SetOutRef(OutRef out, Ref in) {
    out = in;
    return out;
  }
};

template <typename T>
struct NumericType : public PrimitiveType<T> {
  using Val = typename PrimitiveType<T>::Val;
  using Local = typename PrimitiveType<T>::Local;
  using Ref = typename PrimitiveType<T>::Ref;
  using OutRef = typename PrimitiveType<T>::OutRef;

  static inline bool CompareLte(Val lhs, Val rhs) { return lhs <= rhs; }

  static inline bool CompareGte(Val lhs, Val rhs) { return lhs >= rhs; }

  static inline bool CompareLt(Val lhs, Val rhs) { return lhs < rhs; }

  static inline bool CompareGt(Val lhs, Val rhs) { return lhs > rhs; }
};

using Type_Unit = PrimitiveType<std::monostate>;
using Type_Bool = NumericType<bool>;
using Type_Int32 = NumericType<int32_t>;
using Type_Double = NumericType<double>;

inline constexpr Type_Unit::Val Const_unit = std::monostate();

inline constexpr Type_Bool::Val Const_true = true;
inline constexpr Type_Bool::Val Const_false = false;

};  // namespace prelude

};  // namespace cachet

#endif /* jit_CachetPrelude_h */
