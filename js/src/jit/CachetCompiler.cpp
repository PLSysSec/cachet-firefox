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

namespace Impl_MIRType {

inline Type_MIRType::Ref Variant_Undefined(Cachet_ContextRef cx) {
  return MIRType::Undefined;
}

inline Type_MIRType::Ref Variant_Null(Cachet_ContextRef cx) {
  return MIRType::Null;
}

inline Type_MIRType::Ref Variant_Boolean(Cachet_ContextRef cx) {
  return MIRType::Boolean;
}

inline Type_MIRType::Ref Variant_Int32(Cachet_ContextRef cx) {
  return MIRType::Int32;
}

inline Type_MIRType::Ref Variant_Int64(Cachet_ContextRef cx) {
  return MIRType::Int64;
}

inline Type_MIRType::Ref Variant_IntPtr(Cachet_ContextRef cx) {
  return MIRType::IntPtr;
}

inline Type_MIRType::Ref Variant_Double(Cachet_ContextRef cx) {
  return MIRType::Double;
}

inline Type_MIRType::Ref Variant_Float32(Cachet_ContextRef cx) {
  return MIRType::Float32;
}

inline Type_MIRType::Ref Variant_String(Cachet_ContextRef cx) {
  return MIRType::String;
}

inline Type_MIRType::Ref Variant_Symbol(Cachet_ContextRef cx) {
  return MIRType::Symbol;
}

inline Type_MIRType::Ref Variant_BigInt(Cachet_ContextRef cx) {
  return MIRType::BigInt;
}

inline Type_MIRType::Ref Variant_Simd128(Cachet_ContextRef cx) {
  return MIRType::Simd128;
}

inline Type_MIRType::Ref Variant_Object(Cachet_ContextRef cx) {
  return MIRType::Object;
}

inline Type_MIRType::Ref Variant_MagicOptimizedOut(Cachet_ContextRef cx) {
  return MIRType::MagicOptimizedOut;
}

inline Type_MIRType::Ref Variant_MagicHole(Cachet_ContextRef cx) {
  return MIRType::MagicHole;
}

inline Type_MIRType::Ref Variant_MagicIsConstructing(Cachet_ContextRef cx) {
  return MIRType::MagicIsConstructing;
}

inline Type_MIRType::Ref Variant_MagicUninitializedLexical(Cachet_ContextRef cx) {
  return MIRType::MagicUninitializedLexical;
}

inline Type_MIRType::Ref Variant_Value(Cachet_ContextRef cx) {
  return MIRType::Value;
}

inline Type_MIRType::Ref Variant_None(Cachet_ContextRef cx) {
  return MIRType::None;
}

inline Type_MIRType::Ref Variant_Slots(Cachet_ContextRef cx) {
  return MIRType::Slots;
}

inline Type_MIRType::Ref Variant_Elements(Cachet_ContextRef cx) {
  return MIRType::Elements;
}

inline Type_MIRType::Ref Variant_Pointer(Cachet_ContextRef cx) {
  return MIRType::Pointer;
}

inline Type_MIRType::Ref Variant_RefOrNull(Cachet_ContextRef cx) {
  return MIRType::RefOrNull;
}

inline Type_MIRType::Ref Variant_StackResults(Cachet_ContextRef cx) {
  return MIRType::StackResults;
}

inline Type_MIRType::Ref Variant_Shape(Cachet_ContextRef cx) {
  return MIRType::Shape;
}

};  // namespace Impl_MIRType

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

namespace Impl_JSOp {

inline Type_JSOp::Ref Variant_Eq(Cachet_ContextRef cx) {
  return JSOp::Eq;
}

inline Type_JSOp::Ref Variant_Ne(Cachet_ContextRef cx) {
  return JSOp::Ne;
}

inline Type_JSOp::Ref Variant_StrictEq(Cachet_ContextRef cx) {
  return JSOp::StrictEq;
}

inline Type_JSOp::Ref Variant_StrictNe(Cachet_ContextRef cx) {
  return JSOp::StrictNe;
}

inline Type_JSOp::Ref Variant_Lt(Cachet_ContextRef cx) {
  return JSOp::Lt;
}

inline Type_JSOp::Ref Variant_Gt(Cachet_ContextRef cx) {
  return JSOp::Gt;
}

inline Type_JSOp::Ref Variant_Le(Cachet_ContextRef cx) {
  return JSOp::Le;
}

inline Type_JSOp::Ref Variant_Ge(Cachet_ContextRef cx) {
  return JSOp::Ge;
}

};  // namespace Impl_JSOp

namespace Impl_Reg {

inline Type_Reg::Ref Variant_R11(Cachet_ContextRef cx) {
  return Register(X86Encoding::r11);
}

};  // namespace Impl_Reg

namespace Impl_PhyFloatReg {

inline Type_PhyFloatReg::Ref Variant_Xmm0(Cachet_ContextRef cx) {
  return X86Encoding::xmm0;
}

inline Type_PhyFloatReg::Ref Variant_Xmm15(Cachet_ContextRef cx) {
  return X86Encoding::xmm15;
}

};  // namespace Impl_PhyFloatReg

namespace Impl_FloatContentType {

inline Type_FloatContentType::Ref Variant_Single(Cachet_ContextRef cx) {
  return FloatRegisters::Single;
}

inline Type_FloatContentType::Ref Variant_Double(Cachet_ContextRef cx) {
  return FloatRegisters::Double;
}

inline Type_FloatContentType::Ref Variant_Simd128(Cachet_ContextRef cx) {
  return FloatRegisters::Simd128;
}

};  // namespace Impl_FloatContentType

namespace Impl_Condition {

inline Type_Condition::Ref Variant_Equal(Cachet_ContextRef cx) {
  return Assembler::Condition::Equal;
}

inline Type_Condition::Ref Variant_NotEqual(Cachet_ContextRef cx) {
  return Assembler::Condition::NotEqual;
}

inline Type_Condition::Ref Variant_Overflow(Cachet_ContextRef cx) {
  return Assembler::Condition::Overflow;
}

inline Type_Condition::Ref Variant_Zero(Cachet_ContextRef cx) {
  return Assembler::Condition::Zero;
}

inline Type_Condition::Ref Variant_NonZero(Cachet_ContextRef cx) {
  return Assembler::Condition::NonZero;
}

inline Type_Condition::Ref Variant_Signed(Cachet_ContextRef cx) {
  return Assembler::Condition::Signed;
}

inline Type_Condition::Ref Variant_NotSigned(Cachet_ContextRef cx) {
  return Assembler::Condition::NotSigned;
}

inline Type_Condition::Ref Variant_GreaterThan(Cachet_ContextRef cx) {
  return Assembler::Condition::GreaterThan;
}

inline Type_Condition::Ref Variant_LessThan(Cachet_ContextRef cx) {
  return Assembler::Condition::LessThan;
}

inline Type_Condition::Ref Variant_GreaterThanOrEqual(Cachet_ContextRef cx) {
  return Assembler::Condition::GreaterThanOrEqual;
}

inline Type_Condition::Ref Variant_LessThanOrEqual(Cachet_ContextRef cx) {
  return Assembler::Condition::LessThanOrEqual;
}

inline Type_Condition::Ref Variant_Above(Cachet_ContextRef cx) {
  return Assembler::Condition::Above;
}

inline Type_Condition::Ref Variant_Below(Cachet_ContextRef cx) {
  return Assembler::Condition::Below;
}

inline Type_Condition::Ref Variant_AboveOrEqual(Cachet_ContextRef cx) {
  return Assembler::Condition::AboveOrEqual;
}

inline Type_Condition::Ref Variant_BelowOrEqual(Cachet_ContextRef cx) {
  return Assembler::Condition::BelowOrEqual;
}

};  // namespace Impl_Condition

