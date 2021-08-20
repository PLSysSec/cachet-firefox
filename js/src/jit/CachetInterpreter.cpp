/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/CachetInterpreter.h"

using namespace js::jit::cachet;

namespace js {

namespace jit {

namespace cachet {

Type_ValueType::Val Impl_Value::Fn_typeOf(Cachet_Context cx,
                                          Type_Value::Ref param_value) {
  return param_value.type();
}

Type_Object::Val Impl_Value::Fn_toObjectUnchecked(Cachet_Context cx,
                                                  Type_Value::Ref param_value) {
  return &param_value.toObject();
};

Type_Shape::Val Impl_Object::Fn_shapeOf(Cachet_Context cx,
                                        Type_Object::Ref param_object) {
  return param_object->shape();
}

Type_Value::Val Impl_NativeObject::Fn_getFixedSlotUnchecked(
    Cachet_Context cx, Type_NativeObject::Ref param_nativeObject,
    Type_Int32::Ref param_slot) {
  return param_nativeObject->getFixedSlot(param_slot);
}

Type_Class::Val Impl_Shape::Fn_classOf(Cachet_Context cx,
                                       Type_Shape::Ref param_shape) {
  return param_shape->getObjectClass();
}

Type_Bool::Val Impl_Shape::Fn_hasFixedSlot(Cachet_Context cx,
                                           Type_Shape::Ref param_shape,
                                           Type_Int32::Ref param_slot) {
  return param_slot >= 0 &&
         param_slot < Type_Int32::Val(param_shape->numFixedSlots());
}

Type_Bool::Val Impl_Class::Fn_isNativeObject(Cachet_Context cx,
                                             Type_Class::Ref param_class) {
  return param_class->isNativeObject();
}

#include "jit/CachetGenerated.inc"

};  // namespace cachet

};  // namespace jit

};  // namespace js
