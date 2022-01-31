// Internal definitions.

namespace Impl_Value {

Type_Bool::Val Impl_Value::Fn_isObject(Cachet_ContextRef cx, Type_Value::Ref param_value) {
  return ({
    Type_ValueType::Local local_tmp_0(Type_ValueType::ToLocal(cx, Impl_Value::Fn_typeOf(cx, param_value)));
    Type_ValueType::CompareEq(Type_ValueType::ToRef(local_tmp_0), Impl_ValueType::Variant_Object(cx));
  });
}

Type_Value::Val Impl_Value::Fn_fromObject(Cachet_ContextRef cx, Type_Object::Ref param_object) {
  Type_Value::Local local_value_0(Type_Value::ToLocal(cx, Impl_Value::Fn_fromObjectUnchecked(cx, param_object)));
  Cachet_Assert(Impl_Value::Fn_isObject(cx, Type_Value::ToRef(local_value_0)));
  Cachet_Assert(({
    Type_Object::Local local_tmp_1(Type_Object::ToLocal(cx, Impl_Value::Fn_toObjectUnchecked(cx, Type_Value::ToRef(local_value_0))));
    Type_Object::CompareEq(Type_Object::ToRef(local_tmp_1), param_object);
  }));
  return Type_Value::ToVal(local_value_0);
}

Type_Object::Val Impl_Value::Fn_toObject(Cachet_ContextRef cx, Type_Value::Ref param_value) {
  Cachet_Assert(Impl_Value::Fn_isObject(cx, param_value));
  Type_Object::Local local_object_0(Type_Object::ToLocal(cx, Impl_Value::Fn_toObjectUnchecked(cx, param_value)));
  Cachet_Assert(({
    Type_Value::Local local_tmp_1(Type_Value::ToLocal(cx, Impl_Value::Fn_fromObjectUnchecked(cx, Type_Object::ToRef(local_object_0))));
    Type_Value::CompareEq(Type_Value::ToRef(local_tmp_1), param_value);
  }));
  return Type_Object::ToVal(local_object_0);
}

};  // namespace Impl_Value

namespace Impl_Object {

Type_Shape::Val Impl_Object::Fn_shapeOf(Cachet_ContextRef cx, Type_Object::Ref param_object) {
  return ({
    Type_Heap::Local local_tmp_0(Type_Heap::ToLocal(cx, Var_heap(cx)));
    Impl_Object::Fn_shapeOfUnchecked(cx, Type_Heap::ToRef(local_tmp_0), param_object);
  });
}

Type_NativeObject::Val Impl_Object::Fn_toNativeObject(Cachet_ContextRef cx, Type_Object::Ref param_object) {
  Type_Shape::Local local_shape_0(Type_Shape::ToLocal(cx, Impl_Object::Fn_shapeOf(cx, param_object)));
  Type_Class::Local local_class_1(Type_Class::ToLocal(cx, Impl_Shape::Fn_classOf(cx, Type_Shape::ToRef(local_shape_0))));
  Cachet_Assert(Impl_Class::Fn_isNativeObject(cx, Type_Class::ToRef(local_class_1)));
  return Type_NativeObject::ToVal(Impl_NativeObject::From_Object(param_object));
}

Type_Value::Val Impl_Object::Fn_getFixedSlot(Cachet_ContextRef cx, Type_Object::Ref param_object, Type_Int32::Ref param_slot) {
  Type_NativeObject::Local local_nativeObject_0(Type_NativeObject::ToLocal(cx, Impl_Object::Fn_toNativeObject(cx, param_object)));
  return Impl_NativeObject::Fn_getFixedSlot(cx, Type_NativeObject::ToRef(local_nativeObject_0), param_slot);
}

};  // namespace Impl_Object