namespace Impl_Scale {

inline Type_Scale::Ref Variant_TimesOne(Cachet_ContextRef cx) {
  return TimesOne;
}

inline Type_Scale::Ref Variant_TimesTwo(Cachet_ContextRef cx) {
  return TimesTwo;
}

inline Type_Scale::Ref Variant_TimesFour(Cachet_ContextRef cx) {
  return TimesFour;
}

inline Type_Scale::Ref Variant_TimesEight(Cachet_ContextRef cx) {
  return TimesEight;
}

};  // namespace Impl_Scale

namespace Impl_OperandLocationKind {

inline Type_OperandLocationKind::Ref Variant_Uninitialized(Cachet_ContextRef cx) {
  return OperandLocationKind::Uninitialized;
}

inline Type_OperandLocationKind::Ref Variant_ValueReg(Cachet_ContextRef cx) {
  return OperandLocationKind::ValueReg;
}

inline Type_OperandLocationKind::Ref Variant_PayloadReg(Cachet_ContextRef cx) {
  return OperandLocationKind::PayloadReg;
}

inline Type_OperandLocationKind::Ref Variant_FloatReg(Cachet_ContextRef cx) {
  return OperandLocationKind::FloatReg;
}

};  // namespace Impl_OperandLocationKind

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

// TODO: these definitions are not used right now and are only there
// to satisfy the linker. Need to be checked to make sure they are correct.
namespace Impl_Float64 {

Type_Bool::Val Fn_isNegativeZero(Cachet_ContextRef cx, Type_Float64::Ref param_float64) {
  return std::abs(param_float64) == 0.0 && std::signbit(param_float64);
}

Type_Bool::Val Fn_isNaN(Cachet_ContextRef cx, Type_Float64::Ref param_float64) {
  return std::isnan(param_float64);
}

Type_Bool::Val Fn_isSafeInt32Unchecked(Cachet_ContextRef cx, Type_Float64::Ref param_float64) {
  (double)(int32_t)param_float64 == param_float64;
}

Type_Float64::Val Fn_fromInt32Unchecked(Cachet_ContextRef cx, Type_Int32::Ref param_int32) {
  return (double)param_int32;
}

Type_Int32::Val Fn_toInt32Unchecked(Cachet_ContextRef cx, Type_Float64::Ref param_float64) {
  return (int32_t)param_float64;
}

};  // namespace Impl_Float64

namespace Impl_Value {

Type_ValueType::Val Fn_typeOf(Cachet_ContextRef cx,
                                          Type_Value::Ref param_value) {
  return param_value.type();
}

Type_Value::Val Fn_fromDoubleUnchecked(Cachet_ContextRef cx, Type_Double::Ref param_value) {
  return DoubleValue(param_value);
}

Type_Double::Val Fn_toDoubleUnchecked(Cachet_ContextRef cx, Type_Value::Ref param_value) {
  return param_value.toDouble();
}

Type_Value::Val Fn_fromInt32Unchecked(Cachet_ContextRef cx, Type_Int32::Ref param_value) {
  return Int32Value(param_value);
}

Type_Int32::Val Fn_toInt32Unchecked(Cachet_ContextRef cx, Type_Value::Ref param_value) {
  return param_value.toInt32();
}

Type_Value::Val Fn_fromBoolUnchecked(Cachet_ContextRef cx, Type_Bool::Ref param_value) {
  return BooleanValue(param_value);
}

Type_Bool::Val Fn_toBoolUnchecked(Cachet_ContextRef cx, Type_Value::Ref param_value) {
  return param_value.toBoolean();
}

Type_Value::Val Fn_getUndefinedUnchecked(Cachet_ContextRef cx) {
  return JS::UndefinedValue();
}

Type_Value::Val Fn_fromObjectUnchecked(Cachet_ContextRef cx,
                                                   Type_Object::Ref param_value) {
  return ObjectValue(*param_value);
}

Type_Object::Val Fn_toObjectUnchecked(Cachet_ContextRef cx,
                                                  Type_Value::Ref param_value) {
  return &param_value.toObject();
}

Type_Value::Val Fn_fromStringUnchecked(Cachet_ContextRef cx,
                                                   Type_String::Ref param_value) {
  return StringValue(param_value);
}

Type_String::Val Fn_toStringUnchecked(Cachet_ContextRef cx,
                                                  Type_Value::Ref param_value) {
  return param_value.toString();
}

Type_Value::Val Fn_fromSymbolUnchecked(Cachet_ContextRef cx,
                                                   Type_Symbol::Ref param_value) {
  return SymbolValue(param_value);
}

Type_Symbol::Val Fn_toSymbolUnchecked(Cachet_ContextRef cx,
                                                  Type_Value::Ref param_value) {
  return param_value.toSymbol();
}

Type_Value::Val Fn_fromBigIntUnchecked(Cachet_ContextRef cx,
                                                   Type_BigInt::Ref param_value) {
  return BigIntValue(param_value);
}

Type_BigInt::Val Fn_toBigIntUnchecked(Cachet_ContextRef cx,
                                                  Type_Value::Ref param_value) {
  return param_value.toBigInt();
}

};  // namespace Impl_Value

