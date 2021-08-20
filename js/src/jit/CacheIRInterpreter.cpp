/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/CacheIRInterpreter.h"

#include "mozilla/FloatingPoint.h"

#include "builtin/RegExp.h"
#include "jit/CachetInterpreter.h"
#include "vm/JSObject.h"
#include "vm/NativeObject.h"
#include "vm/Uint8Clamped.h"

#include "vm/JSObject-inl.h"

using namespace js;
using namespace js::jit;
using namespace mozilla;

Maybe<CacheIRInterpreterResult> CacheIRInterpreter::interpret() {
  CacheIRReader reader(&stubInfo_);

  Maybe<CacheIRInterpreterResult> result;

  do {
    const CacheOp cacheOp(reader.readOp());
    fprintf(stderr, "cacheOp: %s\n", CacheIROpNames[size_t(cacheOp)]);
    switch (cacheOp) {
#define HANDLE_OP_CASE(op, ...)     \
  case CacheOp::op: {               \
    result = interpret##op(reader); \
    break;                          \
  }
      CACHE_IR_OPS(HANDLE_OP_CASE)
#undef HANDLE_OP_CASE

      default: {
        MOZ_ASSERT_UNREACHABLE("Invalid op");
      }
    }
  } while (!result && reader.more());

  return result;
}

#define CACHE_OP(op, ...) \
  Maybe<CacheIRInterpreterResult> CacheIRInterpreter::interpret##op(__VA_ARGS__)

#define CACHE_CONTINUE \
  do {                 \
    return Nothing();  \
  } while (false)

#define CACHE_RETURN                               \
  do {                                             \
    return Some(CacheIRInterpreterResult::Return); \
  } while (false)

#define CACHE_BAIL                                  \
  do {                                              \
    return Some(CacheIRInterpreterResult::Bailout); \
  } while (false)

#define CACHE_THROW                                   \
  do {                                                \
    return Some(CacheIRInterpreterResult::Exception); \
  } while (false)

#define CACHE_ASSERT(test, otherwise) \
  do {                                \
    if (!(test)) {                    \
      otherwise;                      \
    }                                 \
  } while (false)

#define CACHE_ASSERT_THROW(test) \
  do {                           \
    if (!(test)) {               \
      CACHE_THROW;               \
    }                            \
  } while (false)

CACHE_OP(ReturnFromIC) { CACHE_RETURN; }

CACHE_OP(GuardToObject, const ValOperandId inputId) {
  /*
  const HandleValue input(getOperand(inputId));
  CACHE_ASSERT(input.isObject(), CACHE_BAIL);
  setOperand(ObjOperandId(inputId.id()), &input.toObject());
  CACHE_CONTINUE;
  */
  const RootedValue input(cx_, getOperand(inputId));
  RootedObject object(cx_);
  if (cachet::Op_GuardToObject(cx_, input, &object).isOk()) {
    setOperand(ObjOperandId(inputId.id()), object);
    CACHE_CONTINUE;
  } else {
    CACHE_BAIL;
  }
}

CACHE_OP(GuardIsNullOrUndefined, const ValOperandId inputId) {
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.isNullOrUndefined(), CACHE_BAIL);
  CACHE_CONTINUE;
}

CACHE_OP(GuardIsNull, const ValOperandId inputId) {
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.isNull(), CACHE_BAIL);
  CACHE_CONTINUE;
}

CACHE_OP(GuardIsUndefined, const ValOperandId inputId) {
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.isUndefined(), CACHE_BAIL);
  CACHE_CONTINUE;
}

CACHE_OP(GuardToBoolean, const ValOperandId inputId) {
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.isBoolean(), CACHE_BAIL);
  setOperand(BooleanOperandId(inputId.id()), input.toBoolean());
  CACHE_CONTINUE;
}

CACHE_OP(GuardToString, const ValOperandId inputId) {
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.isString(), CACHE_BAIL);
  setOperand(StringOperandId(inputId.id()), input.toString());
  CACHE_CONTINUE;
}

CACHE_OP(GuardToSymbol, const ValOperandId inputId) {
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.isSymbol(), CACHE_BAIL);
  setOperand(SymbolOperandId(inputId.id()), input.toSymbol());
  CACHE_CONTINUE;
}

CACHE_OP(GuardToBigInt, const ValOperandId inputId) {
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.isBigInt(), CACHE_BAIL);
  setOperand(BigIntOperandId(inputId.id()), input.toBigInt());
  CACHE_CONTINUE;
}

CACHE_OP(GuardIsNumber, const ValOperandId inputId) {
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.isNumber(), CACHE_BAIL);
  CACHE_CONTINUE;
}

CACHE_OP(GuardToInt32, const ValOperandId inputId) {
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.isInt32(), CACHE_BAIL);
  setOperand(Int32OperandId(inputId.id()), input.toInt32());
  CACHE_CONTINUE;
}

CACHE_OP(GuardBooleanToInt32, const ValOperandId inputId,
         const Int32OperandId resultId) {
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.isBoolean(), CACHE_BAIL);
  setOperand(resultId, int32_t(input.toBoolean()));
  CACHE_CONTINUE;
}

