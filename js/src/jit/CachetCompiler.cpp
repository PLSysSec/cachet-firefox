/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/CachetCompiler.h"
#include "jit/CacheIRCompiler.h"

using namespace js::jit::cachet;

namespace js {

namespace jit {

namespace cachet {


namespace Impl_ValueType {

Type_ValueType::Ref Variant_Double(Cachet_ContextRef cx) {
  return JS::ValueType::Double;
}

Type_ValueType::Ref Variant_Int32(Cachet_ContextRef cx) {
  return JS::ValueType::Int32;
}

Type_ValueType::Ref Variant_Bool(Cachet_ContextRef cx) {
  return JS::ValueType::Boolean;
}

Type_ValueType::Ref Variant_Undefined(Cachet_ContextRef cx) {
  return JS::ValueType::Undefined;
}

Type_ValueType::Ref Variant_Null(Cachet_ContextRef cx) {
  return JS::ValueType::Null;
}

Type_ValueType::Ref Variant_Magic(Cachet_ContextRef cx) {
  return JS::ValueType::Magic;
}

Type_ValueType::Ref Variant_String(Cachet_ContextRef cx) {
  return JS::ValueType::String;
}

Type_ValueType::Ref Variant_Symbol(Cachet_ContextRef cx) {
  return JS::ValueType::Symbol;
}

Type_ValueType::Ref Variant_PrivateGCThing(Cachet_ContextRef cx) {
  return JS::ValueType::PrivateGCThing;
}

Type_ValueType::Ref Variant_BigInt(Cachet_ContextRef cx) {
  return JS::ValueType::BigInt;
}

Type_ValueType::Ref Variant_Object(Cachet_ContextRef cx) {
  return JS::ValueType::Object;
}

};  // namespace Impl_ValueType


namespace Impl_Object {

Type_NativeObject::Val To_NativeObject(Type_Object::Val param_in) {
  return &param_in->as<NativeObject>();
}

Type_NativeObject::Ref To_NativeObject(Type_Object::Ref param_in) {
  return param_in.as<NativeObject>();
}

};  // namespace Impl_Object


namespace Impl_NativeObject {

Type_Object::Val To_Object(Type_NativeObject::Val param_in) {
  return param_in;
}

Type_Object::Ref To_Object(Type_NativeObject::Ref param_in) {
  return Handle<JSObject*>::fromMarkedLocation(
      reinterpret_cast<JSObject* const*>(param_in.address()));
}

};  // namespace Impl_NativeObject



namespace Impl_Condition {

Type_Condition::Ref Variant_Equal(Cachet_ContextRef cx) {
  return Assembler::Condition::Equal;
}

Type_Condition::Ref Variant_NotEqual(Cachet_ContextRef cx) {
  return Assembler::Condition::NotEqual;
}

};  // namespace Impl_ValueType


namespace Impl_MASM {


void EmitOp_BranchTestObject(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_condition,
                                    Type_ValueReg::Ref param_valueReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestObject(param_condition, param_valueReg, param_branch);
}

void EmitOp_UnboxObject(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                               Type_ValueReg::Ref param_valueReg,
                               Type_Reg::Ref param_objectReg) {
  ops.unboxObject(param_valueReg, param_objectReg);
}

void EmitOp_BranchTestObjectShape(Cachet_ContextRef cx,
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

void EmitOp_BranchTestObjectShapeNoSpectreMitigations(
    Cachet_ContextRef cx, IR_MASM::OpsRef ops,
    Type_Condition::Ref param_condition, Type_Reg::Ref param_objectReg,
    Type_Shape::Ref param_shape, IR_MASM::LabelRef param_branch) {
  ops.branchTestObjShapeNoSpectreMitigations(param_condition, param_objectReg,
                                             param_shape, param_branch);
}

void EmitOp_LoadObjectFixedSlot(Cachet_ContextRef cx,
                                       IR_MASM::OpsRef ops,
                                       Type_Reg::Ref param_objectReg,
                                       Type_Int32::Ref param_slot,
                                       Type_ValueReg::Ref param_outputReg) {
  ops.loadTypedOrValue(Address(param_objectReg, param_slot), param_outputReg);
}

void EmitOp_BranchTestNull(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_Condition::Ref param_condition, Type_ValueReg::Ref param_valueReg, IR_MASM::LabelRef param_branch) {
  ops.branchTestNull(param_condition, param_valueReg, param_branch);
}

};  // namespace Impl_MASM


IR_MASM::OpsRef IR_CacheIR::GetOutput(Cachet_ContextRef cx) {
  return cx.compiler->masm;
}



Type_ValueReg::Ref Impl_CacheIR::Var_outputReg(Cachet_ContextRef cx) {
  return cx.compiler->outputUnchecked_.ref().valueReg();
}

Type_ValueType::Val Impl_Value::Fn_typeOf(Cachet_ContextRef cx,
                                          Type_Value::Ref param_value) {
  return param_value.type();
}

Type_Value::Val Impl_Value::Fn_fromInt32Unchecked(Cachet_ContextRef cx, Type_Int32::Ref param_value) {
  return Int32Value(param_value);
}

Type_Int32::Val Impl_Value::Fn_toInt32Unchecked(Cachet_ContextRef cx, Type_Value::Ref param_value) {
  return param_value.toInt32();
}


Type_Value::Val Impl_Value::Fn_fromObjectUnchecked(Cachet_ContextRef cx,
                                                   Type_Object::Ref param_value) {
  return ObjectValue(*param_value);
}

Type_Object::Val Impl_Value::Fn_toObjectUnchecked(Cachet_ContextRef cx,
                                                  Type_Value::Ref param_value) {
  return &param_value.toObject();
}

Type_Shape::Val Impl_Object::Fn_shapeOfUnchecked(Cachet_ContextRef cx,
                                                 Type_Heap::Ref param_heap,
                                                 Type_Object::Ref param_object) {
  return param_object->shape();
}

Type_Value::Val Impl_NativeObject::Fn_getFixedSlotUnchecked(
    Cachet_ContextRef cx, Type_Heap::Ref param_heap, Type_NativeObject::Ref param_nativeObject,
    Type_Int32::Ref param_slot) {
  param_slot = NativeObject::getFixedSlotIndexFromOffset(param_slot);
  return param_nativeObject->getFixedSlot(param_slot);
}

Type_Class::Val Impl_Shape::Fn_classOf(Cachet_ContextRef cx,
                                       Type_Shape::Ref param_shape) {
  return param_shape->getObjectClass();
}

Type_Bool::Val Impl_Shape::Fn_hasFixedSlot(Cachet_ContextRef cx,
                                           Type_Shape::Ref param_shape,
                                           Type_Int32::Ref param_slot) {
  param_slot = NativeObject::getFixedSlotIndexFromOffset(param_slot);
  return param_slot >= 0 &&
         param_slot < Type_Int32::Val(param_shape->numFixedSlots());
}

Type_Bool::Val Impl_Class::Fn_isNativeObject(Cachet_ContextRef cx,
                                             Type_Class::Ref param_class) {
  return param_class->isNativeObject();
}

Type_ValueReg::Val Impl_CacheIR::Fn_allocateValueReg(Cachet_ContextRef cx) {
  return cx.compiler->allocator.allocateValueRegister(cx.compiler->masm);
}

void Impl_CacheIR::Fn_releaseValueReg(Cachet_ContextRef cx, Type_ValueReg::Ref param_valueReg) {
  return cx.compiler->allocator.releaseValueRegister(param_valueReg);
}

Type_Reg::Val Impl_CacheIR::Fn_allocateReg(Cachet_ContextRef cx) {
  return cx.compiler->allocator.allocateRegister(cx.compiler->masm);
}

void Impl_CacheIR::Fn_releaseReg(Cachet_ContextRef cx, Type_Reg::Ref param_reg) {
  return cx.compiler->allocator.releaseRegister(param_reg);
}

Type_ValueReg::Val Impl_CacheIR::Fn_useValueReg(
    Cachet_ContextRef cx,
    Type_ValueId::Ref param_valueId) {
  OperandLocation loc(cx.compiler->allocator.operandLocation(param_valueId.id()));
  if (loc.kind() == OperandLocation::ValueReg) {
    return loc.valueReg();
  } else {
    return cx.compiler->allocator.useValueRegister(cx.compiler->masm, param_valueId);
  }
}

Type_Reg::Val Impl_CacheIR::Fn_useObjectReg(Cachet_ContextRef cx,
                                            Type_ObjectId::Ref param_objectId) {
  OperandLocation loc(cx.compiler->allocator.operandLocation(param_objectId.id()));
  if (loc.kind() == OperandLocation::PayloadReg) {
    return loc.payloadReg();
  } else {
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_objectId);
  }
}

Type_Int32::Val Impl_CacheIR::Fn_readInt32Field(
    Cachet_ContextRef cx,
    Type_Int32Field::Ref param_int32Field) {
  return cx.compiler->int32StubField(param_int32Field);
}

Type_Shape::Val Impl_CacheIR::Fn_readShapeField(
    Cachet_ContextRef cx,
    Type_ShapeField::Ref param_shapeField) {
  return cx.compiler->shapeStubField(param_shapeField);
}

Type_Bool::Val Impl_CacheIR::Fn_objectGuardNeedsSpectreMitigations(
    Cachet_ContextRef cx,
    Type_ObjectId::Ref param_objectId) {
  return cx.compiler->objectGuardNeedsSpectreMitigations(param_objectId);
}

#define CACHET_CacheIR_COMPILER
#define CACHET_MASM_EMIT

#include "jit/CachetGenerated.inc"

#undef CACHET_CacheIR_COMPILER
#undef CACHET_MASM_EMIT

};  // namespace cachet

};  // namespace jit

};  // namespace js