namespace Impl_Object {

Type_NativeObject::Val To_NativeObject(Type_Object::Val param_in) {
  return &param_in->as<NativeObject>();
}

Type_NativeObject::Ref To_NativeObject(Type_Object::Ref param_in) {
  return param_in.as<NativeObject>();
}

Type_Shape::Val Impl_Object::Fn_shapeOfUnchecked(Cachet_ContextRef cx,
                                                 Type_Heap::Ref param_heap,
                                                 Type_Object::Ref param_object) {
  return param_object->shape();
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

Type_ArgumentsObject::Ref To_ArgumentsObject(Type_NativeObject::Ref param_in) {
  param_in->as<ArgumentsObject>();
}

Type_Value::Val Fn_getFixedSlotUnchecked(Cachet_ContextRef cx,
                                         Type_Heap::Ref param_heap,
                                         Type_NativeObject::Ref param_nativeObject,
                                         Type_UInt32::Ref param_slot) {
  return param_nativeObject->getFixedSlot(param_slot);
}

Type_NativeObjectSlots::Val Fn_getSlotsUnchecked(Cachet_ContextRef cx,
                                                 Type_Heap::Ref param_heap,
                                                 Type_NativeObject::Ref param_nativeObject) {
  return param_nativeObject->getSlotsHeader()->slots();
}

Type_NativeObjectElements::Val Fn_getElementsHeaderUnchecked(Cachet_ContextRef cx,
                                                             Type_Heap::Ref param_heap,
                                                             Type_NativeObject::Ref param_nativeObject) {
  return param_nativeObject->getElementsHeader()->elements();
}

};  // namespace Impl_NativeObject

namespace Impl_NativeObjectSlots {

// TODO(abhishekc-sharma): need to rethink how to implement this
Type_UInt32::Val Fn_length(Cachet_ContextRef cx, Type_NativeObjectSlots::Ref param_slots) {
  return 0;
}

Type_Value::Val Fn_getDynamicSlotUnchecked(Cachet_ContextRef cx,
                                           Type_Heap::Ref param_heap,
                                           Type_NativeObjectSlots::Ref param_slots,
                                           Type_UInt32::Ref param_slot) {
  return param_slots[param_slot];
}

};  // namespace Impl_NativeObjectSlots

namespace Impl_NativeObjectElements {

Type_UInt32::Val Fn_getLengthUnchecked(Cachet_ContextRef cx,
                                       Type_Heap::Ref param_heap,
                                       Type_NativeObjectElements::Ref param_elements) {
  return *(Type_UInt32::Val *)((char *)param_elements + Impl_NativeObjectElements::Var_offsetOfLength(cx));
}

Type_UInt32::Val Fn_getInitializedLengthUnchecked(Cachet_ContextRef cx,
                                                  Type_Heap::Ref param_heap,
                                                  Type_NativeObjectElements::Ref param_elements) {
  return *(Type_UInt32::Val *)((char *)param_elements + Impl_NativeObjectElements::Var_offsetOfInitializedLength(cx));
}

Type_Value::Val Fn_getElementUnchecked(Cachet_ContextRef cx,
                                       Type_Heap::Ref param_heap,
                                       Type_NativeObjectElements::Ref param_elements,
                                       Type_UInt64::Ref param_index) {
  return param_elements[param_index];
}

};  // namespace Impl_NativeObjectElements

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

namespace Impl_ArgumentsObject {

Type_UInt32::Val Fn_getInitialLengthSlotUnchecked(Cachet_ContextRef cx, Type_Heap::Ref param_heap, Type_ArgumentsObject::Ref param_obj) {
  return uint32_t(param_obj->getFixedSlot(ArgumentsObject::INITIAL_LENGTH_SLOT).toInt32()); 
}

Type_ArgumentsData::Val Fn_getArgumentsDataUnchecked(Cachet_ContextRef cx, Type_Heap::Ref param_heap, Type_ArgumentsObject::Ref param_obj) {
  return reinterpret_cast<ArgumentsData*>(
      param_obj->getFixedSlot(ArgumentsObject::DATA_SLOT).toPrivate()
  );
}

};  // namespace Impl_ArgumentsObject

namespace Impl_ArgumentsData {

Type_UInt32::Ref Field_numArgs(Cachet_ContextRef cx, Type_ArgumentsData::Ref in) {
  return in->numArgs;
}

Type_Value::Val Fn_getArgUnchecked(Cachet_ContextRef cx, Type_Heap::Ref param_heap, Type_ArgumentsData::Ref param_argsData, Type_UInt32::Ref param_index) {
  return param_argsData->args[param_index];
}

};  // namespace Impl_ArgumentsData

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

namespace Impl_BaseShape {

Type_Class::Val Fn_classOf(Cachet_ContextRef cx, Type_BaseShape::Ref param_baseShape) {
  return param_baseShape->clasp();
}

Type_TaggedProto::Val Fn_protoOfUnchecked(Cachet_ContextRef cx, Type_Heap::Ref param_heap, Type_BaseShape::Ref param_baseShape) {
  return param_baseShape->proto();
}

};  // namespace Impl_BaseShape

namespace Impl_Shape {

Type_BaseShape::Val Fn_baseShapeOf(Cachet_ContextRef cx, Type_Shape::Ref param_shape) {
  return param_shape->base();
}

Type_UInt32::Val Fn_numFixedSlots(Cachet_ContextRef cx, Type_Shape::Ref param_shape) {
  return param_shape->numFixedSlots();
}

Type_UInt32::Val Fn_slotSpan(Cachet_ContextRef cx, Type_Shape::Ref param_shape) {
  return param_shape->slotSpan();
}

}; // namespace Impl_Shape

namespace Impl_Class {

Type_Class::Val Fn_windowProxyClass(Cachet_ContextRef cx) {
  return cx.js_ctx->runtime()->maybeWindowProxyClass();
}

Type_Bool::Val Fn_emulatesUndefined(Cachet_ContextRef cx, Type_Class::Ref param_class) {
  return param_class->emulatesUndefined();
}

Type_Bool::Val Fn_isNativeObject(Cachet_ContextRef cx, Type_Class::Ref param_class) {
  return param_class->isNativeObject();
}

Type_Bool::Val Fn_isProxyObject(Cachet_ContextRef cx, Type_Class::Ref param_class) {
  return param_class->isProxyObject();
}

};  // namespace Impl_Class

namespace Impl_String {

Type_Bool::Val Fn_isAtom(Cachet_ContextRef cx, Type_String::Ref param_string) {
  return param_string->isAtom();
}

//FIXME(abhishekc-sharma): returns a pointer to a local variable
Type_Atom::Val Fn_asAtom(Cachet_ContextRef cx, Type_String::Ref param_string) {
  return &param_string->asAtom();
}

Type_UInt64::Val Fn_length(Cachet_ContextRef cx, Type_String::Ref param_string) {
  return param_string->length();
}

};  // namespace Impl_String

namespace Impl_TaggedProto {

Type_Bool::Val Fn_isNull(Cachet_ContextRef cx, Type_TaggedProto::Ref param_proto) {
  return !param_proto.raw();
}

Type_Bool::Val Fn_isLazy(Cachet_ContextRef cx, Type_TaggedProto::Ref param_proto) {
  return param_proto.isDynamic();
}

Type_Bool::Val Fn_isObject(Cachet_ContextRef cx, Type_TaggedProto::Ref param_proto) {
  return param_proto.isObject();
}

Type_Object::Val Fn_toObjectUnchecked(Cachet_ContextRef cx, Type_TaggedProto::Ref param_proto) {
  return param_proto.toObject();
}

};  // namespace Impl_TaggedProto

namespace Impl_FloatReg {

Type_PhyFloatReg::Ref Field_reg(Cachet_ContextRef cx, Type_FloatReg::Ref in) {
  return in.encoding();
}

Type_FloatContentType::Ref Field_type(Cachet_ContextRef cx, Type_FloatReg::Ref in) {
  if(in.isSingle()) {
    return Impl_FloatContentType::Variant_Single(cx);
  } else if(in.isDouble()) {
    return Impl_FloatContentType::Variant_Double(cx);
  } else {
    MOZ_ASSERT(in.isSimd128());
    return Impl_FloatContentType::Variant_Simd128(cx);
  }
}

Type_FloatReg::Val Fn_newUnchecked(Cachet_ContextRef cx, Type_PhyFloatReg::Ref param_reg, Type_FloatContentType::Ref param_type) {
  return FloatRegister(param_reg, param_type);
}

};  // namespace Impl_FloatReg

namespace Impl_AnyReg {

Type_Bool::Val Fn_isFloat(Cachet_ContextRef cx, Type_AnyReg::Ref param_anyReg) {
  return param_anyReg.isFloat();
}

Type_AnyReg::Val Fn_fromRegUnchecked(Cachet_ContextRef cx, Type_Reg::Ref param_reg) {
  return AnyRegister(param_reg);
}

Type_Reg::Val Fn_toRegUnchecked(Cachet_ContextRef cx, Type_AnyReg::Ref param_anyReg) {
  return param_anyReg.gpr();
}

Type_AnyReg::Val Fn_fromFloatRegUnchecked(Cachet_ContextRef cx, Type_FloatReg::Ref param_floatReg) {
  return AnyRegister(param_floatReg);
}

Type_FloatReg::Val Fn_toFloatRegUnchecked(Cachet_ContextRef cx, Type_AnyReg::Ref param_anyReg) {
  return param_anyReg.fpu();
}

};  // namespace Impl_AnyReg

namespace Impl_TypedOrValueReg {

Type_MIRType::Val Fn_type(Cachet_ContextRef cx, Type_TypedOrValueReg::Ref param_reg) {
  return param_reg.type();
}

Type_TypedOrValueReg::Val Fn_fromValueRegUnchecked(Cachet_ContextRef cx, Type_ValueReg::Ref param_valueReg) {
  return TypedOrValueRegister(param_valueReg);
}

Type_ValueReg::Val Fn_toValueRegUnchecked(Cachet_ContextRef cx, Type_TypedOrValueReg::Ref param_reg) {
  return param_reg.valueReg();
}

Type_TypedOrValueReg::Val Fn_fromTypedRegUnchecked(Cachet_ContextRef cx, Type_MIRType::Ref param_type, Type_AnyReg::Ref param_reg) {
  return TypedOrValueRegister(param_type, param_reg);
}

Type_AnyReg::Val Fn_toTypedRegUnchecked(Cachet_ContextRef cx, Type_TypedOrValueReg::Ref param_reg) {
  return param_reg.typedReg();
}

};  // namespace Impl_TypedOrValueReg

namespace Impl_GeneralRegSet {

Type_GeneralRegSet::Val Fn_empty(Cachet_ContextRef cx) {
  return TypedRegisterSet<Register>();
}

Type_GeneralRegSet::Val Fn_volatile(Cachet_ContextRef cx) {
  return TypedRegisterSet<Register>::Volatile();
}

Type_GeneralRegSet::Val Fn_intersect(Cachet_ContextRef cx, Type_GeneralRegSet::Ref param_lhs, Type_GeneralRegSet::Ref param_rhs) {
  return TypedRegisterSet<Register>::Intersect(param_lhs, param_rhs);
}

Type_GeneralRegSet::Val Fn_difference(Cachet_ContextRef cx, Type_GeneralRegSet::Ref param_lhs, Type_GeneralRegSet::Ref param_rhs) {
  return TypedRegisterSet<Register>::Subtract(param_lhs, param_rhs);
}

Type_Bool::Val Fn_isEmpty(Cachet_ContextRef cx, Type_GeneralRegSet::Ref param_set) {
  return param_set.empty();
}

Type_Bool::Val Fn_contains(Cachet_ContextRef cx, Type_GeneralRegSet::Ref param_set, Type_Reg::Ref param_reg) {
  return param_set.hasRegisterIndex(param_reg);
}

void Fn_add(Cachet_ContextRef cx, Type_GeneralRegSet::MutRef param_newSet, Type_GeneralRegSet::Ref param_set, Type_Reg::Ref param_reg) {
  param_newSet.addRegisterIndex(param_reg);  
}

void Fn_take(Cachet_ContextRef cx, Type_GeneralRegSet::MutRef param_newSet, Type_GeneralRegSet::Ref param_set, Type_Reg::Ref param_reg) {
  param_newSet.takeRegisterIndex(param_reg);
}

};  // namespace Impl_GeneralRegSet

namespace Impl_FloatRegSet {

Type_FloatRegSet::Val Fn_empty(Cachet_ContextRef cx) {
  return TypedRegisterSet<FloatRegister>();
}

Type_FloatRegSet::Val Fn_volatile(Cachet_ContextRef cx) {
  return TypedRegisterSet<FloatRegister>::Volatile();
}

Type_FloatRegSet::Val Fn_intersect(Cachet_ContextRef cx, Type_FloatRegSet::Ref param_lhs, Type_FloatRegSet::Ref param_rhs) {
  return TypedRegisterSet<FloatRegister>::Intersect(param_lhs, param_rhs);
}

Type_FloatRegSet::Val Fn_difference(Cachet_ContextRef cx, Type_FloatRegSet::Ref param_lhs, Type_FloatRegSet::Ref param_rhs) {
  return TypedRegisterSet<FloatRegister>::Subtract(param_lhs, param_rhs);
}

Type_Bool::Val Fn_contains(Cachet_ContextRef cx, Type_FloatRegSet::Ref param_set, Type_FloatReg::Ref param_floatReg) {
  return param_set.hasRegisterIndex(param_floatReg);
}

void Fn_add(Cachet_ContextRef cx, Type_FloatRegSet::MutRef param_newSet, Type_FloatRegSet::Ref param_set, Type_FloatReg::Ref param_floatReg) {
  param_newSet.addRegisterIndex(param_floatReg);
}

void Fn_take(Cachet_ContextRef cx, Type_FloatRegSet::MutRef param_newSet, Type_FloatRegSet::Ref param_set, Type_FloatReg::Ref param_floatReg) {
  param_newSet.takeRegisterIndex(param_floatReg);
}

Type_Bool::Val Fn_isEmpty(Cachet_ContextRef cx, Type_FloatRegSet::Ref param_set) {
  return param_set.empty();
}

Type_FloatRegSet::Val Fn_reducedForPush(Cachet_ContextRef cx, Type_FloatRegSet::Ref param_set) {
  return param_set.reduceSetForPush();
}

};  // namespace Impl_FloatRegSet

namespace Impl_LiveRegSet {

Type_GeneralRegSet::Val Fn_gprs(Cachet_ContextRef cx, Type_LiveRegSet::Ref param_set) {
  return param_set.gprs();
}

Type_FloatRegSet::Val Fn_fpus(Cachet_ContextRef cx, Type_LiveRegSet::Ref param_set) {
  return param_set.fpus();
}

Type_LiveRegSet::Val Fn_newUnchecked(Cachet_ContextRef cx, Type_GeneralRegSet::Ref param_gprs, Type_FloatRegSet::Ref param_fpus) {
  return LiveRegisterSet(param_gprs, param_fpus);  
}

};  // namespace Impl_LiveRegSet

namespace Impl_Address {

Type_Reg::Ref Field_base(Cachet_ContextRef cx, Type_Address::Ref in) {
  return in.base;
}

Type_Int32::Ref Field_offset(Cachet_ContextRef cx, Type_Address::Ref in) {
  return in.offset;
}

Type_Address::Val Fn_newUnchecked(Cachet_ContextRef cx, Type_Reg::Ref param_base, Type_Int32::Ref param_offset) {
  return Address(param_base, param_offset);
}

};

namespace Impl_BaseIndex {

Type_Reg::Ref Field_base(Cachet_ContextRef cx, Type_BaseIndex::Ref in) {
  return in.base;
}

Type_Reg::Ref Field_index(Cachet_ContextRef cx, Type_BaseIndex::Ref in) {
  return in.index;
}

Type_Scale::Ref Field_scale(Cachet_ContextRef cx, Type_BaseIndex::Ref in) {
  return in.scale;
}

Type_UInt32::Ref Field_offset(Cachet_ContextRef cx, Type_BaseIndex::Ref in) {
  return in.offset;
}

Type_BaseIndex::Val Fn_newUnchecked(Cachet_ContextRef cx, Type_Reg::Ref param_base, Type_Reg::Ref param_index, Type_Scale::Ref param_scale, Type_UInt32::Ref param_offset) {
  return BaseIndex(param_base, param_index, param_scale, param_offset);
}

};  // namespace Impl_BaseIndex

namespace Impl_BaseValueIndex {

Type_BaseIndex::Ref Field_inner(Cachet_ContextRef cx, Type_BaseValueIndex::Ref in) {
  return static_cast<BaseIndex>(in);
}

Type_BaseValueIndex::Val Fn_newUnchecked(Cachet_ContextRef cx, Type_BaseIndex::Ref param_inner) {
  return BaseValueIndex(param_inner.base, param_inner.index, param_inner.offset);
}

};  // namespace Impl_BaseValueIndex

namespace Impl_BaseObjectElementIndex {

Type_BaseValueIndex::Ref Field_inner(Cachet_ContextRef cx, Type_BaseObjectElementIndex::Ref in) {
  return static_cast<BaseValueIndex>(in);
}

Type_BaseObjectElementIndex::Val Fn_newUnchecked(Cachet_ContextRef cx, Type_BaseValueIndex::Ref param_inner) {
  return BaseObjectElementIndex(param_inner.base, param_inner.index, param_inner.offset);
}

};  // namespace Impl_BaseObjectElementIndex

namespace Impl_BaseObjectSlotIndex {

Type_BaseValueIndex::Ref Field_inner(Cachet_ContextRef cx, Type_BaseObjectSlotIndex::Ref in) {
  return static_cast<BaseValueIndex>(in);
}

Type_BaseObjectSlotIndex::Val Fn_newUnchecked(Cachet_ContextRef cx, Type_BaseValueIndex::Ref param_inner) {
  return BaseObjectSlotIndex(param_inner.base, param_inner.index);
}

};  // namespace Impl_BaseObjectSlotIndex

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

void Fn_setFloatRegUnchecked(Cachet_ContextRef cx, Type_OperandLocation::MutRef param_loc, Type_FloatReg::Ref param_floatReg) {
  param_loc.setDoubleReg(param_floatReg);
}

Type_FloatReg::Val Fn_getFloatRegUnchecked(Cachet_ContextRef cx, Type_OperandLocation::Ref param_loc) {
  return param_loc.doubleReg();
}

}