namespace Impl_NativeObject {

Type_Value::Val Impl_NativeObject::Fn_getFixedSlot(Cachet_ContextRef cx, Type_NativeObject::Ref param_nativeObject, Type_Int32::Ref param_slot) {
  Type_Shape::Local local_shape_0(Type_Shape::ToLocal(cx, Impl_Object::Fn_shapeOf(cx, Impl_NativeObject::To_Object(param_nativeObject))));
  Cachet_Assert(Impl_Shape::Fn_hasFixedSlot(cx, Type_Shape::ToRef(local_shape_0), param_slot));
  return ({
    Type_Heap::Local local_tmp_1(Type_Heap::ToLocal(cx, Var_heap(cx)));
    Impl_NativeObject::Fn_getFixedSlotUnchecked(cx, Type_Heap::ToRef(local_tmp_1), param_nativeObject, param_slot);
  });
}

};  // namespace Impl_NativeObject

namespace Impl_CacheIR {

void Impl_CacheIR::Op_GuardToObject(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ValueId::Ref param_valueId, IR_MASM::LabelRef param_failure) {
  Type_ValueReg::Local local_valueReg_0(Type_ValueReg::ToLocal(cx, Impl_CacheIR::Fn_useValueReg(cx, compiler, param_valueId)));
  IR_MASM::EmitOp_BranchTestObject(cx, IR_CacheIR::GetOutput(compiler), Impl_Condition::Variant_NotEqual(cx), Type_ValueReg::ToRef(local_valueReg_0), param_failure);
}

void Impl_CacheIR::Op_GuardShape(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ObjectId::Ref param_objectId, Type_ShapeField::Ref param_shapeField, IR_MASM::LabelRef param_failure) {
  Type_Reg::Local local_objectReg_0(Type_Reg::ToLocal(cx, Impl_CacheIR::Fn_useObjectReg(cx, compiler, param_objectId)));
  Type_Shape::Local local_shape_1(Type_Shape::ToLocal(cx, Impl_CacheIR::Fn_readShapeField(cx, compiler, param_shapeField)));
  if (Impl_CacheIR::Fn_objectGuardNeedsSpectreMitigations(cx, compiler, param_objectId)) {
    Type_Reg::Local local_scratchReg_2(Type_Reg::ToLocal(cx, Impl_CacheIR::Fn_allocateReg(cx, compiler)));
    IR_MASM::EmitOp_BranchTestObjectShape(cx, IR_CacheIR::GetOutput(compiler), Impl_Condition::Variant_NotEqual(cx), Type_Reg::ToRef(local_objectReg_0), Type_Shape::ToRef(local_shape_1), Type_Reg::ToRef(local_scratchReg_2), Type_Reg::ToRef(local_objectReg_0), param_failure);
    Impl_CacheIR::Fn_releaseReg(cx, compiler, Type_Reg::ToRef(local_scratchReg_2));
  } else {
    IR_MASM::EmitOp_BranchTestObjectShapeNoSpectreMitigations(cx, IR_CacheIR::GetOutput(compiler), Impl_Condition::Variant_NotEqual(cx), Type_Reg::ToRef(local_objectReg_0), Type_Shape::ToRef(local_shape_1), param_failure);
  }
}

void Impl_CacheIR::Op_LoadFixedSlotResult(Cachet_ContextRef cx, IR_CacheIR::CompilerRef compiler, Type_ObjectId::Ref param_objectId, Type_Int32Field::Ref param_slotField) {
  Type_Reg::Local local_objectReg_0(Type_Reg::ToLocal(cx, Impl_CacheIR::Fn_useObjectReg(cx, compiler, param_objectId)));
  Type_Int32::Local local_slot_1(Type_Int32::ToLocal(cx, Impl_CacheIR::Fn_readInt32Field(cx, compiler, param_slotField)));
  IR_MASM::EmitOp_LoadObjectFixedSlot(cx, IR_CacheIR::GetOutput(compiler), Type_Reg::ToRef(local_objectReg_0), Type_Int32::ToRef(local_slot_1), Impl_CacheIR::Var_outputReg(cx, compiler));
}

};  // namespace Impl_CacheIR
