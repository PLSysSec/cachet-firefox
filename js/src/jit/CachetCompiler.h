/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_CachetCompiler_h
#define jit_CachetCompiler_h

#include "jit/CacheIR.h"
#include "jit/CachetPrelude.h"
#include "jit/IonCacheIRCompiler.h"
#include "jit/MacroAssembler.h"
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

using Cachet_ContextRef = JSContext*;

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
    return Local(cx);
  }

  static Local ToLocal(Cachet_ContextRef cx, Val&& val) {
    return Local(cx, std::move(val));
  }

  static Local ToLocal(Cachet_ContextRef cx, Ref ref) {
    return Local(cx, ref);
  }

  static Local ToLocal(Cachet_ContextRef cx, MutRef mutRef) {
    return Local(cx, mutRef);
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

  static bool CompareEq(Ref lhs, Ref rhs) {
    return lhs == rhs;
  }

  static bool CompareNeq(Ref lhs, Ref rhs) {
    return lhs != rhs;
  }
};

using Type_Heap = Type_Unit;

using Type_ValueType = PrimitiveType<JS::ValueType>;
using Type_Value = GCType<JS::Value>;
using Type_Object = GCType<JSObject*>;
using Type_NativeObject = GCType<js::NativeObject*>;
using Type_Shape = GCType<js::Shape*>;
using Type_Class = PrimitiveType<const JSClass*>;

using Type_ValueReg = PrimitiveType<ValueOperand>;
using Type_Reg = PrimitiveType<Register>;
using Type_Condition = PrimitiveType<Assembler::Condition>;

using Type_ValueId = PrimitiveType<ValOperandId>;
using Type_ObjectId = PrimitiveType<ObjOperandId>;

using Type_Int32Field = PrimitiveType<uint32_t>;
using Type_ShapeField = PrimitiveType<uint32_t>;

inline Type_Heap::MutRef Var_heap(Cachet_ContextRef cx) {
  static Type_Heap::Local heap;
  return heap;
}

namespace Impl_ValueType {

inline Type_ValueType::Ref Variant_Double(Cachet_ContextRef cx) {
  return JS::ValueType::Double;
}

inline Type_ValueType::Ref Variant_Int32(Cachet_ContextRef cx) {
  return JS::ValueType::Int32;
}

inline Type_ValueType::Ref Variant_Bool(Cachet_ContextRef cx) {
  return JS::ValueType::Boolean;
}

inline Type_ValueType::Ref Variant_Undefined(Cachet_ContextRef cx) {
  return JS::ValueType::Undefined;
}

inline Type_ValueType::Ref Variant_Null(Cachet_ContextRef cx) {
  return JS::ValueType::Null;
}

inline Type_ValueType::Ref Variant_Magic(Cachet_ContextRef cx) {
  return JS::ValueType::Magic;
}

inline Type_ValueType::Ref Variant_String(Cachet_ContextRef cx) {
  return JS::ValueType::String;
}

inline Type_ValueType::Ref Variant_Symbol(Cachet_ContextRef cx) {
  return JS::ValueType::Symbol;
}

inline Type_ValueType::Ref Variant_PrivateGCThing(Cachet_ContextRef cx) {
  return JS::ValueType::PrivateGCThing;
}

inline Type_ValueType::Ref Variant_BigInt(Cachet_ContextRef cx) {
  return JS::ValueType::BigInt;
}

inline Type_ValueType::Ref Variant_Object(Cachet_ContextRef cx) {
  return JS::ValueType::Object;
}

};  // namespace Impl_ValueType

namespace Impl_NativeObject {

inline Type_NativeObject::Val From_Object(Type_Object::Val param_in) {
  return &param_in->as<NativeObject>();
}

inline Type_NativeObject::Ref From_Object(Type_Object::Ref param_in) {
  return param_in.as<NativeObject>();
}

inline Type_Object::Val To_Object(Type_NativeObject::Val param_in) {
  return param_in;
}

inline Type_Object::Ref To_Object(Type_NativeObject::Ref param_in) {
  return Handle<JSObject*>::fromMarkedLocation(
      reinterpret_cast<JSObject* const*>(param_in.address()));
}

};  // namespace Impl_NativeObject

namespace Impl_Condition {

inline Type_Condition::Ref Variant_Equal(Cachet_ContextRef cx) {
  return Assembler::Condition::Equal;
}

inline Type_Condition::Ref Variant_NotEqual(Cachet_ContextRef cx) {
  return Assembler::Condition::NotEqual;
}

};  // namespace Impl_ValueType

namespace IR_MASM {

using LabelRef = Label*;
using OpsRef = MacroAssembler&;

inline void EmitOp_BranchTestObject(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_condition,
                                    Type_ValueReg::Ref param_valueReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestObject(param_condition, param_valueReg, param_branch);
}

inline void EmitOp_UnboxObject(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                               Type_ValueReg::Ref param_valueReg,
                               Type_Reg::Ref param_objectReg) {
  ops.unboxObject(param_valueReg, param_objectReg);
}

inline void EmitOp_BranchTestObjectShape(Cachet_ContextRef cx,
                                         IR_MASM::OpsRef ops,
                                         Type_Condition::Ref param_condition,
                                         Type_Reg::Ref param_objectReg,
                                         Type_Shape::Ref param_shape,
                                         Type_Reg::Ref param_scratchReg,
                                         Type_Reg::Ref param_spectreRegToZero,
                                         IR_MASM::LabelRef param_branch) {
  ops.branchTestObjShape(param_condition, param_objectReg, param_shape,
                         param_scratchReg, param_spectreRegToZero,
                         param_branch);
}

inline void EmitOp_BranchTestObjectShapeNoSpectreMitigations(
    Cachet_ContextRef cx, IR_MASM::OpsRef ops,
    Type_Condition::Ref param_condition, Type_Reg::Ref param_objectReg,
    Type_Shape::Ref param_shape, IR_MASM::LabelRef param_branch) {
  ops.branchTestObjShapeNoSpectreMitigations(param_condition, param_objectReg,
                                             param_shape, param_branch);
}

inline void EmitOp_LoadObjectFixedSlot(Cachet_ContextRef cx,
                                       IR_MASM::OpsRef ops,
                                       Type_Reg::Ref param_objectReg,
                                       Type_Int32::Ref param_slot,
                                       Type_ValueReg::Ref param_outputReg) {
  ops.loadTypedOrValue(Address(param_objectReg, param_slot), param_outputReg);
}

};  // namespace IR_MASM

namespace IR_CacheIR {

using CompilerRef = CacheIRCompiler*;

inline IR_MASM::OpsRef GetOutput(CompilerRef compiler) {
  return compiler->masm;
}

};  // namespace IR_CacheIR

namespace Impl_CacheIR {

inline Type_ValueReg::Ref Var_outputReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler) {
  return compiler->outputUnchecked_.ref().valueReg();
}

};  // namespace Impl_CacheIR

#include "jit/CachetGenerated.h"

};  // namespace cachet

};  // namespace jit

};  // namespace js

#endif /* jit_CachetCompiler_h */