namespace Impl_OperandId {

Type_UInt16::Val Fn_id(Cachet_ContextRef cx, Type_OperandId::Ref param_operandId) {
  param_operandId.id();
}

Type_OperandId::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return OperandId(param_id);
}

};  // namespace Impl_OperandId

namespace Impl_ValueId {

inline Type_OperandId::Val To_OperandId(Type_ValueId::Val in) {
  return static_cast<OperandId>(in);
}

Type_ValueId::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return ValOperandId(param_id);
}

};  // namespace Impl_ValueId

namespace Impl_ObjectId {

inline Type_OperandId::Val To_OperandId(Type_ObjectId::Val in) {
  return static_cast<OperandId>(in);
}

Type_ObjectId::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return ObjOperandId(param_id);
}

};  // namespace Impl_ObjectId

namespace Impl_StringId {

inline Type_OperandId::Val To_OperandId(Type_StringId::Val in) {
  return static_cast<OperandId>(in);
}

Type_StringId::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return StringOperandId(param_id);
}

};  // namespace Impl_StringId

namespace Impl_SymbolId {

inline Type_OperandId::Val To_OperandId(Type_SymbolId::Val in) {
  return static_cast<OperandId>(in);
}

Type_SymbolId::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return SymbolOperandId(param_id);
}

};  // namespace Impl_SymbolId