CACHE_OP(GuardToInt32Index, const ValOperandId inputId,
         const Int32OperandId resultId) {
  const RootedValue input(cx_, getOperand(inputId));
  int32_t result;
  if (input.isInt32()) {
    result = input.toInt32();
  } else if (input.isDouble()) {
    // ToPropertyKey(-0.0) is "0", so we can truncate -0.0 to 0 here.
    CACHE_ASSERT(NumberEqualsInt32(input.toDouble(), &result), CACHE_BAIL);
  } else {
    CACHE_BAIL;
  }
  setOperand(resultId, result);
  CACHE_CONTINUE;
}

CACHE_OP(Int32ToIntPtr, const Int32OperandId inputId,
         const IntPtrOperandId resultId) {
  const int32_t input(getOperand(inputId));
  setOperand(resultId, intptr_t(input));
  CACHE_CONTINUE;
}

CACHE_OP(GuardNumberToIntPtrIndex, const NumberOperandId inputId,
         const bool supportOOB, const IntPtrOperandId resultId) {
  const double input(getNumberOperand(inputId));
  intptr_t result;
  // ToPropertyKey(-0.0) is "0", so we can truncate -0.0 to 0 here.
  if (!NumberEqualsIntPtr(input, &result)) {
    if (supportOOB) {
      // Substitute the invalid index with an arbitrary out-of-bounds index.
      result = -1;
    } else {
      CACHE_BAIL;
    }
  }
  setOperand(resultId, result);
  CACHE_CONTINUE;
}

CACHE_OP(GuardToInt32ModUint32, const ValOperandId inputId,
         const Int32OperandId resultId) {
  const RootedValue input(cx_, getOperand(inputId));
  if (input.isInt32()) {
    setOperand(resultId, input.toInt32());
  } else if (input.isDouble()) {
    // If we have a double-valued input, and the whole part of the number won't
    // fit inside a 32-bit signed integer, we're allowed to return only the
    // lower 32 bits of the result (i.e., modulo 2**32 from the perspective of
    // an unsigned integer, hence the name of the operation). This is most
    // relevant on 64-bit platforms. Implementations are not required to do
    // this, and can instead bail out if they hit this case.
    //
    // Note also that -0.0 is truncated to 0 here.
    intptr_t result;
    CACHE_ASSERT(NumberTruncatesToIntPtr(input.toDouble(), &result),
                 CACHE_BAIL);
    setOperand(resultId, int32_t(result));
  } else {
    CACHE_BAIL;
  }
  CACHE_CONTINUE;
}

CACHE_OP(GuardToUint8Clamped, const ValOperandId inputId,
         const Int32OperandId resultId) {
  const RootedValue input(cx_, getOperand(inputId));
  int32_t result;
  if (input.isInt32()) {
    result = input.toInt32();
  } else if (input.isDouble()) {
    result = uint8_clamped(input.toDouble());
  } else {
    CACHE_BAIL;
  }
  setOperand(resultId, result);
  CACHE_CONTINUE;
}

CACHE_OP(GuardNonDoubleType, const ValOperandId inputId, const ValueType type) {
  MOZ_ASSERT(JSValueType(type) > JSVAL_TYPE_DOUBLE);
  const RootedValue input(cx_, getOperand(inputId));
  CACHE_ASSERT(input.type() == type, CACHE_BAIL);
  CACHE_CONTINUE;
}

CACHE_OP(GuardShape, const ObjOperandId objId, const uint32_t shapeOffset) {
  /*
  const HandleObject obj(getOperand(objId));
  const RootedShape shape(cx_, getShapeField(shapeOffset));
  CACHE_ASSERT(obj->shape() == shape, CACHE_BAIL);
  CACHE_CONTINUE;
  */
  RootedShape shape(cx_, getShapeField(shapeOffset));
  RootedObject object(cx_);
  if (cachet::Op_GuardShape(cx_, getOperand(objId), shape).isOk()) {
    CACHE_CONTINUE;
  } else {
    CACHE_BAIL;
  }
}

CACHE_OP(GuardProto, const ObjOperandId objId, const uint32_t protoOffset) {
  const RootedObject proto(cx_, getObjectField(protoOffset));
  const HandleObject obj(getOperand(objId));
  CACHE_ASSERT(obj->taggedProto().raw() == proto, CACHE_BAIL);
  CACHE_CONTINUE;
}

CACHE_OP(GuardNullProto, const ObjOperandId objId) {
  const HandleObject obj(getOperand(objId));
  CACHE_ASSERT(!obj->taggedProto().raw(), CACHE_BAIL);
  CACHE_CONTINUE;
}

CACHE_OP(GuardClass, const ObjOperandId objId, const GuardClassKind kind) {
  const JSClass* const clasp(ClassForGuardClassKind(cx_, kind));
  const HandleObject obj(getOperand(objId));
  CACHE_ASSERT(obj->getClass() == clasp, CACHE_BAIL);
  CACHE_CONTINUE;
}

