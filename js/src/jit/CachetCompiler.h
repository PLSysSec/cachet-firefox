/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_CachetCompiler_h
#define jit_CachetCompiler_h

#include "builtin/DataViewObject.h"
#include "builtin/MapObject.h"
#include "builtin/Object.h"
#include "jit/CacheIR.h"
#include "jit/CachetPrelude.h"
#include "jit/MacroAssembler.h"
#include "js/Class.h"
#include "js/RootingAPI.h"
#include "js/Value.h"
#include "vm/ArgumentsObject.h"
#include "vm/ArrayBufferObject.h"
#include "vm/ArrayBufferViewObject.h"
#include "vm/JSFunction.h"
#include "vm/JSObject.h"
#include "vm/NativeObject.h"
#include "vm/Shape.h"

namespace js {

namespace jit {

class CacheIRCompiler;
class OperandLocation;

namespace cachet {

using namespace ::cachet::prelude;

struct CachetContext {
  CacheIRCompiler* compiler;
  JSContext* js_ctx;
};

using Cachet_ContextRef = CachetContext;

inline void Cachet_Assert(bool cond) {
  MOZ_ASSERT(cond);
}

template <typename T>
struct GCType {
  using Val = T;
  using Local = js::Rooted<T>;
  using Ref = js::Handle<T>;
  using MutRef = js::MutableHandle<T>;

  static Val ToVal(const Local& local) {
    return local;
  }

  static Val ToVal(Ref ref) {
    return ref;
  }

  static Val ToVal(MutRef mutRef) {
    return mutRef;
  }

  static Local EmptyLocal(Cachet_ContextRef cx) {
    return Local(cx.js_ctx);
  }

  static Local ToLocal(Cachet_ContextRef cx, Val&& val) {
    return Local(cx.js_ctx, std::move(val));
  }

  static Local ToLocal(Cachet_ContextRef cx, Ref ref) {
    return Local(cx.js_ctx, ref);
  }

  static Local ToLocal(Cachet_ContextRef cx, MutRef mutRef) {
    return Local(cx.js_ctx, mutRef);
  }

  static Ref ToRef(const Local& local) {
    return local;
  }

  static Ref ToRef(MutRef mutRef) {
    return mutRef;
  }

  static MutRef ToMutRef(Local& local) {
    return &local;
  }

  static void SetMutRef(MutRef lhs, Val&& rhs) {
    lhs.set(std::move(rhs));
  }

  static void SetMutRef(MutRef lhs, const Local& rhs) {
    lhs.set(rhs);
  }

  static void SetMutRef(MutRef lhs, Ref rhs) {
    lhs.set(rhs);
  }

  static void SetMutRef(MutRef lhs, MutRef rhs) {
    lhs.set(rhs);
  }

  static bool Eq(Ref lhs, Ref rhs) {
    return lhs == rhs;
  }

