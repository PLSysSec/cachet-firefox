/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/CacheIRSpewer.h"

#include "mozilla/Sprintf.h"
#include "mozilla/TextUtils.h"

#include <algorithm>
#include <ctype.h>
#include <dirent.h>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>

#include "jsmath.h"

#include "gc/GC.h"  // js::gc::AutoSuppressGC
#include "jit/CacheIRCompiler.h"
#include "jit/JitSpewer.h"
#include "js/HashTable.h"
#include "js/ScalarType.h"  // js::Scalar::Type
#include "util/GetPidProvider.h"
#include "util/Text.h"
#include "vm/JSFunction.h"
#include "vm/JSObject.h"
#include "vm/JSScript.h"

#include "vm/JSObject-inl.h"
#include "vm/Realm-inl.h"

namespace filesystem = std::experimental::filesystem;

using namespace js;
using namespace js::jit;
using namespace mozilla;

#if defined(JS_CACHEIR_SPEW) || !defined(JS_DISABLE_SHELL)

// Text spewer for CacheIR ops that can be used with JitSpew.
// Output looks like this:
//
//   GuardToInt32       [ValId input] 0
//   GuardToInt32       [ValId input] 1
//   CompareInt32Result [JSOpImm op] Lt, [Int32Id lhs] 0, [Int32Id rhs] 1
//   ReturnFromIC
class MOZ_RAII CacheIROpsJitSpewer {
  GenericPrinter& out_;

  // String prepended to each line. Can be used for indentation.
  const char* prefix_;

  CACHE_IR_SPEWER_GENERATED

  void spewOp(CacheOp op) {
    const char* opName = CacheIROpNames[size_t(op)];
    out_.printf("%s%-29s ", prefix_, opName);
  }
  void spewOpEnd() { out_.printf("\n"); }

  void spewArgSeparator() { out_.printf(", "); }

  void spewOperandId(const char* name, const char* type, OperandId id) {
    spewRawOperandId(name, type, id.id());
  }
  void spewRawOperandId(const char* name, const char* type, uint16_t id) {
    out_.printf("[%s %s] %u", type, name, id);
  }
  void spewField(const char* name, const char* type, uint32_t offset) {
    out_.printf("[%s %s] %u", type, name, offset);
  }
  void spewBoolImm(const char* name, const char* type, bool b) {
    out_.printf("[%s %s] %s", type, name, b ? "true" : "false");
  }
  void spewByteImm(const char* name, const char* type, uint8_t val) {
    out_.printf("[%s %s] %u", type, name, val);
  }
  void spewJSOpImm(const char* name, const char* type, JSOp op) {
    out_.printf("[%s %s] %s", type, name, CodeName(op));
  }
  void spewStaticStringImm(const char* name, const char* type,
                           const char* str) {
    out_.printf("[%s %s] \"%s\"", type, name, str);
  }
  void spewInt32Imm(const char* name, const char* type, int32_t val) {
    out_.printf("[%s %s] %d", type, name, val);
  }
  void spewUInt32Imm(const char* name, const char* type, uint32_t val) {
    out_.printf("[%s %s] %u", type, name, val);
  }
  void spewCallFlagsImm(const char* name, const char* type, CallFlags flags) {
    out_.printf(
        "[%s %s] %s%s%s%s", type, name,
        CallFlags::GetArgFormatName(flags.getArgFormat()),
        flags.isConstructing() ? " + isConstructing" : "",
        flags.isSameRealm() ? " + isSameRealm" : "",
        flags.needsUninitializedThis() ? " + needsUninitializedThis" : "");
  }
  void spewJSWhyMagicImm(const char* name, const char* type, JSWhyMagic magic) {
    out_.printf("[%s %s] %s", type, name, JS::GetJSWhyMagicName(magic));
  }
  void spewScalarTypeImm(const char* name, const char* type,
                         Scalar::Type scalarType) {
    out_.printf("[%s %s] %s", type, name, Scalar::name(scalarType));
  }
  void spewUnaryMathFunctionImm(const char* name, const char* type,
                                UnaryMathFunction fun) {
    out_.printf("[%s %s] %s", type, name, GetUnaryMathFunctionName(fun));
  }
  void spewValueTypeImm(const char* name, const char* type,
                        ValueType valueType) {
    out_.printf("[%s %s] %s", type, name, ValTypeToString(valueType));
  }
  void spewJSNativeImm(const char* name, const char* type, JSNative native) {
    out_.printf("[%s %s] %p", type, name, native);
  }
  void spewGuardClassKindImm(const char* name, const char* type,
                             GuardClassKind kind) {
    out_.printf("[%s %s] %s", type, name, GetGuardClassKindName(kind));
  }
  void spewWasmValTypeImm(const char* name, const char* type,
                          wasm::ValType::Kind kind) {
    out_.printf("[%s %s] %s", type, name, wasm::ValType::kindName(kind));
  }
  void spewAllocKindImm(const char* name, const char* type,
                        gc::AllocKind kind) {
    out_.printf("[%s %s] %s", type, name, gc::AllocKindName(kind));
  }

