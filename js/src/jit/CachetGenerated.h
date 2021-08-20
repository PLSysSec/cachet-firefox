// External forward declarations.

namespace Impl_ValueType {

// inline constexpr Type_ValueType::Val Variant_Double;

// inline constexpr Type_ValueType::Val Variant_Int32;

// inline constexpr Type_ValueType::Val Variant_Boolean;

// inline constexpr Type_ValueType::Val Variant_Undefined;

// inline constexpr Type_ValueType::Val Variant_Null;

// inline constexpr Type_ValueType::Val Variant_Magic;

// inline constexpr Type_ValueType::Val Variant_String;

// inline constexpr Type_ValueType::Val Variant_Symbol;

// inline constexpr Type_ValueType::Val Variant_PrivateGCThing;

// inline constexpr Type_ValueType::Val Variant_BigInt;

// inline constexpr Type_ValueType::Val Variant_Object;

};  // namespace Impl_ValueType

namespace Impl_Value {

Type_ValueType::Val Fn_typeOf(Cachet_Context cx, Type_Value::Ref param_value);

Type_Object::Val Fn_toObjectUnchecked(Cachet_Context cx, Type_Value::Ref param_value);

};  // namespace Impl_Value

namespace Impl_Object {

Type_Shape::Val Fn_shapeOf(Cachet_Context cx, Type_Object::Ref param_object);

};  // namespace Impl_Object

namespace Impl_NativeObject {

inline Type_NativeObject::Val From_Object(Type_Object::Val in);

inline Type_Object::Val To_Object(Type_NativeObject::Val in);

Type_Value::Val Fn_getFixedSlotUnchecked(Cachet_Context cx, Type_NativeObject::Ref param_nativeObject, Type_Int32::Ref param_slot);

};  // namespace Impl_NativeObject

namespace Impl_Shape {

Type_Class::Val Fn_classOf(Cachet_Context cx, Type_Shape::Ref param_shape);

Type_Bool::Val Fn_hasFixedSlot(Cachet_Context cx, Type_Shape::Ref param_shape, Type_Int32::Ref param_slot);

};  // namespace Impl_Shape

namespace Impl_Class {

Type_Bool::Val Fn_isNativeObject(Cachet_Context cx, Type_Class::Ref param_class);

};  // namespace Impl_Class

// Internal forward declarations.

namespace Impl_Value {

Type_Bool::Val Fn_isObject(Cachet_Context cx, Type_Value::Ref param_value);

Cachet_Result<Type_Object::Val> Fn_toObject(Cachet_Context cx, Type_Value::Ref param_value);

};  // namespace Impl_Value

namespace Impl_Object {

Cachet_Result<Type_NativeObject::Val> Fn_toNativeObject(Cachet_Context cx, Type_Object::Ref param_object);

Cachet_Result<Type_Value::Val> Fn_getFixedSlot(Cachet_Context cx, Type_Object::Ref param_object, Type_Int32::Ref param_slot);

};  // namespace Impl_Object

namespace Impl_NativeObject {

Cachet_Result<Type_Value::Val> Fn_getFixedSlot(Cachet_Context cx, Type_NativeObject::Ref param_nativeObject, Type_Int32::Ref param_slot);

};  // namespace Impl_NativeObject

Cachet_Result<Type_Unit::Val> Op_GuardToObject(Cachet_Context cx, Type_Value::Ref param_value, Type_Object::OutRef param_object);

Cachet_Result<Type_Unit::Val> Op_GuardShape(Cachet_Context cx, Type_Object::Ref param_object, Type_Shape::Ref param_shape);

Type_Value::Val Op_LoadFixedSlotResult(Cachet_Context cx, Type_Object::Ref param_object, Type_Int32::Ref param_slot);