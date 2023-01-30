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

namespace Impl_JSValueType {

inline Type_JSValueType::Ref Variant_Double(Cachet_ContextRef cx) {
  return JSVAL_TYPE_DOUBLE;
}

inline Type_JSValueType::Ref Variant_Int32(Cachet_ContextRef cx) {
  return JSVAL_TYPE_INT32;
}

inline Type_JSValueType::Ref Variant_Bool(Cachet_ContextRef cx) {
  return JSVAL_TYPE_BOOLEAN;
}

inline Type_JSValueType::Ref Variant_Undefined(Cachet_ContextRef cx) {
  return JSVAL_TYPE_UNDEFINED;
}

inline Type_JSValueType::Ref Variant_Null(Cachet_ContextRef cx) {
  return JSVAL_TYPE_NULL;
}

inline Type_JSValueType::Ref Variant_Magic(Cachet_ContextRef cx) {
  return JSVAL_TYPE_MAGIC;
}

inline Type_JSValueType::Ref Variant_String(Cachet_ContextRef cx) {
  return JSVAL_TYPE_STRING;
}

inline Type_JSValueType::Ref Variant_Symbol(Cachet_ContextRef cx) {
  return JSVAL_TYPE_SYMBOL;
}

inline Type_JSValueType::Ref Variant_PrivateGCThing(Cachet_ContextRef cx) {
  return JSVAL_TYPE_PRIVATE_GCTHING;
}

inline Type_JSValueType::Ref Variant_BigInt(Cachet_ContextRef cx) {
  return JSVAL_TYPE_BIGINT;
}

inline Type_JSValueType::Ref Variant_Object(Cachet_ContextRef cx) {
  return JSVAL_TYPE_OBJECT;
}

inline Type_JSValueType::Ref Variant_Unknown(Cachet_ContextRef cx) {
  return JSVAL_TYPE_UNKNOWN;
}

Type_JSValueType::Val Fn_fromValueType(Cachet_ContextRef cx, Type_ValueType::Ref param_valTy) {
  return JSValueType(param_valTy);
}

};  // namespace Impl_JSValueType

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

Type_Condition::Ref Variant_Overflow(Cachet_ContextRef cx) {
  return Assembler::Condition::Overflow;
}

Type_Condition::Ref Variant_Zero(Cachet_ContextRef cx) {
  return Assembler::Condition::Zero;
}

Type_Condition::Ref Variant_NonZero(Cachet_ContextRef cx) {
  return Assembler::Condition::NonZero;
}

Type_Condition::Ref Variant_Signed(Cachet_ContextRef cx) {
  return Assembler::Condition::Signed;
}

Type_Condition::Ref Variant_NotSigned(Cachet_ContextRef cx) {
  return Assembler::Condition::NotSigned;
}

Type_Condition::Ref Variant_GreaterThan(Cachet_ContextRef cx) {
  return Assembler::Condition::GreaterThan;
}

Type_Condition::Ref Variant_LessThan(Cachet_ContextRef cx) {
  return Assembler::Condition::LessThan;
}

};  // namespace Impl_Condition

namespace Impl_Address {
Type_Address::Val Fn_newUnchecked(Cachet_ContextRef cx, Type_Reg::Ref param_base, Type_Int32::Ref param_offset) {
  return Address(param_base, param_offset);
}

Type_Reg::Val Fn_base(Cachet_ContextRef cx, Type_Address::Ref param_address) {
  return param_address.base;
}

Type_Int32::Val Fn_offset(Cachet_ContextRef cx, Type_Address::Ref param_address) {
  return param_address.offset;
}

};