  static bool Neq(Ref lhs, Ref rhs) {
    return lhs != rhs;
  }
};

using Type_Heap = Type_Unit;

enum class ABIFunction {
  EqualStringsHelperPure,
};

using Type_ABIFunction = PrimitiveType<ABIFunction>;
using Type_MIRType = PrimitiveType<MIRType>;
using Type_JSValueType = PrimitiveType<JSValueType>;
using Type_ValueType = PrimitiveType<JS::ValueType>;
using Type_Value = GCType<JS::Value>;
using Type_Float64 = NumericType<double>;
using Type_Object = GCType<JSObject*>;
using Type_String = GCType<JSString*>;
using Type_Atom = GCType<JSAtom*>;
using Type_Symbol = GCType<JS::Symbol*>;
using Type_BigInt = GCType<JS::BigInt*>;
using Type_NativeObject = GCType<js::NativeObject*>;
using Type_NativeObjectSlots = PrimitiveType<HeapSlot*>;
using Type_NativeObjectElements = PrimitiveType<HeapSlot*>;
using Type_JSFunction = GCType<JSFunction*>;
using Type_ArrayObject = GCType<js::ArrayObject*>;
using Type_PlainObject = GCType<js::PlainObject*>;
using Type_ArrayBufferObjectMaybeShared = GCType<js::ArrayBufferObjectMaybeShared*>;
using Type_ArrayBufferObject = GCType<js::ArrayBufferObject*>;
using Type_SharedArrayBufferObject = GCType<js::SharedArrayBufferObject*>;
using Type_ArrayBufferViewObject = GCType<js::ArrayBufferViewObject*>;
using Type_DataViewObject = GCType<js::DataViewObject*>;
using Type_ArgumentsObject = GCType<js::ArgumentsObject*>;
using Type_ArgumentsData = PrimitiveType<js::ArgumentsData*>;
using Type_MappedArgumentsObject = GCType<js::MappedArgumentsObject*>;
using Type_UnmappedArgumentsObject = GCType<js::UnmappedArgumentsObject*>;
using Type_SetObject = GCType<js::SetObject*>;
using Type_MapObject = GCType<js::MapObject*>;
using Type_Shape = GCType<js::Shape*>;
using Type_BaseShape = GCType<js::BaseShape*>;
using Type_Class = PrimitiveType<const JSClass*>;
using Type_TaggedProto = GCType<js::TaggedProto>;

using Type_ValueReg = PrimitiveType<ValueOperand>;
using Type_Reg = PrimitiveType<Register>;
using Type_PhyFloatReg = PrimitiveType<X86Encoding::XMMRegisterID>;
using Type_FloatContentType = PrimitiveType<FloatRegisters::ContentType>;
using Type_FloatReg = PrimitiveType<FloatRegister>;
using Type_AnyReg = PrimitiveType<AnyRegister>;
using Type_TypedOrValueReg = PrimitiveType<TypedOrValueRegister>;
using Type_GeneralRegSet = PrimitiveType<GeneralRegisterSet>;
using Type_FloatRegSet = PrimitiveType<FloatRegisterSet>;
using Type_LiveRegSet = PrimitiveType<LiveRegisterSet>;
using Type_Condition = PrimitiveType<Assembler::Condition>;
using Type_Address = PrimitiveType<js::jit::Address>;
using Type_Scale = PrimitiveType<js::jit::Scale>;
using Type_BaseIndex = PrimitiveType<js::jit::BaseIndex>;
using Type_BaseValueIndex = PrimitiveType<js::jit::BaseValueIndex>;
using Type_BaseObjectElementIndex = PrimitiveType<js::jit::BaseObjectElementIndex>;
using Type_BaseObjectSlotIndex = PrimitiveType<js::jit::BaseObjectSlotIndex>;

using Type_OperandLocation = PrimitiveType<OperandLocation>;

enum class OperandLocationKind {
  Uninitialized = 0,
  PayloadReg,
  ValueReg
};

using Type_OperandLocationKind = PrimitiveType<OperandLocationKind>;

using Type_OperandId = PrimitiveType<OperandId>;
using Type_ValueId = PrimitiveType<ValOperandId>;
using Type_ObjectId = PrimitiveType<ObjOperandId>;
using Type_Int32Id = PrimitiveType<Int32OperandId>;
using Type_NumberId = PrimitiveType<NumberOperandId>;
using Type_BooleanId = PrimitiveType<BooleanOperandId>;
using Type_StringId = PrimitiveType<StringOperandId>;
using Type_SymbolId = PrimitiveType<SymbolOperandId>;
using Type_BigIntId = PrimitiveType<BigIntOperandId>;
using Type_ValueTagId = PrimitiveType<ValueTagOperandId>;

class TypedOperandIdW : public TypedOperandId {
  bool isValueId_;

 public:
  TypedOperandIdW(TypedOperandId id, bool isValueId)
      : TypedOperandId(id), isValueId_(isValueId) {}

  bool operator==(const TypedOperandIdW& other) const { return id() == other.id() && type() == other.type(); }
  bool operator!=(const TypedOperandIdW& other) const { return id() != other.id() || type() != other.type(); }

  bool isValueId() {
    return isValueId_;
  }
};

using Type_TypedId = PrimitiveType<TypedOperandIdW>;

using Type_Int32Field = PrimitiveType<uint32_t>;
using Type_ObjectField = PrimitiveType<uint32_t>;
using Type_StringField = PrimitiveType<uint32_t>;
using Type_ShapeField = PrimitiveType<uint32_t>;
using Type_ClassField = PrimitiveType<uint32_t>;

using Type_GuardClassKind = PrimitiveType<GuardClassKind>;

inline Type_Heap::MutRef Var_heap(Cachet_ContextRef cx) {
  static Type_Heap::Local heap;
  return heap;
}

namespace IR_MASM {
  using LabelRef = Label*;
  using LabelLocal = Label*;
  using LabelMutRef = Label*&;
  using OpsRef = MacroAssembler&;

  LabelLocal NewLabel(Cachet_ContextRef cx);
  LabelMutRef ToLabelMutRef(LabelLocal& label);
  LabelRef ToLabelRef(LabelLocal label);

  IR_MASM::OpsRef GetOutput(Cachet_ContextRef cx);
  
  void BindLabel(Cachet_ContextRef cx, OpsRef ops, LabelMutRef label);
}



#define CACHET_CacheIR_COMPILER
#define CACHET_MASM_EMIT

#include "jit/CachetGenerated.h"

#undef CACHET_CacheIR_COMPILER
#undef CACHET_MASM_EMIT


// --------------------------------------------------
// Below this line, are accessors which are not
// generated by Cachet, but are used by CacheIRCompiler
// ----------------------------------------------------
//

namespace IR_CacheIR {

IR_MASM::OpsRef GetOutput(Cachet_ContextRef cx);

};  // namespace IR_CacheIR

namespace Impl_CacheIR {

Type_TypedOrValueReg::Ref Var_outputReg(Cachet_ContextRef cx);

};  // namespace Impl_CacheIR

};  // namespace cachet

};  // namespace jit

};  // namespace js

#endif /* jit_CachetCompiler_h */