 public:
  CacheIROpsJitSpewer(GenericPrinter& out, const char* prefix)
      : out_(out), prefix_(prefix) {}

  void spew(CacheIRReader& reader) {
    do {
      switch (reader.readOp()) {
#  define SPEW_OP(op, ...) \
    case CacheOp::op:      \
      spew##op(reader);    \
      break;
        CACHE_IR_OPS(SPEW_OP)
#  undef SPEW_OP

        default:
          MOZ_CRASH("Invalid op");
      }
    } while (reader.more());
  }
};

void jit::SpewCacheIROps(GenericPrinter& out, const char* const prefix,
                         const CacheIRStubInfo* const stubInfo) {
  CacheIRReader reader(stubInfo);
  CacheIROpsJitSpewer spewer(out, prefix);
  spewer.spew(reader);
}

bool jit::SpewCacheIRStubFields(JSContext* const cx, GenericPrinter& out,
                                const CacheIRStubInfo* const stubInfo,
                                uint8_t* const stubData) {
  gc::AutoSuppressGC nogc(cx);

  uint32_t field(0);
  size_t offset(0);

  HashSet<Shape*> shapes(cx);
  HashSet<BaseShape*> baseShapes(cx);
  HashSet<const JSClass*> classes(cx);
  HashSet<JSObject*> taggedProtos(cx);
  HashSet<JSString*> strings(cx);

  while (true) {
    const StubField::Type fieldType(stubInfo->fieldType(field));
    if (fieldType == StubField::Type::Limit) {
      break;  // Done.
    }

    const char* const fieldTypeName(StubField::TypeNames[uint8_t(fieldType)]);
    out.printf("%sField%*s %10zu, ", fieldTypeName,
               12 - int(strlen(fieldTypeName)), "", offset);
    switch (fieldType) {
      case StubField::Type::RawInt32: {
        out.printf("%i\n", int(stubInfo->getStubRawWord(stubData, offset)));
        break;
      }
      case StubField::Type::RawPointer: {
        out.printf("%" PRIxPTR "\n",
                   stubInfo->getStubRawWord(stubData, offset));
        break;
      }
      case StubField::Type::Shape: {
        GCPtrShape& shapeField =
            stubInfo->getStubField<Shape*>(stubData, offset);
        Shape* const shape(shapeField.get());
        out.printf("%p\n", shape);
        auto ptr = shapes.lookupForAdd(shape);
        if (!ptr) {
          if (!shapes.add(ptr, shape)) {
            return false;
          }
        }
        break;
      }
      case StubField::Type::GetterSetter: {
        GCPtrGetterSetter& getterSetterField =
            stubInfo->getStubField<GetterSetter*>(stubData, offset);
        out.printf("%p\n", getterSetterField.get());
        break;
      }
      case StubField::Type::Object: {
        GCPtrObject& objectField =
            stubInfo->getStubField<JSObject*>(stubData, offset);
        out.printf("%p\n", objectField.get());
        break;
      }
      case StubField::Type::Symbol: {
        GCPtr<JS::Symbol*>& symbolField =
            stubInfo->getStubField<JS::Symbol*>(stubData, offset);
        out.printf("%p\n", symbolField.get());
        break;
      }
      case StubField::Type::String: {
        GCPtrString& stringField =
            stubInfo->getStubField<JSString*>(stubData, offset);
	JSString* const string(stringField.get());
        out.printf("%p\n", string);
        auto ptr = strings.lookupForAdd(string);
        if (!ptr) {
          if (!strings.add(ptr, string)) {
            return false;
          }
        }
        break;
      }
      case StubField::Type::BaseScript: {
        GCPtr<BaseScript*>& baseScriptField =
            stubInfo->getStubField<BaseScript*>(stubData, offset);
        out.printf("%p\n", baseScriptField.get());
        break;
      }
      case StubField::Type::Id: {
        GCPtrId& idField = stubInfo->getStubField<jsid>(stubData, offset);
        out.printf("%p\n", (void*)JSID_BITS(idField.get()));
        break;
      }
      case StubField::Type::AllocSite: {
        out.printf("%p\n",
                   stubInfo->getPtrStubField<gc::AllocSite>(stubData, offset));
        break;
      }
      case StubField::Type::RawInt64: {
        out.printf("%" PRIi64 "\n",
                   stubInfo->getStubRawInt64(stubData, offset));
        break;
      }
      case StubField::Type::Value: {
        GCPtrValue& valueField =
            stubInfo->getStubField<JS::Value>(stubData, offset);
        out.printf("%016" PRIx64 "\n", valueField.get().asRawBits());
        break;
      }
      case StubField::Type::Limit: {
        MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE(
            "StubField::Type::Limit is handled before the switch");
      }
    }

    field++;
    offset += StubField::sizeInBytes(fieldType);
  }

  out.printf("%%\n");

  for (auto iter = shapes.iter(); !iter.done(); iter.next()) {
    Shape* const shape(iter.get());
    out.putChar('\n');

    BaseShape* const baseShape(shape->base());
    {
      auto ptr = baseShapes.lookupForAdd(baseShape);
      if (!ptr) {
        if (!baseShapes.add(ptr, baseShape)) {
          return false;
        }
      }
    }
    out.printf("ShapeBase                     %p, %p\n", shape, baseShape);

    const JSClass* const klass(shape->getObjectClass());
    {
      auto ptr = classes.lookupForAdd(klass);
      if (!ptr) {
        if (!classes.add(ptr, klass)) {
          return false;
        }
      }
    }
    out.printf("ShapeClass                    %p, %p\n", shape, klass);

    out.printf("ShapeNumFixedSlots            %p, %" PRIu32 "\n", shape,
               shape->numFixedSlots());
    out.printf("ShapeSlotSpan                 %p, %" PRIu32 "\n", shape,
               shape->slotSpan());
  }

  for (auto iter = baseShapes.iter(); !iter.done(); iter.next()) {
    BaseShape* const baseShape(iter.get());
    out.putChar('\n');

    const TaggedProto taggedProto(baseShape->proto());
    JSObject* const rawTaggedProto(taggedProto.raw());
    {
      auto ptr = taggedProtos.lookupForAdd(rawTaggedProto);
      if (!ptr) {
        if (!taggedProtos.add(ptr, rawTaggedProto)) {
          return false;
        }
      }
    }
    out.printf("BaseShapeTaggedProto          %p, %p\n", baseShape,
               rawTaggedProto);
  }

  for (auto iter = classes.iter(); !iter.done(); iter.next()) {
    const JSClass* const klass(iter.get());
    out.putChar('\n');

    if (klass->isNativeObject()) {
      out.printf("ClassIsNativeObject           %p\n", klass);
    }
  }

  for (auto iter = taggedProtos.iter(); !iter.done(); iter.next()) {
    JSObject* const rawTaggedProto(iter.get());
    const TaggedProto taggedProto(rawTaggedProto);
    out.putChar('\n');

    if (taggedProto.isObject()) {
      out.printf("TaggedProtoIsObject           %p\n", rawTaggedProto);
    } else if (taggedProto.isDynamic()) {
      out.printf("TaggedProtoIsLazy             %p\n", rawTaggedProto);
    } else {
      MOZ_ASSERT(rawTaggedProto == nullptr);
      out.printf("TaggedProtoIsNull             %p\n", rawTaggedProto);
    }
  }

  for (auto iter = strings.iter(); !iter.done(); iter.next()) {
    JSString* const string(iter.get());
    out.putChar('\n');

    if (string->isAtom()) {
      out.printf("StringIsAtom                  %p\n", string);
    }
  }

  return true;
}

