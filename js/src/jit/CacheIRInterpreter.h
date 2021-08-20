/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_CacheIRInterpeter_h
#define jit_CacheIRInterpeter_h

#include "mozilla/Attributes.h"
#include "mozilla/Maybe.h"

#include <type_traits>
#include <utility>

#include "jit/CacheIR.h"
#include "jit/CacheIRCompiler.h"
#include "jit/CacheIRGenerated.h"
#include "js/GCPolicyAPI.h"
#include "js/Value.h"
#include "vm/BigIntType.h"
#include "vm/JSObject.h"
#include "vm/StringType.h"
#include "vm/SymbolType.h"

namespace js {

namespace jit {

class CacheIROperand {
  struct Payload {
    union {
#define DEFINE_UNION_MEMBER(Name) CacheTypes<CacheType::Name> Name;
      CACHE_IR_TYPES(DEFINE_UNION_MEMBER)
#undef DEFINE_UNION_MEMBER
      uint64_t bits;
    };
    Payload() : bits(0) {}
  };
  static_assert(sizeof(Payload) ==
                sizeof(decltype(std::declval<Payload>().bits)));

 public:
  enum class Tag : uint8_t {
#define DEFINE_TAG_VARIANT(Name) Name = uint8_t(CacheType::Name),
    CACHE_IR_TYPES(DEFINE_TAG_VARIANT)
#undef DEFINE_TAG_VARIANT
        Uninitialized = 0xFF,
  };
  static_assert(sizeof(Tag) == sizeof(CacheType));

 public:
  Tag tag() const { return tag_; }

  uint64_t asBits() const { return payload_.bits; }

  bool isInitialized() const { return tag_ != Tag::Uninitialized; }

  CacheType type() const {
    MOZ_ASSERT(isInitialized());
    return CacheType(tag_);
  }

 private:
  template <CacheType type>
  CacheTypes<type> const& as() const;

 public:
#define DEFINE_ACCESSORS(Name)                                     \
  bool is##Name() const { return tag_ == Tag::Name; }              \
                                                                   \
  CacheTypes<CacheType::Name> const& as##Name() const {            \
    MOZ_ASSERT(is##Name());                                        \
    return payload_.Name;                                          \
  }                                                                \
                                                                   \
  CacheTypes<CacheType::Name>& as##Name() {                        \
    MOZ_ASSERT(is##Name());                                        \
    return payload_.Name;                                          \
  }                                                                \
                                                                   \
  void set##Name(CacheTypes<CacheType::Name> const Name) {         \
    tag_ = Tag::Name;                                              \
    payload_.Name = Name;                                          \
  }                                                                \
                                                                   \
 private:                                                          \
  template <>                                                      \
  CacheTypes<CacheType::Name> const& as<CacheType::Name>() const { \
    return as##Name();                                             \
  }                                                                \
                                                                   \
 public:
  CACHE_IR_TYPES(DEFINE_ACCESSORS)
#undef DEFINE_ACCESSORS

  bool isValLike() const {
    return isVal() || (isInitialized() &&
                       ValueTypeFromCacheType(type()) != JSVAL_TYPE_UNKNOWN);
  }

 private:
  template <CacheType type>
  Value toValImpl() const {
    constexpr JSValueType valueType = ValueTypeFromCacheType(type);
    if constexpr (type == CacheType::Val) {
      return as<type>();
    } else if constexpr (valueType != JSVAL_TYPE_UNKNOWN) {
      return JS::TypedValue<valueType>::create(as<type>());
    } else {
      MOZ_CRASH("Can't box CacheIR operand");
    }
  }

 public:
  Value toVal() const {
    MOZ_ASSERT(isValLike());
    switch (type()) {
#define HANDLE_TYPE_CASE(Name)           \
  case CacheType::Name: {                \
    return toValImpl<CacheType::Name>(); \
    break;                               \
  }
      CACHE_IR_TYPES(HANDLE_TYPE_CASE)
#undef HANDLE_TYPE_CASE
    }
  }

  // "Number" includes Int32 operands as well as boxed Int32 and Double values.
  bool isNumber() const { return isInt32() || (isVal() && asVal().isNumber()); }
  double toNumber() const {
    MOZ_ASSERT(isNumber());
    return isInt32() ? double(asInt32()) : asVal().toNumber();
  }
  template <typename T>
  void setNumber(const T t) {
    setVal(JS::NumberValue(t));
  }