namespace Impl_BoolId {

inline Type_OperandId::Val To_OperandId(Type_BoolId::Val in) {
  return static_cast<OperandId>(in);
}

Type_BoolId::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return BooleanOperandId(param_id);
}

};  // namespace Impl_BoolId

namespace Impl_Int32Id {

inline Type_OperandId::Val To_OperandId(Type_Int32Id::Val in) {
  return static_cast<OperandId>(in);
}

Type_Int32Id::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return Int32OperandId(param_id);
}

};  // namespace Impl_Int32Id

namespace Impl_NumberId {

inline Type_ValueId::Val To_ValueId(Type_NumberId::Val in) {
  return static_cast<ValOperandId>(in);
}

Type_NumberId::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return NumberOperandId(param_id);
}

};  // namespace Impl_NumberId

namespace Impl_BigIntId {

inline Type_OperandId::Val To_OperandId(Type_BigIntId::Val in) {
  return static_cast<OperandId>(in);
}

Type_BigIntId::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return BigIntOperandId(param_id);
}

};  // namespace Impl_BigIntId

namespace Impl_ValueTagId {

inline Type_OperandId::Val To_OperandId(Type_ValueTagId::Val in) {
  return static_cast<OperandId>(in);
}

Type_ValueTagId::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return ValueTagOperandId(param_id);
}

};  // namespace Impl_ValueTagId

namespace Impl_IntPtrId {

inline Type_OperandId::Val To_OperandId(Type_IntPtrId::Val in) {
  return static_cast<OperandId>(in);
}

Type_IntPtrId::Val Fn_fromIdUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id) {
  return IntPtrOperandId(param_id);
}

};  // namespace Impl_IntPtrId

namespace Impl_TypedId {

inline Type_OperandId::Val To_OperandId(Type_TypedId::Val in) {
  return static_cast<OperandId>(in);
}

Type_JSValueType::Val Fn_type(Cachet_ContextRef cx, Type_TypedId::Ref param_typedId) {
  return param_typedId.type();
}

Type_TypedId::Val Fn_newUnchecked(Cachet_ContextRef cx, Type_UInt16::Ref param_id, Type_JSValueType::Ref param_type) {
  return TypedOperandId(ValOperandId(param_id), param_type);
}

};  // namespace Impl_TypedId

namespace Impl_FailurePath {

IR_MASM::LabelRef Field_label_(Cachet_ContextRef cx, Type_FailurePath::Ref in) {
  return in->label();
}

};  // namespace Impl_FailurePath

