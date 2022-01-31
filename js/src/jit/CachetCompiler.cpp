/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/CachetCompiler.h"

using namespace js::jit::cachet;

namespace js {

namespace jit {

namespace cachet {

Type_ValueType::Val Impl_Value::Fn_typeOf(Cachet_ContextRef cx,
                                          Type_Value::Ref param_value) {
  return param_value.type();
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

Type_ValueReg::Val Impl_CacheIR::Fn_allocateValueReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler) {
  return compiler->allocator.allocateValueRegister(compiler->masm);
}

void Impl_CacheIR::Fn_releaseValueReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ValueReg::Ref param_valueReg) {
  return compiler->allocator.releaseValueRegister(param_valueReg);
}

Type_Reg::Val Impl_CacheIR::Fn_allocateReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler) {
  return compiler->allocator.allocateRegister(compiler->masm);
}

void Impl_CacheIR::Fn_releaseReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_Reg::Ref param_reg) {
  return compiler->allocator.releaseRegister(param_reg);
}

Type_ValueReg::Val Impl_CacheIR::Fn_useValueReg(
    Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler,
    Type_ValueId::Ref param_valueId) {
  OperandLocation loc(compiler->allocator.operandLocation(param_valueId.id()));
  if (loc.kind() == OperandLocation::ValueReg) {
    return loc.valueReg();
  } else {
    return compiler->allocator.useValueRegister(compiler->masm, param_valueId);
  }
}

Type_Reg::Val Impl_CacheIR::Fn_useObjectReg(Cachet_ContextRef cx,
                                            IR_CacheIR::CompilerRef compiler,
                                            Type_ObjectId::Ref param_objectId) {
  OperandLocation loc(compiler->allocator.operandLocation(param_objectId.id()));
  if (loc.kind() == OperandLocation::PayloadReg) {
    return loc.payloadReg();
  } else {
    return compiler->allocator.useRegister(compiler->masm, param_objectId);
  }
}

Type_Int32::Val Impl_CacheIR::Fn_readInt32Field(
    Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler,
    Type_Int32Field::Ref param_int32Field) {
  return compiler->int32StubField(param_int32Field);
}

Type_Shape::Val Impl_CacheIR::Fn_readShapeField(
    Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler,
    Type_ShapeField::Ref param_shapeField) {
  return compiler->shapeStubField(param_shapeField);
}

Type_Bool::Val Impl_CacheIR::Fn_objectGuardNeedsSpectreMitigations(
    Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler,
    Type_ObjectId::Ref param_objectId) {
  return compiler->objectGuardNeedsSpectreMitigations(param_objectId);
}

#include "jit/CachetGenerated.inc"

};  // namespace cachet

};  // namespace jit

};  // namespace js