 private:
  Tag tag_;
  Payload payload_;
};

}  // namespace jit
}  // namespace js

namespace JS {

template <>
struct JS::GCPolicy<js::jit::CacheIROperand> {
#define DELEGATE_GC(method, ret, ...)                                \
 private:                                                            \
  template <typename T, class = void>                                \
  struct has_##method : std::false_type {};                          \
                                                                     \
  template <typename T>                                              \
  struct has_##method<T, std::void_t<decltype(GCPolicy<T>::method)>> \
      : std::true_type {};                                           \
                                                                     \
  template <typename T, typename... Args>                            \
  static auto delegate_##method(Args... args) {                      \
    if constexpr (has_##method<T>::value) {                          \
      return GCPolicy<T>::method(args...);                           \
    } else {                                                         \
      return IgnoreGCPolicy<T>::method(args...);                     \
    }                                                                \
  }                                                                  \
                                                                     \
 public:                                                             \
  static ret method(__VA_ARGS__)

  DELEGATE_GC(trace, void, JSTracer* const trc,
              js::jit::CacheIROperand* const value, const char* const name) {
    switch (value->tag()) {
      case js::jit::CacheIROperand::Tag::Uninitialized: {
        break;
      }
#define HANDLE_TRACE_CASE(Name)                                    \
  case js::jit::CacheIROperand::Tag::Name: {                       \
    delegate_trace<js::jit::CacheTypes<js::jit::CacheType::Name>>( \
        trc, &value->as##Name(), name);                            \
    break;                                                         \
  }
        CACHE_IR_TYPES(HANDLE_TRACE_CASE)
#undef HANDLE_TRACE_CASE
    }
  }

  DELEGATE_GC(sweep, void, js::jit::CacheIROperand* const value) {
    switch (value->tag()) {
      case js::jit::CacheIROperand::Tag::Uninitialized: {
        break;
      }
#define HANDLE_SWEEP_CASE(Name)                                    \
  case js::jit::CacheIROperand::Tag::Name: {                       \
    delegate_sweep<js::jit::CacheTypes<js::jit::CacheType::Name>>( \
        &value->as##Name());                                       \
    break;                                                         \
  }
        CACHE_IR_TYPES(HANDLE_SWEEP_CASE)
#undef HANDLE_SWEEP_CASE
    }
  }

  DELEGATE_GC(needsSweep, bool, js::jit::CacheIROperand* const value) {
    switch (value->tag()) {
      case js::jit::CacheIROperand::Tag::Uninitialized: {
        return false;
      }
#define HANDLE_NEEDS_SWEEP_CASE(Name)                                          \
  case js::jit::CacheIROperand::Tag::Name: {                                   \
    return delegate_needsSweep<js::jit::CacheTypes<js::jit::CacheType::Name>>( \
        &value->as##Name());                                                   \
  }
        CACHE_IR_TYPES(HANDLE_NEEDS_SWEEP_CASE)
#undef HANDLE_NEEDS_SWEEP_CASE
    }
  }

  DELEGATE_GC(traceWeak, bool, JSTracer* const trc,
              js::jit::CacheIROperand* const value) {
    switch (value->tag()) {
      case js::jit::CacheIROperand::Tag::Uninitialized: {
        return true;
      }
#define HANDLE_TRACE_WEAK_CASE(Name)                                          \
  case js::jit::CacheIROperand::Tag::Name: {                                  \
    return delegate_traceWeak<js::jit::CacheTypes<js::jit::CacheType::Name>>( \
        trc, &value->as##Name());                                             \
  }
        CACHE_IR_TYPES(HANDLE_TRACE_WEAK_CASE)
#undef HANDLE_TRACE_WEAK_CASE
    }
  }

  DELEGATE_GC(isValid, bool, const js::jit::CacheIROperand& value) {
    switch (value.tag()) {
      case js::jit::CacheIROperand::Tag::Uninitialized: {
        return true;
      }
#define HANDLE_IS_VALID_CASE(Name)                                          \
  case js::jit::CacheIROperand::Tag::Name: {                                \
    return delegate_isValid<js::jit::CacheTypes<js::jit::CacheType::Name>>( \
        value.as##Name());                                                  \
  }
        CACHE_IR_TYPES(HANDLE_IS_VALID_CASE)
#undef HANDLE_IS_VALID_CASE
    }
  }

#undef DELEGATE_GC
};

}  // namespace JS

namespace js {

#define DEFINE_SELF_METHOD(Wrapper, ...)                                \
  __VA_ARGS__ jit::CacheIROperand& self() __VA_ARGS__ {                 \
    return static_cast<__VA_ARGS__ Wrapper<jit::CacheIROperand>*>(this) \
        ->get();                                                        \
  }

#define DEFINE_TYPE_TEST(Name) \
  bool is##Name() const { return self().is##Name(); }

#define DEFINE_CONST_PRIMITIVE_GETTER(Name)                       \
  jit::CacheTypes<jit::CacheType::Name> const& as##Name() const { \
    return self().as##Name();                                     \
  }

#define DEFINE_MUTABLE_PRIMITIVE_GETTER(Name)         \
  jit::CacheTypes<jit::CacheType::Name>& as##Name() { \
    return self().as##Name();                         \
  }

#define DEFINE_CONST_GC_GETTER(Name)                                          \
  Handle<jit::CacheTypes<jit::CacheType::Name>> as##Name() const {            \
    return Handle<jit::CacheTypes<jit::CacheType::Name>>::fromMarkedLocation( \
        &self().as##Name());                                                  \
  }

#define DEFINE_MUTABLE_GC_GETTER(Name)                              \
  MutableHandle<jit::CacheTypes<jit::CacheType::Name>> as##Name() { \
    return MutableHandle<jit::CacheTypes<jit::CacheType::Name>>::   \
        fromMarkedLocation(&self().as##Name());                     \
  }

#define DEFINE_SETTER(Name)                                          \
  void set##Name(jit::CacheTypes<jit::CacheType::Name> const Name) { \
    self().set##Name(Name);                                          \
  }

#define DEFINE_CONST_METHODS                                    \
  jit::CacheIROperand::Tag tag() const { return self().tag(); } \
  uint64_t asBits() const { return self().asBits(); }           \
  bool isInitialized() const { return self().isInitialized(); } \
  jit::CacheType type() const { return self().type(); }         \
  CACHE_IR_TYPES(DEFINE_TYPE_TEST)                              \
  CACHE_IR_PRIMITIVE_TYPES(DEFINE_CONST_PRIMITIVE_GETTER)       \
  CACHE_IR_GC_TYPES(DEFINE_CONST_GC_GETTER)                     \
  bool isValLike() const { return self().isValLike(); }         \
  Value toVal() const { return self().toVal(); }                \
  bool isNumber() const { return self().isNumber(); }           \
  double toNumber() const { return self().toNumber(); }

#define DEFINE_MUTABLE_METHODS                              \
  CACHE_IR_PRIMITIVE_TYPES(DEFINE_MUTABLE_PRIMITIVE_GETTER) \
  CACHE_IR_GC_TYPES(DEFINE_MUTABLE_GC_GETTER)               \
  CACHE_IR_TYPES(DEFINE_SETTER)                             \
  template <typename T>                                     \
  void setNumber(const T t) {                               \
    self().setNumber(t);                                    \
  }

template <typename Wrapper>
class HandleBase<jit::CacheIROperand, Wrapper>
    : public WrappedPtrOperations<jit::CacheIROperand, Wrapper> {
  DEFINE_SELF_METHOD(Handle, const)

 public:
  DEFINE_CONST_METHODS
};

template <typename Wrapper>
class MutableHandleBase<jit::CacheIROperand, Wrapper>
    : public MutableWrappedPtrOperations<jit::CacheIROperand, Wrapper> {
  DEFINE_SELF_METHOD(MutableHandle, const)
  DEFINE_SELF_METHOD(MutableHandle)

 public:
  DEFINE_CONST_METHODS
  DEFINE_MUTABLE_METHODS
};

template <typename Wrapper>
class RootedBase<jit::CacheIROperand, Wrapper>
    : public MutableWrappedPtrOperations<jit::CacheIROperand, Wrapper> {
  DEFINE_SELF_METHOD(Rooted, const)
  DEFINE_SELF_METHOD(Rooted)

 public:
  DEFINE_CONST_METHODS
  DEFINE_MUTABLE_METHODS
};

#undef DEFINE_SELF_METHOD
#undef DEFINE_TYPE_TEST
#undef DEFINE_CONST_PRIMITIVE_GETTER
#undef DEFINE_MUTABLE_PRIMITIVE_GETTER
#undef DEFINE_CONST_GC_GETTER
#undef DEFINE_MUTABLE_GC_GETTER
#undef DEFINE_SETTER
#undef DEFINE_GC_SETTER
#undef DEFINE_CONST_METHODS
#undef DEFINE_MUTABLE_METHODS

namespace jit {

enum class CacheIRInterpreterResult {
  Return,
  Bailout,
  Exception,
  UnsupportedOp,
};

class CacheIRInterpreterBase {
 protected:
  CACHE_IR_INTERPRETER_BASE_GENERATED
};

class MOZ_RAII CacheIRInterpreter : private CacheIRInterpreterBase {
 public:
  CacheIRInterpreter(JSContext* cx, const CacheIRStubInfo& stubInfo,
                     uint8_t* const stubData)
      : cx_(cx),
        stubInfo_(stubInfo),
        stubData_(stubData),
        output_(cx),
        operands_(cx) {}

  [[nodiscard]] bool init() {
    return operands_.resize(stubInfo_.numOperandIds());
  }

  mozilla::Maybe<CacheIRInterpreterResult> interpret();

  Handle<CacheIROperand> output() const { return output_; }
  MutableHandle<CacheIROperand> output() { return &output_; }

  Handle<CacheIROperand> operand(OperandId id) const {
    return operands_[id.id()];
  }
  MutableHandle<CacheIROperand> operand(OperandId id) {
    return operands_[id.id()];
  }

#define DEFINE_PRIMITIVE_OPERAND_GETTERS(Name)                            \
  CacheTypes<CacheType::Name> const& getOperand(const Name##OperandId id) \
      const {                                                             \
    return operand(id).as##Name();                                        \
  }                                                                       \
  CacheTypes<CacheType::Name>& getOperand(const Name##OperandId id) {     \
    return operand(id).as##Name();                                        \
  }
  CACHE_IR_PRIMITIVE_TYPES(DEFINE_PRIMITIVE_OPERAND_GETTERS)
#undef DEFINE_PRIMITIVE_OPERAND_GETTERS

#define DEFINE_GC_OPERAND_GETTERS(Name)                                    \
  template <CacheType type = CacheType::Name,                              \
            typename = std::enable_if_t<type == CacheType::Name &&         \
                                        type != CacheType::Val>>           \
  Handle<CacheTypes<CacheType::Name>> getOperand(const Name##OperandId id) \
      const {                                                              \
    return operand(id).as##Name();                                         \
  }                                                                        \
  template <CacheType type = CacheType::Name,                              \
            typename = std::enable_if_t<type == CacheType::Name &&         \
                                        type != CacheType::Val>>           \
  MutableHandle<CacheTypes<CacheType::Name>> getOperand(                   \
      const Name##OperandId id) {                                          \
    return operand(id).as##Name();                                         \
  }
  CACHE_IR_GC_TYPES(DEFINE_GC_OPERAND_GETTERS)
#undef DEFINE_GC_OPERAND_GETTERS

  Value getOperand(const ValOperandId id) const { return operand(id).toVal(); }

#define DEFINE_OPERAND_SETTER(Name)                         \
  void setOperand(const Name##OperandId id,                 \
                  CacheTypes<CacheType::Name> const Name) { \
    operand(id).set##Name(Name);                            \
  }
  CACHE_IR_TYPES(DEFINE_OPERAND_SETTER)
#undef DEFINE_OPERAND_SETTER

  double getNumberOperand(const NumberOperandId id) const {
    return operand(id).toNumber();
  }
  template <typename T>
  void setNumberOperand(const NumberOperandId id, const T t) {
    operand(id).setNumber(t);
  }

 private:
#define BRING_BASE_INTERPRET_METHOD_INTO_SCOPE(op, ...) \
  using CacheIRInterpreterBase::interpret##op;
  CACHE_IR_OPS(BRING_BASE_INTERPRET_METHOD_INTO_SCOPE)
#undef BRING_BASE_INTERPRET_METHOD_INTO_SCOPE

#define CACHE_OP(op, ...) \
  mozilla::Maybe<CacheIRInterpreterResult> interpret##op(__VA_ARGS__)

  CACHE_OP(ReturnFromIC);
  CACHE_OP(GuardToObject, ValOperandId inputId);
  CACHE_OP(GuardIsNullOrUndefined, ValOperandId inputId);
  CACHE_OP(GuardIsNull, ValOperandId inputId);
  CACHE_OP(GuardIsUndefined, ValOperandId inputId);
  CACHE_OP(GuardToBoolean, ValOperandId inputId);
  CACHE_OP(GuardToString, ValOperandId inputId);
  CACHE_OP(GuardToSymbol, ValOperandId inputId);
  CACHE_OP(GuardToBigInt, ValOperandId inputId);
  CACHE_OP(GuardIsNumber, ValOperandId inputId);
  CACHE_OP(GuardToInt32, ValOperandId inputId);
  CACHE_OP(GuardBooleanToInt32, ValOperandId inputId, Int32OperandId resultId);
  CACHE_OP(GuardToInt32Index, ValOperandId inputId, Int32OperandId resultId);
  CACHE_OP(Int32ToIntPtr, Int32OperandId inputId, IntPtrOperandId resultId);
  CACHE_OP(GuardNumberToIntPtrIndex, NumberOperandId inputId, bool supportOOB,
           IntPtrOperandId resultId);
  CACHE_OP(GuardToInt32ModUint32, ValOperandId inputId,
           Int32OperandId resultId);
  CACHE_OP(GuardToUint8Clamped, ValOperandId inputId, Int32OperandId resultId);
  CACHE_OP(GuardNonDoubleType, ValOperandId inputId, ValueType type);
  CACHE_OP(GuardShape, ObjOperandId objId, uint32_t shapeOffset);
  CACHE_OP(GuardProto, ObjOperandId objId, uint32_t protoOffset);
  CACHE_OP(GuardNullProto, ObjOperandId objId);
  CACHE_OP(GuardClass, ObjOperandId objId, GuardClassKind kind);
  CACHE_OP(GuardAnyClass, ObjOperandId objId, uint32_t claspOffset);
  CACHE_OP(HasClassResult, ObjOperandId objId, uint32_t claspOffset);
  CACHE_OP(CallRegExpMatcherResult, ObjOperandId regexpId,
           StringOperandId inputId, Int32OperandId lastIndexId);
  CACHE_OP(CallRegExpSearcherResult, ObjOperandId regexpId,
           StringOperandId inputId, Int32OperandId lastIndexId);
  CACHE_OP(CallRegExpTesterResult, ObjOperandId regexpId,
           StringOperandId inputId, Int32OperandId lastIndexId);
  CACHE_OP(RegExpFlagResult, ObjOperandId regexpId, int32_t flagsMask);
  CACHE_OP(CallSubstringKernelResult, StringOperandId strId,
           Int32OperandId beginId, Int32OperandId lengthId);
  CACHE_OP(GuardDynamicSlotValue, ObjOperandId objId, uint32_t offsetOffset,
           uint32_t valOffset);
  CACHE_OP(LoadFixedSlotResult, ObjOperandId objId, uint32_t offsetOffset);
  CACHE_OP(LoadDynamicSlotResult, ObjOperandId objId, uint32_t offsetOffset);

#undef CACHE_OP

  CACHE_IR_INTERPRETER_DISPATCH_GENERATED

  int32_t getInt32Field(uint32_t offset) const {
    return stubInfo_.getStubRawWord(stubData_, offset);
  }
  Shape* getShapeField(uint32_t offset) const {
    return stubInfo_.getStubField<Shape*>(stubData_, offset).get();
  }
  JSObject* getObjectField(uint32_t offset) const {
    return stubInfo_.getStubField<JSObject*>(stubData_, offset).get();
  }
  Value getValueField(uint32_t offset) const {
    return stubInfo_.getStubField<Value>(stubData_, offset).get();
  }
  const JSClass* getClassField(uintptr_t offset) const {
    return (const JSClass*)stubInfo_.getStubRawWord(stubData_, offset);
  }

  const Value& LoadDynamicSlot(const ObjOperandId objId,
                               const uint32_t offsetOffset) const;

  JSContext* const cx_;
  const CacheIRStubInfo& stubInfo_;
  uint8_t* const stubData_;

  Rooted<CacheIROperand> output_;
  JS::RootedVector<CacheIROperand> operands_;
};

}  // namespace jit
}  // namespace js

#endif /* jit_CacheIRInterpreter_h */