CACHE_OP(GuardAnyClass, const ObjOperandId objId, const uint32_t claspOffset) {
  const JSClass* const clasp(getClassField(claspOffset));
  const HandleObject obj(getOperand(objId));
  CACHE_ASSERT(obj->getClass() == clasp, CACHE_BAIL);
  CACHE_CONTINUE;
}

CACHE_OP(HasClassResult, const ObjOperandId objId, const uint32_t claspOffset) {
  const JSClass* const clasp(getClassField(claspOffset));
  const HandleObject obj(getOperand(objId));
  output().setVal(BooleanValue(obj->getClass() == clasp));
  CACHE_CONTINUE;
}

CACHE_OP(CallRegExpMatcherResult, const ObjOperandId regexpId,
         const StringOperandId inputId, const Int32OperandId lastIndexId) {
  const HandleObject regexp(getOperand(regexpId));
  const HandleString input(getOperand(inputId));
  const int32_t lastIndex(getOperand(lastIndexId));
  RootedValue result(cx_);
  CACHE_ASSERT(
      RegExpMatcherRaw(cx_, regexp, input, lastIndex, nullptr, &result),
      CACHE_THROW);
  output().setVal(result.get());
  CACHE_CONTINUE;
}

CACHE_OP(CallRegExpSearcherResult, const ObjOperandId regexpId,
         const StringOperandId inputId, const Int32OperandId lastIndexId) {
  const HandleObject regexp(getOperand(regexpId));
  const HandleString input(getOperand(inputId));
  const int32_t lastIndex(getOperand(lastIndexId));
  int32_t result;
  CACHE_ASSERT(
      RegExpSearcherRaw(cx_, regexp, input, lastIndex, nullptr, &result),
      CACHE_THROW);
  output().setVal(Int32Value(result));
  CACHE_CONTINUE;
}

CACHE_OP(CallRegExpTesterResult, const ObjOperandId regexpId,
         const StringOperandId inputId, const Int32OperandId lastIndexId) {
  const HandleObject regexp(getOperand(regexpId));
  const HandleString input(getOperand(inputId));
  const int32_t lastIndex(getOperand(lastIndexId));
  int32_t result;
  CACHE_ASSERT(RegExpTesterRaw(cx_, regexp, input, lastIndex, &result),
               CACHE_THROW);
  output().setVal(Int32Value(result));
  CACHE_CONTINUE;
}

CACHE_OP(RegExpFlagResult, const ObjOperandId regexpId,
         const int32_t flagsMask) {
  const HandleObject regexp(getOperand(regexpId));
  MOZ_ASSERT(regexp->is<RegExpObject>());
  output().setVal(
      BooleanValue(static_cast<bool>(regexp->as<RegExpObject>().getFlags())));
  CACHE_CONTINUE;
}

CACHE_OP(CallSubstringKernelResult, const StringOperandId strId,
         const Int32OperandId beginId, const Int32OperandId lengthId) {
  const HandleString str(getOperand(strId));
  const int32_t begin(getOperand(beginId));
  const int32_t length(getOperand(lengthId));
  const RootedString result(cx_, SubstringKernel(cx_, str, begin, length));
  CACHE_ASSERT(result, CACHE_THROW);
  output().setVal(StringValue(result));
  CACHE_CONTINUE;
}

const Value& CacheIRInterpreter::LoadDynamicSlot(
    const ObjOperandId objId, const uint32_t offsetOffset) const {
  const HandleObject obj(getOperand(objId));
  const int32_t offset(getInt32Field(offsetOffset));
  MOZ_ASSERT(obj->is<NativeObject>());
  const size_t dynamicSlot(NativeObject::getDynamicSlotIndexFromOffset(offset));
  return obj->as<NativeObject>().getDynamicSlot(dynamicSlot);
}

CACHE_OP(GuardDynamicSlotValue, const ObjOperandId objId,
         const uint32_t offsetOffset, const uint32_t valOffset) {
  const RootedValue val(cx_, getValueField(valOffset));
  const RootedValue slotVal(cx_, LoadDynamicSlot(objId, offsetOffset));
  CACHE_ASSERT(val == slotVal, CACHE_BAIL);
  CACHE_CONTINUE;
}

CACHE_OP(LoadFixedSlotResult, const ObjOperandId objId,
         const uint32_t offsetOffset) {
  /*
  const HandleObject obj(getOperand(objId));
  const int32_t offset(getInt32Field(offsetOffset));
  MOZ_ASSERT(obj->is<NativeObject>());
  const size_t fixedSlot(NativeObject::getFixedSlotIndexFromOffset(offset));
  output().setVal(obj->as<NativeObject>().getFixedSlot(fixedSlot));
  CACHE_CONTINUE;
  */
  const int32_t offset(getInt32Field(offsetOffset));
  const size_t slot(NativeObject::getFixedSlotIndexFromOffset(offset));
  output().setVal(cachet::Op_LoadFixedSlotResult(cx_, getOperand(objId), slot));
  CACHE_CONTINUE;
}

CACHE_OP(LoadDynamicSlotResult, const ObjOperandId objId,
         const uint32_t offsetOffset) {
  output().setVal(LoadDynamicSlot(objId, offsetOffset));
  CACHE_CONTINUE;
}