bool jit::SpewCacheIRStub(JSContext* const cx, GenericPrinter& out,
                          const CacheIRStubInfo* const stubInfo,
                          uint8_t* const stubData) {
  const CacheKind kind(stubInfo->kind());
  const ICStubEngine engine(stubInfo->engine());
  out.printf("%s %s\n", GetCacheKindName(kind), GetICStubEngineName(engine));

  const CacheKindSignature& sig(GetCacheKindSignature(kind));
  for (uint16_t inputIndex = 0; inputIndex < sig.numInputs; ++inputIndex) {
    const CacheKindSignature::Input input(sig.inputs[inputIndex]);
    out.printf("Input                         [%sId %s] %u\n",
               GetCacheTypeName(input.type), input.name, inputIndex);
  }
  if (sig.output) {
    const MIRType outputType(MIRTypeFromCacheType(*sig.output));
    out.printf("Output                        %s\n", StringFromMIRType(outputType));
  }
  out.printf("%%\n\n");

  SpewCacheIROps(out, "", stubInfo);
  out.printf("%%\n\n");
  if (!SpewCacheIRStubFields(cx, out, stubInfo, stubData)) {
    return false;
  }

  return true;
}

#endif /* JS_CACHEIR_SPEW || !JS_DISABLE_SHELL */

