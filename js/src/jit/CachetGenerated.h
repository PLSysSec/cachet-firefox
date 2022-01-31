// External forward declarations.

namespace Impl_ValueType {

inline Type_ValueType::Ref Variant_Double();

inline Type_ValueType::Ref Variant_Int32();

inline Type_ValueType::Ref Variant_Bool();

inline Type_ValueType::Ref Variant_Undefined();

inline Type_ValueType::Ref Variant_Null();

inline Type_ValueType::Ref Variant_Magic();

inline Type_ValueType::Ref Variant_String();

inline Type_ValueType::Ref Variant_Symbol();

inline Type_ValueType::Ref Variant_PrivateGCThing();

inline Type_ValueType::Ref Variant_BigInt();

inline Type_ValueType::Ref Variant_Object();

};  // namespace Impl_ValueType

namespace Impl_Condition {

inline Type_Condition::Ref Variant_Equal();

inline Type_Condition::Ref Variant_NotEqual();

};  // namespace Impl_Condition

namespace Impl_Value {

Type_ValueType::Val Fn_typeOf(Cachet_ContextRef cx, Type_Value::Ref param_value);

Type_Value::Val Fn_fromObjectUnchecked(Cachet_ContextRef cx, Type_Object::Ref param_value);

Type_Object::Val Fn_toObjectUnchecked(Cachet_ContextRef cx, Type_Value::Ref param_value);

};  // namespace Impl_Value

namespace Impl_Object {

Type_Shape::Val Fn_shapeOfUnchecked(Cachet_ContextRef cx, Type_Heap::Ref param_heap, Type_Object::Ref param_object);

};  // namespace Impl_Object

namespace Impl_NativeObject {

inline Type_NativeObject::Val From_Object(Type_Object::Val param_in);

inline Type_NativeObject::Ref From_Object(Type_Object::Ref param_in);

inline Type_Object::Val To_Object(Type_NativeObject::Val param_in);

inline Type_Object::Ref To_Object(Type_NativeObject::Ref param_in);

Type_Value::Val Fn_getFixedSlotUnchecked(Cachet_ContextRef cx, Type_Heap::Ref param_heap, Type_NativeObject::Ref param_nativeObject, Type_Int32::Ref param_slot);

};  // namespace Impl_NativeObject

namespace Impl_Shape {

Type_Class::Val Fn_classOf(Cachet_ContextRef cx, Type_Shape::Ref param_shape);

Type_Bool::Val Fn_hasFixedSlot(Cachet_ContextRef cx, Type_Shape::Ref param_shape, Type_Int32::Ref param_slot);

};  // namespace Impl_Shape

namespace Impl_Class {

Type_Bool::Val Fn_isNativeObject(Cachet_ContextRef cx, Type_Class::Ref param_class);

};  // namespace Impl_Class

namespace Impl_CacheIR {

inline Type_ValueReg::Ref Var_outputReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler);

Type_ValueReg::Val Fn_allocateValueReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler);

void Fn_releaseValueReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ValueReg::Ref param_valueReg);

Type_Reg::Val Fn_allocateReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler);

void Fn_releaseReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_Reg::Ref param_reg);

Type_ValueReg::Val Fn_useValueReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ValueId::Ref param_valueId);

Type_Reg::Val Fn_useObjectReg(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ObjectId::Ref param_objectId);

Type_Int32::Val Fn_readInt32Field(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_Int32Field::Ref param_int32Field);

Type_Shape::Val Fn_readShapeField(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ShapeField::Ref param_shapeField);

Type_Bool::Val Fn_objectGuardNeedsSpectreMitigations(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ObjectId::Ref param_objectId);

};  // namespace Impl_CacheIR

inline Type_Heap::MutRef Var_heap(Cachet_ContextRef cx);

inline Type_Int32::Ref Var_zero(Cachet_ContextRef cx);

// Internal forward declarations.

namespace Impl_Value {

Type_Bool::Val Fn_isObject(Cachet_ContextRef cx, Type_Value::Ref param_value);

Type_Value::Val Fn_fromObject(Cachet_ContextRef cx, Type_Object::Ref param_object);

Type_Object::Val Fn_toObject(Cachet_ContextRef cx, Type_Value::Ref param_value);

};  // namespace Impl_Value

namespace Impl_Object {

Type_Shape::Val Fn_shapeOf(Cachet_ContextRef cx, Type_Object::Ref param_object);

Type_NativeObject::Val Fn_toNativeObject(Cachet_ContextRef cx, Type_Object::Ref param_object);

Type_Value::Val Fn_getFixedSlot(Cachet_ContextRef cx, Type_Object::Ref param_object, Type_Int32::Ref param_slot);

};  // namespace Impl_Object

namespace Impl_NativeObject {

Type_Value::Val Fn_getFixedSlot(Cachet_ContextRef cx, Type_NativeObject::Ref param_nativeObject, Type_Int32::Ref param_slot);

};  // namespace Impl_NativeObject

namespace Impl_CacheIR {

void Op_GuardToObject(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ValueId::Ref param_valueId, IR_MASM::LabelRef param_failure);

void Op_GuardShape(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ObjectId::Ref param_objectId, Type_ShapeField::Ref param_shapeField, IR_MASM::LabelRef param_failure);

void Op_LoadFixedSlotResult(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ObjectId::Ref param_objectId, Type_Int32Field::Ref param_slotField);

};  // namespace Impl_CacheIR