namespace Impl_MASM {

void EmitOp_AssumeUnreachable(Cachet_ContextRef cx, IR_MASM::OpsRef ops) {
  ops.assumeUnreachable("Cachet unreachable");
}

void EmitOp_Mov(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                      Type_Reg::Ref param_srcReg, Type_Reg::Ref param_dstReg) {
  ops.mov(param_srcReg, param_dstReg);
}

void EmitOp_MovData(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                    Type_Reg::Ref param_srcReg,
                    Type_Reg::Ref param_dstReg) {
  ops.mov(param_srcReg, param_dstReg);
}

void EmitOp_MoveValue(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                        Type_ValueReg::Ref param_srcReg, Type_ValueReg::Ref param_dstReg) {
  ops.moveValue(param_srcReg, param_dstReg);
}

void EmitOp_MovePtrBoolImmWord(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                Type_Bool::Ref param_b, Type_Reg::Ref param_dstReg) {
  ops.movePtr(ImmWord(param_b), param_dstReg);
}

void EmitOp_MoveValueImm(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                          Type_Value::Ref param_value, Type_ValueReg::Ref param_dstReg) {
  ops.moveValue(param_value, param_dstReg);
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

void EmitOp_Load32Address(Cachet_ContextRef cx,
                          IR_MASM::OpsRef ops,
                          Type_Address::Ref param_address,
                          Type_Reg::Ref param_dstReg) {
  ops.load32(param_address, param_dstReg);
}

void EmitOp_LoadTypedOrValueAddress(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Address::Ref param_address,
                                    Type_TypedOrValueReg::Ref param_dstReg) {
  ops.loadTypedOrValue(param_address, param_dstReg);
}

void EmitOp_LoadTypedOrValueBaseObjectElementIndex(Cachet_ContextRef cx,
                                                   IR_MASM::OpsRef ops,
                                                   Type_BaseObjectElementIndex::Ref param_index,
                                                   Type_TypedOrValueReg::Ref param_dstReg) {
  ops.loadTypedOrValue(param_index, param_dstReg);
}

void EmitOp_LoadValueBaseObjectElementIndex(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                            Type_BaseObjectElementIndex::Ref param_index,
                                            Type_ValueReg::Ref param_dstReg) {
  ops.loadValue(param_index, param_dstReg);
}

void EmitOp_LoadPtrAddress(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                           Type_Address::Ref param_address,
                           Type_Reg::Ref param_dstReg) {
  ops.loadPtr(param_address, param_dstReg);
}

void EmitOp_GuardSpecificAtom(Cachet_ContextRef cx,
                              IR_MASM::OpsRef ops,
                              Type_Reg::Ref param_strReg,
                              Type_Atom::Ref param_atom,
                              Type_Reg::Ref param_scratchReg,
                              Type_LiveRegSet::Ref param_volatileRegs,
                              IR_MASM::LabelRef param_fail) {
  ops.guardSpecificAtom(param_strReg, param_atom, param_scratchReg, param_volatileRegs, param_fail);
}

void EmitOp_TagValue(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_JSValueType::Ref param_valTy,
                      Type_Reg::Ref param_payload, Type_ValueReg::Ref param_dest) {
  ops.tagValue(param_valTy, param_payload, param_dest);
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

void EmitOp_UnboxObjectProto(Cachet_ContextRef cx,
                             IR_MASM::OpsRef ops,
                             Type_Reg::Ref param_srcReg,
                             Type_Reg::Ref param_destReg) {
  // Phantom op
}

void EmitOp_FallibleUnboxObject(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                Type_ValueReg::Ref param_valueReg,
                                Type_Reg::Ref param_objectReg,
                                IR_MASM::LabelRef param_failure) {
  ops.fallibleUnboxObject(param_valueReg, param_objectReg, param_failure);
}

void EmitOp_FallibleUnboxBoolean(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                        Type_ValueReg::Ref param_valueReg,
                                        Type_Reg::Ref param_boolReg,
                                        IR_MASM::LabelRef param_failure) {
  ops.fallibleUnboxBoolean(param_valueReg, param_boolReg, param_failure);
}

void EmitOp_CastBoolToInt32(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_Reg::Ref param_int32Reg) {
  // Phantom op
}

void EmitOp_ConvertInt32ValueToDouble(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                        Type_ValueReg::Ref param_valueReg) {
  ops.convertInt32ValueToDouble(param_valueReg);
}

void EmitOp_SpectreBoundsCheck32Address(Cachet_ContextRef cx,
                                        IR_MASM::OpsRef ops,
                                        Type_Reg::Ref param_indexReg,
                                        Type_Address::Ref param_length,
                                        Type_Reg::Ref param_maybeScratch,
                                        IR_MASM::LabelRef param_failure) {
  ops.spectreBoundsCheck32(param_indexReg, param_length, param_maybeScratch, param_failure);
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

void EmitOp_BranchObject(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                         Type_Condition::Ref param_condition,
                         Type_Reg::Ref param_lhsReg,
                         Type_Reg::Ref param_rhsReg,
                         IR_MASM::LabelRef param_branch) {
  ops.branchPtr(param_condition, param_lhsReg, param_rhsReg, param_branch);
}

void EmitOp_ConvertInt32ToDouble(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                 Type_Reg::Ref param_srcReg,
                                 Type_FloatReg::Ref param_destReg) {
  ops.convertInt32ToDouble(param_srcReg, param_destReg);
}

void EmitOp_BranchTestInt32(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_condition,
                                    Type_ValueReg::Ref param_valueReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestInt32(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestInt32Truthy(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Bool::Ref param_truthy,
                                    Type_ValueReg::Ref param_valueReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestInt32Truthy(param_truthy, param_valueReg, param_branch);
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

void EmitOp_BranchTestObject(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_condition,
                                    Type_ValueReg::Ref param_valueReg,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestObject(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestObjectTag(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                Type_Condition::Ref param_condition,
                                Type_Reg::Ref param_tagReg,
                                IR_MASM::LabelRef param_branch) {
  ops.branchTestObject(param_condition, param_tagReg, param_branch);
}

void EmitOp_BranchTestValue(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                    Type_Condition::Ref param_cond,
                                    Type_BaseIndex::Ref param_lhs,
                                    Type_ValueReg::Ref param_rhs,
                                    IR_MASM::LabelRef param_branch) {
  ops.branchTestValue(param_cond, param_lhs, param_rhs, param_branch);
}

void EmitOp_BranchTestNullProto(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                Type_Reg::Ref param_reg,
                                IR_MASM::LabelRef param_branch) {
  ops.branchTestPtr(Assembler::Zero, param_reg, param_reg, param_branch);
}

void EmitOp_BranchTestLazyProto(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                Type_Reg::Ref param_reg,
                                IR_MASM::LabelRef param_branch) {
  ops.branchPtr(Assembler::Equal, param_reg, ImmWord(1), param_branch);
}

void EmitOp_BranchTestNull(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_Condition::Ref param_condition, Type_ValueReg::Ref param_valueReg, IR_MASM::LabelRef param_branch) {
  ops.branchTestNull(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestNullTag(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_Condition::Ref param_condition, Type_Reg::Ref param_tagReg, IR_MASM::LabelRef param_branch) {
  ops.branchTestNull(param_condition, param_tagReg, param_branch);
}

void EmitOp_TestNullSet(Cachet_ContextRef cx, IR_MASM::OpsRef ops, Type_Condition::Ref param_condition, Type_ValueReg::Ref param_valueReg, Type_Reg::Ref param_destReg) {
  ops.testNullSet(param_condition, param_valueReg, param_destReg);
}

void EmitOp_BranchTestUndefined(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                Type_Condition::Ref param_condition,
                                Type_ValueReg::Ref param_valueReg,
                                IR_MASM::LabelRef param_branch) {
  ops.branchTestUndefined(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestUndefinedTag(Cachet_ContextRef cx,
                                   IR_MASM::OpsRef ops,
                                   Type_Condition::Ref param_condition,
                                   Type_Reg::Ref param_tagReg,
                                   IR_MASM::LabelRef param_branch) {
  ops.branchTestUndefined(param_condition, param_tagReg, param_branch);
}

void EmitOp_BranchIfObjectEmulatesUndefined(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                            Type_Reg::Ref param_objReg,
                                            Type_Reg::Ref param_scratchReg,
                                            IR_MASM::LabelRef param_slowCheck,
                                            IR_MASM::LabelRef param_branch) {
  ops.branchIfObjectEmulatesUndefined(param_objReg, param_scratchReg, param_slowCheck, param_branch);
}

void EmitOp_TestUndefinedSet(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                             Type_Condition::Ref param_condition,
                             Type_ValueReg::Ref param_valueReg,
                             Type_Reg::Ref param_destReg) {
  ops.testUndefinedSet(param_condition, param_valueReg, param_destReg);
}

void EmitOp_BranchTestMagic(Cachet_ContextRef cx,
                            IR_MASM::OpsRef ops,
                            Type_Condition::Ref param_condition,
                            Type_ValueReg::Ref param_valueReg,
                            IR_MASM::LabelRef param_branch) {
  ops.branchTestMagic(param_condition, param_valueReg, param_branch);
}

void EmitOp_BranchTestMagicBaseObjectElementIndex(Cachet_ContextRef cx,
                                                  IR_MASM::OpsRef ops,
                                                  Type_Condition::Ref param_condition,
                                                  Type_BaseObjectElementIndex::Ref param_index,
                                                  IR_MASM::LabelRef param_branch) {
  ops.branchTestMagic(param_condition, param_index, param_branch);
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

void EmitOp_Branch32(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                     Type_Condition::Ref param_condition,
                     Type_Reg::Ref param_lhsReg,
                     Type_Reg::Ref param_rhsReg,
                     IR_MASM::LabelRef param_branch) {
  ops.branch32(param_condition, param_lhsReg, param_rhsReg, param_branch);
}

void EmitOp_Branch32Tag(Cachet_ContextRef cx,
                        IR_MASM::OpsRef ops,
                        Type_Condition::Ref param_condition,
                        Type_Reg::Ref param_lhsReg,
                        Type_Reg::Ref param_rhsReg,
                        IR_MASM::LabelRef param_branch) {
  ops.branch32(param_condition, param_lhsReg, param_rhsReg, param_branch);
}

void EmitOp_Branch32Imm(Cachet_ContextRef cx,
                        IR_MASM::OpsRef ops,
                        Type_Condition::Ref param_condition,
                        Type_Reg::Ref param_lhsReg,
                        Type_Int32::Ref param_rhsInt32,
                        IR_MASM::LabelRef param_branch) {
  ops.branch32(param_condition, param_lhsReg, Imm32(param_rhsInt32), param_branch);
}

void EmitOp_Branch32AddressImm32(Cachet_ContextRef cx,
                                 IR_MASM::OpsRef ops,
                                 Type_Condition::Ref param_condition,
                                 Type_Address::Ref param_address,
                                 Type_Int32::Ref param_rhsInt32,
                                 IR_MASM::LabelRef param_branch) {
  ops.branch32(param_condition, param_address, Imm32(param_rhsInt32), param_branch);
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

void EmitOp_BranchAdd32(Cachet_ContextRef cx,
                                         IR_MASM::OpsRef ops,
                                         Type_Condition::Ref param_condition,
                                         Type_Reg::Ref param_srcReg,
                                         Type_Reg::Ref param_dstReg,
                                         IR_MASM::LabelRef param_branch) {
  ops.branchAdd32(param_condition, param_srcReg, param_dstReg,
                         param_branch);
}

void EmitOp_BranchAdd32Imm(Cachet_ContextRef cx,
                           IR_MASM::OpsRef ops,
                           Type_Condition::Ref param_condition,
                           Type_Int32::Ref param_lhsInt32,
                           Type_Reg::Ref param_dstReg,
                           IR_MASM::LabelRef param_branch) {
  ops.branchAdd32(param_condition, Imm32(param_lhsInt32), param_dstReg, param_branch);
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

void EmitOp_BranchSub32Imm(Cachet_ContextRef cx,
                           IR_MASM::OpsRef ops,
                           Type_Condition::Ref param_condition,
                           Type_Int32::Ref param_lhsInt32,
                           Type_Reg::Ref param_dstReg,
                           IR_MASM::LabelRef param_branch) {
  ops.branchSub32(param_condition, Imm32(param_lhsInt32), param_dstReg, param_branch);
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

void EmitOp_Not32(Cachet_ContextRef cx,
                  IR_MASM::OpsRef ops,
                  Type_Reg::Ref param_valueReg) {
  ops.not32(param_valueReg);
}

void EmitOp_And32(Cachet_ContextRef cx,
                  IR_MASM::OpsRef ops,
                  Type_Reg::Ref param_srcReg,
                  Type_Reg::Ref param_dstReg) {
  ops.and32(param_srcReg, param_dstReg);
}

void EmitOp_Or32(Cachet_ContextRef cx,
                                         IR_MASM::OpsRef ops,
                                         Type_Reg::Ref param_srcReg,
                                         Type_Reg::Ref param_dstReg) {
  ops.or32(param_srcReg, param_dstReg);
}

void EmitOp_Xor32(Cachet_ContextRef cx,
                  IR_MASM::OpsRef ops,
                  Type_Reg::Ref param_srcReg,
                  Type_Reg::Ref param_dstReg) {
  ops.xor32(param_srcReg, param_dstReg);
}

void EmitOp_FlexibleLshift32(Cachet_ContextRef cx,
                             IR_MASM::OpsRef ops,
                             Type_Reg::Ref param_shiftReg,
                             Type_Reg::Ref param_srcDestReg) {
  ops.flexibleLshift32(param_shiftReg, param_srcDestReg);
}

void EmitOp_FlexibleRshift32Arithmetic(Cachet_ContextRef cx,
                                       IR_MASM::OpsRef ops,
                                       Type_Reg::Ref param_shiftReg,
                                       Type_Reg::Ref param_srcDestReg) {
  ops.flexibleRshift32Arithmetic(param_shiftReg, param_srcDestReg);
}

void EmitOp_TestObjectSet(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                          Type_Condition::Ref param_condition,
                          Type_ValueReg::Ref param_valueReg,
                          Type_Reg::Ref param_dstReg) {
  ops.testObjectSet(param_condition, param_valueReg, param_dstReg);
}

void EmitOp_LoadArgumentsObjectLength(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                      Type_Reg::Ref param_objectReg,
                                      Type_Reg::Ref param_outputReg,
                                      IR_MASM::LabelRef param_failure) {
  ops.loadArgumentsObjectLength(param_objectReg, param_outputReg, param_failure);
}

void EmitOp_LoadArgumentsObjectElement(Cachet_ContextRef cx, IR_MASM::OpsRef ops,
                                       Type_Reg::Ref param_objReg,
                                       Type_Reg::Ref param_indexReg,
                                       Type_ValueReg::Ref param_outputReg,
                                       Type_Reg::Ref param_tempReg,
                                       IR_MASM::LabelRef param_failure) {
  ops.loadArgumentsObjectElement(param_objReg, param_indexReg, param_outputReg, param_tempReg, param_failure);
}

void EmitOp_LoadObjectProto(Cachet_ContextRef cx,
                            IR_MASM::OpsRef ops,
                            Type_Reg::Ref param_objectReg,
                            Type_Reg::Ref param_protoReg) {
  ops.loadObjProto(param_objectReg, param_protoReg);
}

void EmitOp_SplitTagForTest(Cachet_ContextRef cx,
                            IR_MASM::OpsRef ops,
                            Type_ValueReg::Ref param_valueReg,
                            Type_Reg::Ref param_tagReg) {
  // skipping conversion from ScratchTagScope
  ops.splitTag(param_valueReg, param_tagReg);
}

};  // namespace Impl_MASM

namespace IR_MASM {

IR_MASM::OpsRef GetOutput(Cachet_ContextRef cx) {
  return cx.compiler->masm;
}

IR_MASM::LabelLocal NewLabel(Cachet_ContextRef cx) {
  return Label();
}

IR_MASM::LabelMutRef ToLabelMutRef(IR_MASM::LabelLocal& label) {
  return &label;
}

IR_MASM::LabelRef ToLabelRef(IR_MASM::LabelLocal& label) {
  return &label;
}

void BindLabel(Cachet_ContextRef cx, OpsRef ops, IR_MASM::LabelMutRef label) {
  ops.bind(label);
}

}; // namespace IR_MASM

namespace IR_CacheIR {

IR_MASM::OpsRef GetOutput(Cachet_ContextRef cx) {
  return cx.compiler->masm;
}

}; // namespace IR_CacheIR

namespace Impl_CacheIR {

Type_TypedOrValueReg::Ref Var_outputReg(Cachet_ContextRef cx) {
  return cx.compiler->outputUnchecked_.ref();
}

Type_FloatRegSet::MutRef Var_liveFloatRegSet(Cachet_ContextRef cx) {
  return cx.compiler->liveFloatRegs_.set(); 
}

Type_FailurePath::Val Fn_addFailurePath(Cachet_ContextRef cx) {
  FailurePath* failurePath;
  // TODO: use this result in some way
  cx.compiler->addFailurePath(&failurePath);
  return failurePath;
}

Type_ValueReg::Val Fn_allocateValueReg(Cachet_ContextRef cx) {
  return cx.compiler->allocator.allocateValueRegister(cx.compiler->masm);
}

void Fn_releaseValueReg(Cachet_ContextRef cx, Type_ValueReg::Ref param_valueReg) {
  return cx.compiler->allocator.releaseValueRegister(param_valueReg);
}

Type_Reg::Val Fn_allocateReg(Cachet_ContextRef cx) {
  return cx.compiler->allocator.allocateRegister(cx.compiler->masm);
}

void Fn_releaseReg(Cachet_ContextRef cx, Type_Reg::Ref param_reg) {
  return cx.compiler->allocator.releaseRegister(param_reg);
}

Type_Reg::Val Fn_allocateScratchReg(Cachet_ContextRef cx) {
#ifdef DEBUG
  cx.compiler->masm.debugTrackedRegisters_.add(ScratchReg);
#endif
  return ScratchReg;
}

void Fn_releaseScratchReg(Cachet_ContextRef cx) {
#ifdef DEBUG
  cx.compiler->masm.debugTrackedRegisters_.take(ScratchReg);
#endif
}



Type_Reg::Val Fn_defineObjectId(Cachet_ContextRef cx,
                                            Type_ObjectId::Ref param_objectId) {
  return cx.compiler->allocator.defineRegister(cx.compiler->masm, param_objectId);
}

Type_Reg::Val Fn_defineInt32Id(Cachet_ContextRef cx,
                                            Type_Int32Id::Ref param_int32Id) {
  return cx.compiler->allocator.defineRegister(cx.compiler->masm, param_int32Id);
}

Type_ValueReg::Val Fn_defineNumberId(Cachet_ContextRef cx,
                                            Type_NumberId::Ref param_numberId) {
  return cx.compiler->allocator.defineValueRegister(cx.compiler->masm, param_numberId);
}

Type_Reg::Val Fn_defineValueTagId(Cachet_ContextRef cx,
                                            Type_ValueTagId::Ref param_valueTagId) {
  return cx.compiler->allocator.defineRegister(cx.compiler->masm, param_valueTagId);
}

Type_JSValueType::Ref Fn_knownType(Cachet_ContextRef cx, Type_ValueId::Ref param_valueId) {
  return cx.compiler->allocator.knownType(param_valueId);
}

Type_ValueReg::Val Fn_useValueId(
    Cachet_ContextRef cx,
    Type_ValueId::Ref param_valueId) {
    return cx.compiler->allocator.useValueRegister(cx.compiler->masm, param_valueId);
}

Type_Reg::Val Fn_useObjectId(Cachet_ContextRef cx,
                                            Type_ObjectId::Ref param_objectId) {
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_objectId);
}

Type_Reg::Val Fn_useInt32Id(Cachet_ContextRef cx,
                                            Type_Int32Id::Ref param_int32Id) {
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_int32Id);
}

Type_ValueReg::Val Fn_useNumberId(Cachet_ContextRef cx,
                                            Type_NumberId::Ref param_numberId) {
    return cx.compiler->allocator.useValueRegister(cx.compiler->masm, param_numberId);
}

Type_Reg::Val Fn_useBoolId(Cachet_ContextRef cx,
                                            Type_BoolId::Ref param_boolId) {
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_boolId);
}

Type_Reg::Val Fn_useStringId(Cachet_ContextRef cx,
                                            Type_StringId::Ref param_stringId) {
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_stringId);
}

Type_Reg::Val Fn_useSymbolId(Cachet_ContextRef cx,
                                            Type_SymbolId::Ref param_symbolId) {
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_symbolId);
}

Type_Reg::Val Fn_useBigIntId(Cachet_ContextRef cx,
                                            Type_BigIntId::Ref param_bigIntId) {
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_bigIntId);
}

Type_Reg::Val Fn_useValueTagId(Cachet_ContextRef cx,
                                            Type_ValueTagId::Ref param_valueTagId) {
    return cx.compiler->allocator.useRegister(cx.compiler->masm, param_valueTagId);
}

void Fn_emitLoadInt32StubField(
    Cachet_ContextRef cx,
    Type_Int32Field::Ref param_int32Field,
    Type_Reg::Ref param_dstReg) {
  StubFieldOffset val(param_int32Field, StubField::Type::RawInt32);
  cx.compiler->emitLoadStubField(val, param_dstReg);
}

void Fn_emitLoadObjectStubField(
    Cachet_ContextRef cx,
    Type_ObjectField::Ref param_objectField,
    Type_Reg::Ref param_dstReg) {
  StubFieldOffset val(param_objectField, StubField::Type::Object);
  cx.compiler->emitLoadStubField(val, param_dstReg);
}

void Fn_emitLoadValueStubField(
    Cachet_ContextRef cx,
    Type_ValueField::Ref param_valueField,
    Type_ValueReg::Ref param_dstReg) {
  StubFieldOffset val(param_valueField, StubField::Type::Value);
  cx.compiler->emitLoadValueStubField(val, param_dstReg);
}

Type_Int32::Val Fn_readInt32Field(
    Cachet_ContextRef cx,
    Type_Int32Field::Ref param_int32Field) {
  return cx.compiler->int32StubField(param_int32Field);
}

Type_String::Val Fn_readStringField(
    Cachet_ContextRef cx,
    Type_StringField::Ref param_stringField) {
  return cx.compiler->stringStubField(param_stringField);
}

Type_Shape::Val Fn_readShapeField(
    Cachet_ContextRef cx,
    Type_ShapeField::Ref param_shapeField) {
  return cx.compiler->shapeStubField(param_shapeField);
}

Type_Class::Val Fn_readClassField(
    Cachet_ContextRef cx,
    Type_ClassField::Ref param_classField) {
  return cx.compiler->classStubField(param_classField);
}

Type_Value::Val Fn_readValueField(
    Cachet_ContextRef cx,
    Type_ValueField::Ref param_valueField) {
  return cx.compiler->valueStubField(param_valueField);
}

Type_Bool::Val Fn_objectGuardNeedsSpectreMitigations(
    Cachet_ContextRef cx,
    Type_ObjectId::Ref param_objectId) {
  return cx.compiler->objectGuardNeedsSpectreMitigations(param_objectId);
}

};  // namespace Impl_CacheIR

#define CACHET_CacheIR_COMPILER
#define CACHET_MASM_EMIT

#include "jit/CachetGenerated.inc"

#undef CACHET_CacheIR_COMPILER
#undef CACHET_MASM_EMIT

};  // namespace cachet

};  // namespace jit

};  // namespace js