#ifdef JS_CACHEIR_SPEW

static bool StringStartsWith(const char* const prefix, const char* const str) {
  return strncmp(prefix, str, strlen(prefix)) == 0;
}

void jit::SpewCacheIRStubToFile(JSContext* cx, JSScript* const script,
                                const CacheIRStubInfo* const stubInfo,
                                const CacheIRStubKey::Lookup& lookup,
                                uint8_t* const stubData) {
  if (JitOptions.cacheIrSpewDirPath.empty()) {
    return;
  }

  const char* const scriptFileName(script->filename());

  // Don't dump stubs from internal scripts.
  if (strcmp("self-hosted", scriptFileName) == 0 ||
      StringStartsWith("about:", scriptFileName) ||
      StringStartsWith("chrome://", scriptFileName) ||
      StringStartsWith("file://", scriptFileName) ||
      StringStartsWith("moz-extension://", scriptFileName) ||
      StringStartsWith("resource://", scriptFileName)) {
    return;
  }

  // Create a stub-dumping directory for the engine and cache kind.
  static const filesystem::path stubsDirName("stubs");
  const filesystem::path stubsDirPath(JitOptions.cacheIrSpewDirPath / stubsDirName);
  const filesystem::path icStubEngineDirName(GetICStubEngineName(stubInfo->engine()));
  const filesystem::path cacheKindDirName(GetCacheKindName(stubInfo->kind()));
  const filesystem::path cacheKindDirRelPath(icStubEngineDirName / cacheKindDirName);
  filesystem::create_directories(stubsDirPath / cacheKindDirRelPath);

  // Generate a file name for the dumped CacheIR stub.
  std::stringstream stubFileNameStream;
  HashNumber hash(CacheIRStubKey::hash(lookup));
  //hash = AddToHash(hash, HashBytes(stubData, stubInfo->stubDataSize()));
  stubFileNameStream << hash << ".cacheir";
  const filesystem::path stubFileName(stubFileNameStream.str());

  // Dump the stub if it hasn't been dumped already.
  const filesystem::path stubFileRelPath(cacheKindDirRelPath / stubFileName);
  const filesystem::path stubFilePath(stubsDirPath / stubFileRelPath);
  FILE* const stubFile(fopen(stubFilePath.c_str(), "wx"));
  if (stubFile) {
    Fprinter stubFilePrinter(stubFile);
    SpewCacheIRStub(cx, stubFilePrinter, stubInfo, stubData);
    stubFilePrinter.flush();
    stubFilePrinter.finish();
  }

  // Generate a path-safe directory name from the script file name.
  std::string scriptFilteredFileName(scriptFileName);
  std::replace_if(scriptFilteredFileName.begin(), scriptFilteredFileName.end(), [](char c) {
    return !(isalnum(c) || c == '.' || c == '-' || c == '_');
  }, '-');
  std::stringstream scriptDirNameStream;
  scriptDirNameStream << scriptFilteredFileName << '-' << HashString(scriptFileName);
  const filesystem::path scriptDirName(scriptDirNameStream.str());
  static const filesystem::path scriptsDirName("scripts");
  const filesystem::path scriptsDirPath(JitOptions.cacheIrSpewDirPath / scriptsDirName);
  const filesystem::path scriptDirPath(scriptsDirPath / scriptDirName);

  // Save the script source code, if available.
  /*
  RootedString scriptSource(cx);
  RootedFunction scriptFunc(cx, script->function());
  if (scriptFunc) {
    scriptSource = FunctionToString(cx, scriptFunc, false);
  } else {
    bool haveSource;
    if (ScriptSource::loadSource(cx, script->scriptSource(), &haveSource) && haveSource) {
      RootedScript rootedScript(cx, script);
      scriptSource = JSScript::sourceData(cx, rootedScript);
    }
  }
  if (scriptSource) {
    Sprinter scriptSourceBuf(cx);
    if (scriptSourceBuf.init() && scriptSourceBuf.putString(scriptSource)) {
      filesystem::create_directories(scriptDirPath);
      const filesystem::path scriptSourceFilePath(scriptDirPath / filesystem::path("source.js"));
      FILE* const scriptSourceFile(fopen(scriptSourceFilePath.c_str(), "wx"));
      if (scriptSourceFile) {
        fputs(scriptSourceBuf.string(), scriptSourceFile);
        fclose(scriptSourceFile);
      }
    }
  }
  */

  // Create a symbolic link associating the stub file with the script.
  const filesystem::path scriptCacheKindDirPath(scriptDirPath / stubsDirName / cacheKindDirRelPath);
  filesystem::create_directories(scriptCacheKindDirPath);
  const filesystem::path scriptStubLinkPath(scriptCacheKindDirPath / stubFileName);
  static const filesystem::path stubsDirFromScriptCacheKindDirRelPath(
      filesystem::path("..") / filesystem::path("..") / filesystem::path("..") /
      filesystem::path("..") / filesystem::path("..") / stubsDirName);
  std::error_code ignoredError;
  filesystem::create_symlink(stubsDirFromScriptCacheKindDirRelPath /
                             stubFileRelPath, scriptStubLinkPath, ignoredError);
}

