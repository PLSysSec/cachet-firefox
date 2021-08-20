/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_CachetInterpreter_h
#define jit_CachetInterpreter_h

#include "mozilla/Result.h"
#include "mozilla/ResultVariant.h"

#include <utility>

#include "jit/CachetPrelude.h"
#include "js/Class.h"
#include "js/RootingAPI.h"
#include "js/Value.h"
#include "vm/JSObject.h"
#include "vm/NativeObject.h"
#include "vm/Shape.h"

namespace js {

namespace jit {

namespace cachet {

using namespace ::cachet::prelude;

using Cachet_Context = JSContext*;

template <typename T>
using Cachet_Result = mozilla::Result<T, Cachet_Bail>;

using Cachet_Err = mozilla::GenericErrorResult<Cachet_Bail>;

template <typename T>
inline Cachet_Result<T> Cachet_Ok(const T& value) {
  return value;
}

template <typename T>
inline Cachet_Result<T> Cachet_Ok(T&& value) {
  return value;
}

template <typename T>
inline bool Cachet_IsOk(const Cachet_Result<T>& result) {
  return result.isOk();
}

template <typename T>
inline T Cachet_Unwrap(Cachet_Result<T>&& result) {
  return result.unwrap();
}

inline void Cachet_Assert(bool cond) { MOZ_ASSERT(cond); }

template <typename T>
struct GCType {
  using Val = T;
  using Local = js::Rooted<T>;
  using Ref = js::Handle<T>;
  using OutRef = js::MutableHandle<T>;

  static inline bool CompareEq(Val lhs, Val rhs) { return lhs == rhs; }

  static inline bool CompareNeq(Val lhs, Val rhs) { return lhs != rhs; }

  static inline Val ToVal(const Local& local) { return local; }

  static inline Val ToVal(Ref ref) { return ref; }

  static inline Local EmptyLocal(Cachet_Context cx) { return Local(cx); }

  static inline Local ToLocal(Cachet_Context cx, const Val& val) {
    return Local(cx, val);
  }

  static inline Local ToLocal(Cachet_Context cx, Val&& val) {
    return Local(cx, std::move(val));
  }

  static inline Local ToLocal(Cachet_Context cx, const Local& local) {
    return Local(cx, local);
  }

  static inline Local ToLocal(Cachet_Context cx, Ref ref) {
    return Local(cx, ref);
  }

  static inline Ref ToRef(const Val& val) {
    return Ref::fromMarkedLocation(&val);
  }

  static inline Ref ToRef(const Local& local) { return local; }

  static inline OutRef ToOutRef(Local& local) { return &local; }

  static inline Val SetOutRef(OutRef out, const Val& in) {
    out.set(in);
    return out;
  }

  static inline Val SetOutRef(OutRef out, Val&& in) {
    out.set(std::move(in));
    return out;
  }

  static inline Val SetOutRef(OutRef out, const Local& in) {
    out.set(in);
    return out;
  }

  static inline Val SetOutRef(OutRef out, Ref in) {
    out.set(in);
    return out;
  }
};

using Type_ValueType = PrimitiveType<JS::ValueType>;
using Type_Value = GCType<JS::Value>;
using Type_Object = GCType<JSObject*>;
using Type_NativeObject = GCType<js::NativeObject*>;
using Type_Shape = GCType<js::Shape*>;
using Type_Class = PrimitiveType<const JSClass*>;

namespace Impl_ValueType {

inline constexpr Type_ValueType::Val Variant_Double = JS::ValueType::Double;
inline constexpr Type_ValueType::Val Variant_Int32 = JS::ValueType::Int32;
inline constexpr Type_ValueType::Val Variant_Boolean = JS::ValueType::Boolean;
inline constexpr Type_ValueType::Val Variant_Undefined =
    JS::ValueType::Undefined;
inline constexpr Type_ValueType::Val Variant_Null = JS::ValueType::Null;
inline constexpr Type_ValueType::Val Variant_Magic = JS::ValueType::Magic;
inline constexpr Type_ValueType::Val Variant_String = JS::ValueType::String;
inline constexpr Type_ValueType::Val Variant_Symbol = JS::ValueType::Symbol;
inline constexpr Type_ValueType::Val Variant_PrivateGCThing =
    JS::ValueType::PrivateGCThing;
inline constexpr Type_ValueType::Val Variant_BigInt = JS::ValueType::BigInt;
inline constexpr Type_ValueType::Val Variant_Object = JS::ValueType::Object;

};  // namespace Impl_ValueType

namespace Impl_NativeObject {

inline Type_NativeObject::Val From_Object(Type_Object::Val in) {
  return &in->as<NativeObject>();
}

inline Type_Object::Val To_Object(Type_NativeObject::Val in) { return in; }

};  // namespace Impl_NativeObject

#include "jit/CachetGenerated.h"

};  // namespace cachet

};  // namespace jit

};  // namespace js

#endif /* jit_CachetInterpreter_h */
