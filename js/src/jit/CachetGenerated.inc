// Internal definitions.

namespace Impl_Value {

Type_Bool::Val Fn_isObject(Cachet_Context cx, Type_Value::Ref param_value) {
  return ({
    Type_ValueType::CompareEq(Impl_Value::Fn_typeOf(cx, param_value), Impl_ValueType::Variant_Object);
  });
}

Cachet_Result<Type_Object::Val> Fn_toObject(Cachet_Context cx, Type_Value::Ref param_value) {
  return Cachet_Ok(({
    if (!Impl_Value::Fn_isObject(cx, param_value)) {
      return Cachet_Err(Cachet_Bail());
    }
    Impl_Value::Fn_toObjectUnchecked(cx, param_value);
  }));
}

};  // namespace Impl_Value

namespace Impl_Object {

Cachet_Result<Type_NativeObject::Val> Fn_toNativeObject(Cachet_Context cx, Type_Object::Ref param_object) {
  return Cachet_Ok(({
    Type_Shape::Local local_shape_0(Type_Shape::ToLocal(cx, Impl_Object::Fn_shapeOf(cx, param_object)));
    Type_Class::Local local_class_1(Type_Class::ToLocal(cx, Impl_Shape::Fn_classOf(cx, Type_Shape::ToRef(local_shape_0))));
    if (!Impl_Class::Fn_isNativeObject(cx, Type_Class::ToRef(local_class_1))) {
      return Cachet_Err(Cachet_Bail());
    }
    Impl_NativeObject::From_Object(Type_Object::ToVal(param_object));
  }));
}

Cachet_Result<Type_Value::Val> Fn_getFixedSlot(Cachet_Context cx, Type_Object::Ref param_object, Type_Int32::Ref param_slot) {
  return Cachet_Ok(({
    Type_NativeObject::Local local_nativeObject_0(Type_NativeObject::ToLocal(cx, ({
      Cachet_Result<Type_NativeObject::Val> result_0(Impl_Object::Fn_toNativeObject(cx, param_object));
      if (!Cachet_IsOk(result_0)) {
        return Cachet_Err(Cachet_Bail());
      }
      Cachet_Unwrap(static_cast<Cachet_Result<Type_NativeObject::Val>&&>(result_0));
    })));
    ({
      Cachet_Result<Type_Value::Val> result_1(Impl_NativeObject::Fn_getFixedSlot(cx, Type_NativeObject::ToRef(local_nativeObject_0), param_slot));
      if (!Cachet_IsOk(result_1)) {
        return Cachet_Err(Cachet_Bail());
      }
      Cachet_Unwrap(static_cast<Cachet_Result<Type_Value::Val>&&>(result_1));
    });
  }));
}

};  // namespace Impl_Object

namespace Impl_NativeObject {

Cachet_Result<Type_Value::Val> Fn_getFixedSlot(Cachet_Context cx, Type_NativeObject::Ref param_nativeObject, Type_Int32::Ref param_slot) {
  return Cachet_Ok(({
    Type_Shape::Local local_shape_0(Type_Shape::ToLocal(cx, ({
      Type_Object::Local temp_0(Type_Object::ToLocal(cx, Impl_NativeObject::To_Object(Type_NativeObject::ToVal(param_nativeObject))));
      Impl_Object::Fn_shapeOf(cx, Type_Object::ToRef(temp_0));
    })));
    if (!Impl_Shape::Fn_hasFixedSlot(cx, Type_Shape::ToRef(local_shape_0), param_slot)) {
      return Cachet_Err(Cachet_Bail());
    }
    Impl_NativeObject::Fn_getFixedSlotUnchecked(cx, param_nativeObject, param_slot);
  }));
}

};  // namespace Impl_NativeObject

Cachet_Result<Type_Unit::Val> Op_GuardToObject(Cachet_Context cx, Type_Value::Ref param_value, Type_Object::OutRef param_object) {
  return Cachet_Ok(({
    (void) Type_Object::SetOutRef(param_object, ({
      Cachet_Result<Type_Object::Val> result_0(Impl_Value::Fn_toObject(cx, param_value));
      if (!Cachet_IsOk(result_0)) {
        return Cachet_Err(Cachet_Bail());
      }
      Cachet_Unwrap(static_cast<Cachet_Result<Type_Object::Val>&&>(result_0));
    }));
    Const_unit;
  }));
}

Cachet_Result<Type_Unit::Val> Op_GuardShape(Cachet_Context cx, Type_Object::Ref param_object, Type_Shape::Ref param_shape) {
  return Cachet_Ok(({
    if (!Type_Shape::CompareEq(Impl_Object::Fn_shapeOf(cx, param_object), Type_Shape::ToVal(param_shape))) {
      return Cachet_Err(Cachet_Bail());
    }
    Const_unit;
  }));
}

Type_Value::Val Op_LoadFixedSlotResult(Cachet_Context cx, Type_Object::Ref param_object, Type_Int32::Ref param_slot) {
  return ({
    Cachet_Unwrap(Impl_Object::Fn_getFixedSlot(cx, param_object, param_slot));
  });
}