template <typename CharT>
static void QuoteString(GenericPrinter& out, const CharT* s, size_t length) {
  const CharT* end = s + length;
  for (const CharT* t = s; t < end; s = ++t) {
    // This quote implementation is probably correct,
    // but uses \u even when not strictly necessary.
    char16_t c = *t;
    if (c == '"' || c == '\\') {
      out.printf("\\");
      out.printf("%c", char(c));
    } else if (!IsAsciiPrintable(c)) {
      out.printf("\\u%04x", c);
    } else {
      out.printf("%c", char(c));
    }
  }
}

static void QuoteString(GenericPrinter& out, JSLinearString* str) {
  JS::AutoCheckCannotGC nogc;

  // Limit the string length to reduce the JSON file size.
  size_t length = std::min(str->length(), size_t(128));
  if (str->hasLatin1Chars()) {
    QuoteString(out, str->latin1Chars(nogc), length);
  } else {
    QuoteString(out, str->twoByteChars(nogc), length);
  }
}

// JSON spewer for CacheIR ops. Output looks like this:
//
// ...
//    {
//      "op":"GuardToInt32",
//      "args":[
//        {
//          "name":"input",
//          "type":"Id",
//          "value":0
//        }
//      ]
//    },
//    {
//      "op":"Int32IncResult",
//      "args":[
//        {
//          "name":"input",
//          "type":"Id",
//          "value":0
//        }
//      ]
//    }
// ...
class MOZ_RAII CacheIROpsJSONSpewer {
  JSONPrinter& j_;

  CACHE_IR_SPEWER_GENERATED

  void spewOp(CacheOp op) {
    const char* opName = CacheIROpNames[size_t(op)];
    j_.beginObject();
    j_.property("op", opName);
    j_.beginListProperty("args");
  }
  void spewOpEnd() {
    j_.endList();
    j_.endObject();
  }

  void spewArgSeparator() {}

  template <typename T>
  void spewArgImpl(const char* name, const char* type, T value) {
    j_.beginObject();
    j_.property("type", type);
    j_.property("name", name);
    j_.property("value", value);
    j_.endObject();
  }