namespace Impl_MASM {

void EmitOp_Mov(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                      Type_Reg::Ref param_srcReg, Type_Reg::Ref param_dstReg) {
  ops.mov(param_srcReg, param_dstReg);
}

void EmitOp_Move32(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                    Type_Int32::Val int32, Type_Reg::Ref param_dstReg) {
  ops.move32(Imm32(int32), param_dstReg); 
}

void EmitOp_Cmp32Move32(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                    Type_Condition::Ref param_condition,
                    Type_Reg::Ref param_lhsReg,
                    Type_Reg::Ref param_rhsReg,
                    Type_Reg::Ref param_srcReg,
                    Type_Reg::Ref param_dstReg) {

  ops.cmp32Move32(param_condition, param_lhsReg, param_rhsReg, param_srcReg, param_dstReg);
}

void EmitOp_Jump(Cachet_ContextRef cx, IR_MASM::OpsRef ops, IR_MASM::LabelRef param_target) {
  ops.jump(param_target);
}

void EmitOp_LoadValueAddress(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Address::Ref param_address,
                                    Type_ValueReg::Ref param_dstReg) {
  ops.loadValue(param_address, param_dstReg);
}

void EmitOp_UnboxInt32(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                          Type_ValueReg::Ref param_valueReg,
                          Type_Reg::Ref param_int32Reg) {
  ops.unboxInt32(param_valueReg, param_int32Reg);
}

void EmitOp_UnboxObject(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                               Type_ValueReg::Ref param_valueReg,
                               Type_Reg::Ref param_objectReg) {
  ops.unboxObject(param_valueReg, param_objectReg);
}

void EmitOp_BranchTestNumber(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_condition,
                                    Type_ValueReg::Ref param_valueReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestNumber(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestNumberTag(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                            Type_Condition::Ref param_condition,
                            Type_Reg::Ref param_tagReg,
                            IR_MASM::LabelRef param_branch) {
  ops.branchTestNumber(param_condition, param_tagReg, param_branch);
}

void EmitOp_BranchTestBoolean(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_Condition::Ref param_condition, Type_ValueReg::Ref param_valueReg, IR_MASM::LabelRef param_branch) {
  ops.branchTestBoolean(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestString(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_Condition::Ref param_condition, Type_ValueReg::Ref param_valueReg, IR_MASM::LabelRef param_branch) {
  ops.branchTestString(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestSymbol(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_Condition::Ref param_condition, Type_ValueReg::Ref param_valueReg, IR_MASM::LabelRef param_branch) {
  ops.branchTestSymbol(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestBigInt(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_Condition::Ref param_condition, Type_ValueReg::Ref param_valueReg, IR_MASM::LabelRef param_branch) {
  ops.branchTestBigInt(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestNull(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_Condition::Ref param_condition, Type_ValueReg::Ref param_valueReg, IR_MASM::LabelRef param_branch) {
  ops.branchTestNull(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestObject(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_condition,
                                    Type_ValueReg::Ref param_valueReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestObject(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestObjClass(Cachet_ContextRef cx,
                                    IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_condition,
                                    Type_Reg::Ref param_objectReg,
                                    Type_Class::Ref param_class,
                                    Type_Reg::Ref param_scratchReg,
                                    Type_Reg::Ref param_spectreRegToZero,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestObjClass(param_condition, param_objectReg, param_class,
                         param_scratchReg, param_spectreRegToZero,
                         param_branch);
}

void EmitOp_BranchTestObjClassNoSpectreMitigations(Cachet_ContextRef cx,
                                    IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_condition,
                                    Type_Reg::Ref param_objectReg,
                                    Type_Class::Ref param_class,
                                    Type_Reg::Ref param_scratchReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestObjClassNoSpectreMitigations(param_condition, param_objectReg,
                                             param_class, param_scratchReg,
                                             param_branch);
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

void EmitOp_BranchTestUndefined(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_condition,
                                    Type_ValueReg::Ref param_valueReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestUndefined(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestInt32(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_condition,
                                    Type_ValueReg::Ref param_valueReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestInt32(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTest32(Cachet_ContextRef cx,
                            IR_MASM::OpsRef ops,
                            Type_Condition::Ref param_condition,
                            Type_Reg::Ref param_lhsReg,
                            Type_Reg::Ref param_rhsReg,
                            IR_MASM::LabelRef param_branch) {
  ops.branchTest32(param_condition, param_lhsReg, param_rhsReg, param_branch);
}

void EmitOp_BranchTest32Imm(Cachet_ContextRef cx,
                            IR_MASM::OpsRef ops,
                            Type_Condition::Ref param_condition,
                            Type_Reg::Ref param_lhsReg,
                            Type_Int32::Ref param_rhsInt32,
                            IR_MASM::LabelRef param_branch) {
  ops.branchTest32(param_condition, param_lhsReg, Imm32(param_rhsInt32), param_branch);
}

void EmitOp_Branch32Imm(Cachet_ContextRef cx,
                        IR_MASM::OpsRef ops,
                        Type_Condition::Ref param_condition,
                        Type_Reg::Ref param_lhsReg,
                        Type_Int32::Ref param_rhsInt32,
                        IR_MASM::LabelRef param_branch) {
  ops.branch32(param_condition, param_lhsReg, Imm32(param_rhsInt32), param_branch);
}

void EmitOp_Branch32Tag(Cachet_ContextRef cx,
                        IR_MASM::OpsRef ops,
                        Type_Condition::Ref param_condition,
                        Type_Reg::Ref param_lhsReg,
                        Type_Reg::Ref param_rhsReg,
                        IR_MASM::LabelRef param_branch) {
  ops.branch32(param_condition, param_lhsReg, param_rhsReg, param_branch);
}

void EmitOp_BranchIfNonNativeObj(Cachet_ContextRef cx,
                                  IR_MASM::OpsRef ops,
                                  Type_Reg::Ref param_objectReg,
                                  Type_Reg::Ref param_scratchReg,
                                  IR_MASM::LabelRef param_branch) {
  ops.branchIfNonNativeObj(param_objectReg, param_scratchReg, param_branch);
}

void EmitOp_BranchTestObjectIsProxy(Cachet_ContextRef cx,
                                    IR_MASM::OpsRef ops,
                                    Type_Bool::Ref param_proxy,
                                    Type_Reg::Ref param_objectReg,
                                    Type_Reg::Ref param_scratchReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestObjectIsProxy(param_proxy, param_objectReg, param_scratchReg, param_branch);
}


void EmitOp_TagValue(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_JSValueType::Ref param_valTy,
                      Type_Reg::Ref param_payload, Type_ValueReg::Ref param_dest) {
  ops.tagValue(param_valTy, param_payload, param_dest);
}

void EmitOp_BranchAdd32(Cachet_ContextRef cx,
                                         IR_MASM::OpsRef ops,
                                         Type_Condition::Ref param_condition,
                                         Type_Reg::Ref param_srcReg,
                                         Type_Reg::Ref param_dstReg,
                                         IR_MASM::LabelRef param_branch) {
  ops.branchAdd32(param_condition, param_srcReg, param_dstReg,
                         param_branch);
}

void EmitOp_BranchSub32(Cachet_ContextRef cx,
                                         IR_MASM::OpsRef ops,
                                         Type_Condition::Ref param_condition,
                                         Type_Reg::Ref param_srcReg,
                                         Type_Reg::Ref param_dstReg,
                                         IR_MASM::LabelRef param_branch) {
  ops.branchSub32(param_condition, param_srcReg, param_dstReg,
                         param_branch);
}

void EmitOp_BranchMul32(Cachet_ContextRef cx,
                                         IR_MASM::OpsRef ops,
                                         Type_Condition::Ref param_condition,
                                         Type_Reg::Ref param_srcReg,
                                         Type_Reg::Ref param_dstReg,
                                         IR_MASM::LabelRef param_branch) {
  ops.branchMul32(param_condition, param_srcReg, param_dstReg,
                         param_branch);
}

void EmitOp_Neg32(Cachet_ContextRef cx,
                  IR_MASM::OpsRef ops,
                  Type_Reg::Ref param_valueReg) {
  ops.neg32(param_valueReg);
}

void EmitOp_Or32(Cachet_ContextRef cx,
                                         IR_MASM::OpsRef ops,
                                         Type_Reg::Ref param_srcReg,
                                         Type_Reg::Ref param_dstReg) {
  ops.or32(param_srcReg, param_dstReg);
}

};  // namespace Impl_MASM


IR_MASM::OpsRef IR_CacheIR::GetOutput(Cachet_ContextRef cx) {
  return cx.compiler->masm;
}

IR_MASM::LabelLocal IR_MASM::NewLabel(Cachet_ContextRef cx) {
  return nullptr;
}

IR_MASM::LabelMutRef IR_MASM::ToLabelMutRef(IR_MASM::LabelLocal& label) {
  return label;
}

IR_MASM::LabelRef IR_MASM::ToLabelRef(IR_MASM::LabelLocal label) {
  return label;
}

void IR_MASM::BindLabel(Cachet_ContextRef cx, OpsRef ops, IR_MASM::LabelMutRef label) {
  ops.bind(label);
}

Type_ValueReg::Ref Impl_CacheIR::Var_outputReg(Cachet_ContextRef cx) {
  return cx.compiler->outputUnchecked_.ref().valueReg();
}

Type_ValueType::Val Impl_Value::Fn_typeOf(Cachet_ContextRef cx,
                                          Type_Value::Ref param_value) {
  return param_value.type();
}

Type_Value::Val Impl_Value::Fn_fromDoubleUnchecked(Cachet_ContextRef cx, Type_Double::Ref param_value) {
  return DoubleValue(param_value);
}

Type_Double::Val Impl_Value::Fn_toDoubleUnchecked(Cachet_ContextRef cx, Type_Value::Ref param_value) {
  return param_value.toDouble();
}

Type_Value::Val Impl_Value::Fn_fromInt32Unchecked(Cachet_ContextRef cx, Type_Int32::Ref param_value) {
  return Int32Value(param_value);
}

Type_Int32::Val Impl_Value::Fn_toInt32Unchecked(Cachet_ContextRef cx, Type_Value::Ref param_value) {
  return param_value.toInt32();
}

Type_Value::Val Impl_Value::Fn_fromBoolUnchecked(Cachet_ContextRef cx, Type_Bool::Ref param_value) {
  return BooleanValue(param_value);
}

Type_Bool::Val Impl_Value::Fn_toBoolUnchecked(Cachet_ContextRef cx, Type_Value::Ref param_value) {
  return param_value.toBoolean();
}

Type_Value::Val Impl_Value::Fn_fromObjectUnchecked(Cachet_ContextRef cx,
                                                   Type_Object::Ref param_value) {
  return ObjectValue(*param_value);
}

Type_Object::Val Impl_Value::Fn_toObjectUnchecked(Cachet_ContextRef cx,
                                                  Type_Value::Ref param_value) {
  return &param_value.toObject();
}

Type_Value::Val Impl_Value::Fn_fromStringUnchecked(Cachet_ContextRef cx,
                                                   Type_String::Ref param_value) {
  return StringValue(param_value);
}

Type_String::Val Impl_Value::Fn_toStringUnchecked(Cachet_ContextRef cx,
                                                  Type_Value::Ref param_value) {
  return param_value.toString();
}

Type_Value::Val Impl_Value::Fn_fromSymbolUnchecked(Cachet_ContextRef cx,
                                                   Type_Symbol::Ref param_value) {
  return SymbolValue(param_value);
}

Type_Symbol::Val Impl_Value::Fn_toSymbolUnchecked(Cachet_ContextRef cx,
                                                  Type_Value::Ref param_value) {
  return param_value.toSymbol();
}

Type_Value::Val Impl_Value::Fn_fromBigIntUnchecked(Cachet_ContextRef cx,
                                                   Type_BigInt::Ref param_value) {
  return BigIntValue(param_value);
}

Type_BigInt::Val Impl_Value::Fn_toBigIntUnchecked(Cachet_ContextRef cx,
                                                  Type_Value::Ref param_value) {
  return param_value.toBigInt();
}

Type_Shape::Val Impl_Object::Fn_shapeOfUnchecked(Cachet_ContextRef cx,
                                                 Type_Heap::Ref param_heap,
                                                 Type_Object::Ref param_object) {
  return param_object->shape();
}

Type_Value::Val Impl_NativeObject::Fn_getFixedSlotUnchecked(
    Cachet_ContextRef cx, Type_Heap::Ref param_heap, Type_NativeObject::Ref param_nativeObject,
    Type_UInt32::Ref param_slot) {
  return param_nativeObject->getFixedSlot(param_slot);
}

namespace Impl_JSFunction {

inline Type_Class::Ref Var_rawClass(Cachet_ContextRef cx) {
  return &JSFunction::class_;
}

};  // namespace Impl_JSFunction

namespace Impl_ArrayObject {

inline Type_Class::Ref Var_rawClass(Cachet_ContextRef cx) {
  return &ArrayObject::class_;
}

};  // namespace Impl_ArrayObject

namespace Impl_PlainObject {

inline Type_Class::Ref Var_rawClass(Cachet_ContextRef cx) {
  return &PlainObject::class_;
}

};  // namespace Impl_PlainObject

namespace Impl_ArrayBufferObject {

inline Type_Class::Ref Var_rawClass(Cachet_ContextRef cx) {
  return &ArrayBufferObject::class_;
}

};  // namespace Impl_ArrayBufferObject

namespace Impl_SharedArrayBufferObject {

inline Type_Class::Ref Var_rawClass(Cachet_ContextRef cx) {
  return &SharedArrayBufferObject::class_;
}

};  // namespace Impl_SharedArrayBufferObject

namespace Impl_DataViewObject {

inline Type_Class::Ref Var_rawClass(Cachet_ContextRef cx) {
  return &DataViewObject::class_;
}

};  // namespace Impl_DataViewObject

namespace Impl_UnmappedArgumentsObject {

inline Type_Class::Ref Var_rawClass(Cachet_ContextRef cx) {
  return &UnmappedArgumentsObject::class_;
}

};  // namespace Impl_UnmappedArgumentsObject

namespace Impl_MappedArgumentsObject {

inline Type_Class::Ref Var_rawClass(Cachet_ContextRef cx) {
  return &MappedArgumentsObject::class_;
}

};  // namespace Impl_MappedArgumentsObject

namespace Impl_SetObject {

inline Type_Class::Ref Var_rawClass(Cachet_ContextRef cx) {
  return &SetObject::class_;
}

};  // namespace Impl_SetObject

namespace Impl_MapObject {

inline Type_Class::Ref Var_rawClass(Cachet_ContextRef cx) {
  return &MapObject::class_;
}

};  // namespace Impl_MapObject

namespace Impl_Shape {

Type_BaseShape::Val Fn_baseShapeOf(Cachet_ContextRef cx, Type_Shape::Ref param_shape) {
  return param_shape->base();
}

Type_Bool::Val Fn_hasFixedSlot(Cachet_ContextRef cx,
                                           Type_Shape::Ref param_shape,
                                           Type_UInt32::Ref param_slot) {
  return param_slot >= 0 &&
         param_slot < Type_UInt32::Val(param_shape->numFixedSlots());
}

}; // namespace Impl_Shape

namespace Impl_BaseShape {

Type_Class::Val Fn_classOf(Cachet_ContextRef cx, Type_BaseShape::Ref param_baseShape) {
  return param_baseShape->clasp();
}

};  // namespace Impl_BaseShape

namespace Impl_Class {

Type_Class::Val Fn_windowProxyClass(Cachet_ContextRef cx) {
  return cx.js_ctx->runtime()->maybeWindowProxyClass();
}

Type_Bool::Val Fn_isNativeObject(Cachet_ContextRef cx, Type_Class::Ref param_class) {
  return param_class->isNativeObject();
}

Type_Bool::Val Fn_isProxyObject(Cachet_ContextRef cx, Type_Class::Ref param_class) {
  return param_class->isProxyObject();
}

};  // namespace Impl_Class

namespace Impl_OperandLocationKind {

Type_OperandLocationKind::Ref Variant_Uninitialized(Cachet_ContextRef cx) {
  return OperandLocationKind::Uninitialized;
}

Type_OperandLocationKind::Ref Variant_ValueReg(Cachet_ContextRef cx) {
  return OperandLocationKind::ValueReg;
}

Type_OperandLocationKind::Ref Variant_PayloadReg(Cachet_ContextRef cx) {
  return OperandLocationKind::PayloadReg;
}

}

namespace Impl_OperandLocation {

Type_OperandLocationKind::Val Fn_kind(Cachet_ContextRef cx, Type_OperandLocation::Ref param_loc) {
  return Type_OperandLocationKind::Val(param_loc.kind());
}

Type_OperandLocation::Val Fn_newUninitializedUnchecked(Cachet_ContextRef cx) {
  return OperandLocation();
}

void Fn_setValueRegUnchecked(Cachet_ContextRef cx, Type_OperandLocation::MutRef param_loc, Type_ValueReg::Ref param_valueReg) {
  param_loc.setValueReg(param_valueReg);
}

Type_ValueReg::Val Fn_getValueRegUnchecked(Cachet_ContextRef cx, Type_OperandLocation::Ref param_loc) {
  return param_loc.valueReg();
}

void Fn_setPayloadRegUnchecked(Cachet_ContextRef cx, Type_OperandLocation::MutRef param_loc, Type_Reg::Ref param_reg, Type_JSValueType::Ref param_type) {
  param_loc.setPayloadReg(param_reg, param_type);
}

Type_Reg::Val Fn_getPayloadRegUnchecked(Cachet_ContextRef cx, Type_OperandLocation::Ref param_loc) {
  return param_loc.payloadReg();
}

Type_JSValueType::Val Fn_getPayloadTypeUnchecked(Cachet_ContextRef cx, Type_OperandLocation::Ref param_loc) {
  return param_loc.payloadType();
}

}

inline Type_OperandId::Val Impl_ValueId::To_OperandId(Type_ValueId::Val in) {
  return static_cast<Type_OperandId::Val>(in);
}

inline Type_OperandId::Val Impl_ObjectId::To_OperandId(Type_ObjectId::Val in) {
  return static_cast<Type_OperandId::Val>(in);
}

inline Type_OperandId::Val Impl_Int32Id::To_OperandId(Type_Int32Id::Val in) {
  return static_cast<Type_OperandId::Val>(in);
}

inline Type_OperandId::Val Impl_BooleanId::To_OperandId(Type_BooleanId::Val in) {
  return static_cast<Type_OperandId::Val>(in);
}

inline Type_OperandId::Val Impl_ValueTagId::To_OperandId(Type_ValueTagId::Val in) {
  return static_cast<Type_OperandId::Val>(in);
}

namespace Impl_TypedId {

inline Type_OperandId::Val To_OperandId(Type_TypedId::Val in) {
  return static_cast<Type_OperandId::Val>(in);
}

Type_JSValueType::Val Fn_type(Cachet_ContextRef cx, Type_TypedId::Ref param_typedId) {
  return param_typedId.type();
}

Type_Bool::Val Fn_isValueId(Cachet_ContextRef cx, Type_TypedId::Ref param_typedId) {
  return param_typedId.isValueId();
}

Type_TypedId::Val Fn_fromValueIdUnchecked(Cachet_ContextRef cx, Type_ValueId::Ref param_valueId, Type_JSValueType::Ref param_type) {
  return TypedOperandIdW(TypedOperandId(param_valueId, param_type), true);
}

Type_ValueId::Val Fn_toValueIdUnchecked(Cachet_ContextRef cx, Type_TypedId::Ref param_typedId) {
  return Type_ValueId::Val(param_typedId.id());
}

Type_TypedId::Val Fn_fromObjectIdUnchecked(Cachet_ContextRef cx, Type_ObjectId::Ref param_objectId) {
  return TypedOperandIdW(TypedOperandId(param_objectId), false);
}

Type_ObjectId::Val Fn_toObjectIdUnchecked(Cachet_ContextRef cx, Type_TypedId::Ref param_typedId) {
  return Type_ObjectId::Val(param_typedId.id());
}

Type_TypedId::Val Fn_fromInt32IdUnchecked(Cachet_ContextRef cx, Type_Int32Id::Ref param_int32Id) {
  return TypedOperandIdW(TypedOperandId(param_int32Id), false);
}

Type_Int32Id::Val Fn_toInt32IdUnchecked(Cachet_ContextRef cx, Type_TypedId::Ref param_typedId) {
  return Type_Int32Id::Val(param_typedId.id());
}

Type_TypedId::Val Fn_fromBooleanIdUnchecked(Cachet_ContextRef cx, Type_BooleanId::Ref param_booleanId) {
  return TypedOperandIdW(TypedOperandId(param_booleanId), false);
}

Type_BooleanId::Val Fn_toBooleanIdUnchecked(Cachet_ContextRef cx, Type_TypedId::Ref param_typedId) {
  return Type_BooleanId::Val(param_typedId.id());
}

Type_TypedId::Val Fn_fromValueTagIdUnchecked(Cachet_ContextRef cx, Type_ValueTagId::Ref param_valueTagId) {
  return TypedOperandIdW(TypedOperandId(param_valueTagId), false);
}

Type_ValueTagId::Val Fn_toValueTagIdUnchecked(Cachet_ContextRef cx, Type_TypedId::Ref param_typedId) {
  return Type_ValueTagId::Val(param_typedId.id());
}

}

namespace Impl_GuardClassKind {

inline Type_GuardClassKind::Ref Variant_Array(Cachet_ContextRef cx) {
  return GuardClassKind::Array;
}

inline Type_GuardClassKind::Ref Variant_ArrayBuffer(Cachet_ContextRef cx) {
  return GuardClassKind::ArrayBuffer;
}

inline Type_GuardClassKind::Ref Variant_SharedArrayBuffer(Cachet_ContextRef cx) {
  return GuardClassKind::SharedArrayBuffer;
}

inline Type_GuardClassKind::Ref Variant_DataView(Cachet_ContextRef cx) {
  return GuardClassKind::DataView;
}

inline Type_GuardClassKind::Ref Variant_MappedArguments(Cachet_ContextRef cx) {
  return GuardClassKind::MappedArguments;
}

inline Type_GuardClassKind::Ref Variant_UnmappedArguments(Cachet_ContextRef cx) {
  return GuardClassKind::UnmappedArguments;
}

inline Type_GuardClassKind::Ref Variant_WindowProxy(Cachet_ContextRef cx) {
  return GuardClassKind::WindowProxy;
}

inline Type_GuardClassKind::Ref Variant_JSFunction(Cachet_ContextRef cx) {
  return GuardClassKind::JSFunction;
}

inline Type_GuardClassKind::Ref Variant_Set(Cachet_ContextRef cx) {
  return GuardClassKind::Set;
}

inline Type_GuardClassKind::Ref Variant_Map(Cachet_ContextRef cx) {
  return GuardClassKind::Map;
}

};  // namespace Impl_GuardClassKind

void Impl_CacheIR::Fn_addFailurePath(Cachet_ContextRef cx, IR_MASM::LabelMutRef failure) {
  FailurePath* failurePath;
  // TODO: use this result in some way
  cx.compiler->addFailurePath(&failurePath);
  failure = failurePath->label();
}

Type_JSValueType::Ref Impl_CacheIR::Fn_knownType(Cachet_ContextRef cx, Type_ValueId::Ref param_valueId) {
  return cx.compiler->allocator.knownType(param_valueId);
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

Type_Reg::Val Impl_CacheIR::Fn_defineInt32Reg(Cachet_ContextRef cx,
                                            Type_Int32Id::Ref param_int32Id) {
  return cx.compiler->allocator.defineRegister(cx.compiler->masm, param_int32Id);
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
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_objectId);
}

Type_Reg::Val Impl_CacheIR::Fn_useInt32Reg(Cachet_ContextRef cx,
                                            Type_Int32Id::Ref param_int32Id) {
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_int32Id);
}

Type_Reg::Val Impl_CacheIR::Fn_useValueTagReg(Cachet_ContextRef cx,
                                            Type_ValueTagId::Ref param_valueTagId) {
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_valueTagId);
}

void Impl_CacheIR::Fn_emitLoadInt32StubField(
    Cachet_ContextRef cx,
    Type_Int32Field::Ref param_int32Field,
    Type_Reg::Ref param_dstReg) {
  StubFieldOffset val(param_int32Field, StubField::Type::RawInt32);
  cx.compiler->emitLoadStubField(val, param_dstReg);
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

Type_Class::Val Impl_CacheIR::Fn_readClassField(
    Cachet_ContextRef cx,
    Type_ClassField::Ref param_classField) {
  return cx.compiler->classStubField(param_classField);
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