  void spewOperandId(const char* name, const char* type, OperandId id) {
    spewRawOperandId(name, type, id.id());
  }
  void spewRawOperandId(const char* name, const char* type, uint16_t id) {
    spewArgImpl(name, "Id", id);
  }
  void spewField(const char* name, const char* type, uint32_t offset) {
    spewArgImpl(name, "Field", offset);
  }
  void spewBoolImm(const char* name, const char* type, bool b) {
    spewArgImpl(name, "Imm", b);
  }
  void spewByteImm(const char* name, const char* type, uint8_t val) {
    spewArgImpl(name, "Imm", val);
  }
  void spewJSOpImm(const char* name, const char* type, JSOp op) {
    spewArgImpl(name, "JSOp", CodeName(op));
  }
  void spewStaticStringImm(const char* name, const char* type,
                           const char* str) {
    spewArgImpl(name, "String", str);
  }
  void spewInt32Imm(const char* name, const char* type, int32_t val) {
    spewArgImpl(name, "Imm", val);
  }
  void spewUInt32Imm(const char* name, const char* type, uint32_t val) {
    spewArgImpl(name, "Imm", val);
  }
  void spewCallFlagsImm(const char* name, const char* type, CallFlags flags) {
    spewArgImpl(name, "Imm", flags.toByte());
  }
  void spewJSWhyMagicImm(const char* name, const char* type, JSWhyMagic magic) {
    spewArgImpl(name, "Imm", JS::GetJSWhyMagicName(magic));
  }
  void spewScalarTypeImm(const char* name, const char* type,
                         Scalar::Type scalarType) {
    spewArgImpl(name, "Imm", Scalar::name(scalarType));
  }
  void spewUnaryMathFunctionImm(const char* name, const char* type,
                                UnaryMathFunction fun) {
    const char* funName = GetUnaryMathFunctionName(fun);
    spewArgImpl(name, "MathFunction", funName);
  }
  void spewValueTypeImm(const char* name, const char* type,
                        ValueType valueType) {
    spewArgImpl(name, "Imm", ValTypeToString(valueType));
  }
  void spewJSNativeImm(const char* name, const char* type, JSNative native) {
    spewArgImpl(name, "Word", uintptr_t(native));
  }
  void spewGuardClassKindImm(const char* name, const char* type,
                             GuardClassKind kind) {
    spewArgImpl(name, "Imm", GetGuardClassKindName(kind));
  }
  void spewWasmValTypeImm(const char* name, const char* type,
                          wasm::ValType::Kind kind) {
    spewArgImpl(name, "Imm", wasm::ValType::kindName(kind));
  }
  void spewAllocKindImm(const char* name, const char* type,
                        gc::AllocKind kind) {
    spewArgImpl(name, "Imm", gc::AllocKindName(kind));
  }

 public:
  explicit CacheIROpsJSONSpewer(JSONPrinter& j) : j_(j) {}

  void spew(CacheIRReader& reader) {
    do {
      switch (reader.readOp()) {
#  define SPEW_OP(op, ...) \
    case CacheOp::op:      \
      spew##op(reader);    \
      break;
        CACHE_IR_OPS(SPEW_OP)
#  undef SPEW_OP

        default:
          MOZ_CRASH("Invalid op");
      }
    } while (reader.more());
  }
};

CacheIRSpewer CacheIRSpewer::cacheIRspewer;

CacheIRSpewer::CacheIRSpewer()
    : outputLock_(mutexid::CacheIRSpewer), guardCount_(0) {
 spewInterval_ =
      getenv("CACHEIR_LOG_FLUSH") ? atoi(getenv("CACHEIR_LOG_FLUSH")) : 10000;

  if (spewInterval_ < 1) {
    spewInterval_ = 1;
  }
}

CacheIRSpewer::~CacheIRSpewer() {
  if (!enabled()) {
    return;
  }

  json_.ref().endList();
  output_.flush();
  output_.finish();
}

#  ifndef JIT_SPEW_DIR
#    if defined(_WIN32)
#      define JIT_SPEW_DIR "."
#    elif defined(__ANDROID__)
#      define JIT_SPEW_DIR "/data/local/tmp"
#    else
#      define JIT_SPEW_DIR "/tmp"
#    endif
#  endif

bool CacheIRSpewer::init(const char* filename) {
  if (enabled()) {
    return true;
  }

  char name[256];
  uint32_t pid = getpid();
  // Default to JIT_SPEW_DIR/cacheir${pid}.json
  if (filename[0] == '1') {
    SprintfLiteral(name, JIT_SPEW_DIR "/cacheir%" PRIu32 ".json", pid);
  } else {
    SprintfLiteral(name, "%s%" PRIu32 ".json", filename, pid);
  }

  if (!output_.init(name)) {
    return false;
  }

  json_.emplace(output_);
  json_->beginList();
  return true;
}

void CacheIRSpewer::beginCache(const IRGenerator& gen) {
  MOZ_ASSERT(enabled());
  JSONPrinter& j = json_.ref();
  const char* filename = gen.script_->filename();
  j.beginObject();
  j.property("name", CacheKindNames[uint8_t(gen.cacheKind_)]);
  j.property("file", filename ? filename : "null");
  j.property("mode", int(gen.mode_));
  if (jsbytecode* pc = gen.pc_) {
    unsigned column;
    j.property("line", PCToLineNumber(gen.script_, pc, &column));
    j.property("column", column);
    j.formatProperty("pc", "%p", pc);
  }
}

void CacheIRSpewer::valueProperty(const char* name, const Value& v) {
  MOZ_ASSERT(enabled());
  JSONPrinter& j = json_.ref();

  j.beginObjectProperty(name);

  const char* type = InformalValueTypeName(v);
  if (v.isInt32()) {
    type = "int32";
  }
  j.property("type", type);

  if (v.isInt32()) {
    j.property("value", v.toInt32());
  } else if (v.isDouble()) {
    j.floatProperty("value", v.toDouble(), 3);
  } else if (v.isString() || v.isSymbol()) {
    JSString* str = v.isString() ? v.toString() : v.toSymbol()->description();
    if (str && str->isLinear()) {
      j.beginStringProperty("value");
      QuoteString(output_, &str->asLinear());
      j.endStringProperty();
    }
  } else if (v.isObject()) {
    JSObject& object = v.toObject();
    j.formatProperty("value", "%p (shape: %p)", &object, object.shape());

    if (object.is<JSFunction>()) {
      if (JSAtom* name = object.as<JSFunction>().displayAtom()) {
        j.beginStringProperty("funName");
        QuoteString(output_, name);
        j.endStringProperty();
      }
    }

    if (NativeObject* nobj =
            object.is<NativeObject>() ? &object.as<NativeObject>() : nullptr) {
      j.beginListProperty("flags");
      {
        if (nobj->isIndexed()) {
          j.value("indexed");
        }
        if (nobj->inDictionaryMode()) {
          j.value("dictionaryMode");
        }
      }
      j.endList();
      if (nobj->isIndexed()) {
        j.beginObjectProperty("indexed");
        {
          j.property("denseInitializedLength",
                     nobj->getDenseInitializedLength());
          j.property("denseCapacity", nobj->getDenseCapacity());
          j.property("denseElementsAreSealed", nobj->denseElementsAreSealed());
          j.property("denseElementsAreFrozen", nobj->denseElementsAreFrozen());
        }
        j.endObject();
      }
    }
  }

  j.endObject();
}

void CacheIRSpewer::opcodeProperty(const char* name, const JSOp op) {
  MOZ_ASSERT(enabled());
  JSONPrinter& j = json_.ref();

  j.beginStringProperty(name);
  output_.put(CodeName(op));
  j.endStringProperty();
}

void CacheIRSpewer::cacheIRSequence(CacheIRReader& reader) {
  MOZ_ASSERT(enabled());
  JSONPrinter& j = json_.ref();

  j.beginListProperty("cacheIR");

  CacheIROpsJSONSpewer spewer(j);
  spewer.spew(reader);

  j.endList();
}

void CacheIRSpewer::attached(const char* name) {
  MOZ_ASSERT(enabled());
  json_.ref().property("attached", name);
}

void CacheIRSpewer::endCache() {
  MOZ_ASSERT(enabled());
  json_.ref().endObject();
}

#endif /* JS_CACHEIR_SPEW */
