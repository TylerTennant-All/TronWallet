// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: core/TronInventoryItems.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "core/TronInventoryItems.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace protocol {
class InventoryItemsDefaultTypeInternal {
public:
 ::google::protobuf::internal::ExplicitlyConstructed<InventoryItems>
     _instance;
} _InventoryItems_default_instance_;

namespace protobuf_core_2fTronInventoryItems_2eproto {


namespace {

::google::protobuf::Metadata file_level_metadata[1];

}  // namespace

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTableField
    const TableStruct::entries[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  {0, 0, 0, ::google::protobuf::internal::kInvalidMask, 0, 0},
};

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::AuxillaryParseTableField
    const TableStruct::aux[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ::google::protobuf::internal::AuxillaryParseTableField(),
};
PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTable const
    TableStruct::schema[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { NULL, NULL, 0, -1, -1, -1, -1, NULL, false },
};

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(InventoryItems, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(InventoryItems, type_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(InventoryItems, items_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(InventoryItems)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&_InventoryItems_default_instance_),
};

namespace {

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "core/TronInventoryItems.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

}  // namespace
void TableStruct::InitDefaultsImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::InitProtobufDefaults();
  _InventoryItems_default_instance_._instance.DefaultConstruct();
  ::google::protobuf::internal::OnShutdownDestroyMessage(
      &_InventoryItems_default_instance_);}

void InitDefaults() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &TableStruct::InitDefaultsImpl);
}
namespace {
void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\035core/TronInventoryItems.proto\022\010protoco"
      "l\"-\n\016InventoryItems\022\014\n\004type\030\001 \001(\005\022\r\n\005ite"
      "ms\030\002 \003(\014BP\n\017org.tron.protosB\022TronInvento"
      "ryItemsZ)github.com/tronprotocol/grpc-ga"
      "teway/coreb\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 178);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "core/TronInventoryItems.proto", &protobuf_RegisterTypes);
}
} // anonymous namespace

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;

}  // namespace protobuf_core_2fTronInventoryItems_2eproto


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int InventoryItems::kTypeFieldNumber;
const int InventoryItems::kItemsFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

InventoryItems::InventoryItems()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    protobuf_core_2fTronInventoryItems_2eproto::InitDefaults();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:protocol.InventoryItems)
}
InventoryItems::InventoryItems(const InventoryItems& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      items_(from.items_),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  type_ = from.type_;
  // @@protoc_insertion_point(copy_constructor:protocol.InventoryItems)
}

void InventoryItems::SharedCtor() {
  type_ = 0;
  _cached_size_ = 0;
}

InventoryItems::~InventoryItems() {
  // @@protoc_insertion_point(destructor:protocol.InventoryItems)
  SharedDtor();
}

void InventoryItems::SharedDtor() {
}

void InventoryItems::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* InventoryItems::descriptor() {
  protobuf_core_2fTronInventoryItems_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_core_2fTronInventoryItems_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const InventoryItems& InventoryItems::default_instance() {
  protobuf_core_2fTronInventoryItems_2eproto::InitDefaults();
  return *internal_default_instance();
}

InventoryItems* InventoryItems::New(::google::protobuf::Arena* arena) const {
  InventoryItems* n = new InventoryItems;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void InventoryItems::Clear() {
// @@protoc_insertion_point(message_clear_start:protocol.InventoryItems)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  items_.Clear();
  type_ = 0;
  _internal_metadata_.Clear();
}

bool InventoryItems::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:protocol.InventoryItems)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 type = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &type_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated bytes items = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u /* 18 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->add_items()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:protocol.InventoryItems)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:protocol.InventoryItems)
  return false;
#undef DO_
}

void InventoryItems::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:protocol.InventoryItems)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 type = 1;
  if (this->type() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->type(), output);
  }

  // repeated bytes items = 2;
  for (int i = 0, n = this->items_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      2, this->items(i), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:protocol.InventoryItems)
}

::google::protobuf::uint8* InventoryItems::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:protocol.InventoryItems)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 type = 1;
  if (this->type() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->type(), target);
  }

  // repeated bytes items = 2;
  for (int i = 0, n = this->items_size(); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteBytesToArray(2, this->items(i), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:protocol.InventoryItems)
  return target;
}

size_t InventoryItems::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:protocol.InventoryItems)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // repeated bytes items = 2;
  total_size += 1 *
      ::google::protobuf::internal::FromIntSize(this->items_size());
  for (int i = 0, n = this->items_size(); i < n; i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::BytesSize(
      this->items(i));
  }

  // int32 type = 1;
  if (this->type() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->type());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void InventoryItems::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:protocol.InventoryItems)
  GOOGLE_DCHECK_NE(&from, this);
  const InventoryItems* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const InventoryItems>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:protocol.InventoryItems)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:protocol.InventoryItems)
    MergeFrom(*source);
  }
}

void InventoryItems::MergeFrom(const InventoryItems& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:protocol.InventoryItems)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  items_.MergeFrom(from.items_);
  if (from.type() != 0) {
    set_type(from.type());
  }
}

void InventoryItems::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:protocol.InventoryItems)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void InventoryItems::CopyFrom(const InventoryItems& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:protocol.InventoryItems)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool InventoryItems::IsInitialized() const {
  return true;
}

void InventoryItems::Swap(InventoryItems* other) {
  if (other == this) return;
  InternalSwap(other);
}
void InventoryItems::InternalSwap(InventoryItems* other) {
  using std::swap;
  items_.InternalSwap(&other->items_);
  swap(type_, other->type_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata InventoryItems::GetMetadata() const {
  protobuf_core_2fTronInventoryItems_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_core_2fTronInventoryItems_2eproto::file_level_metadata[kIndexInFileMessages];
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// InventoryItems

// int32 type = 1;
void InventoryItems::clear_type() {
  type_ = 0;
}
::google::protobuf::int32 InventoryItems::type() const {
  // @@protoc_insertion_point(field_get:protocol.InventoryItems.type)
  return type_;
}
void InventoryItems::set_type(::google::protobuf::int32 value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:protocol.InventoryItems.type)
}

// repeated bytes items = 2;
int InventoryItems::items_size() const {
  return items_.size();
}
void InventoryItems::clear_items() {
  items_.Clear();
}
const ::std::string& InventoryItems::items(int index) const {
  // @@protoc_insertion_point(field_get:protocol.InventoryItems.items)
  return items_.Get(index);
}
::std::string* InventoryItems::mutable_items(int index) {
  // @@protoc_insertion_point(field_mutable:protocol.InventoryItems.items)
  return items_.Mutable(index);
}
void InventoryItems::set_items(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:protocol.InventoryItems.items)
  items_.Mutable(index)->assign(value);
}
#if LANG_CXX11
void InventoryItems::set_items(int index, ::std::string&& value) {
  // @@protoc_insertion_point(field_set:protocol.InventoryItems.items)
  items_.Mutable(index)->assign(std::move(value));
}
#endif
void InventoryItems::set_items(int index, const char* value) {
  GOOGLE_DCHECK(value != NULL);
  items_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:protocol.InventoryItems.items)
}
void InventoryItems::set_items(int index, const void* value, size_t size) {
  items_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:protocol.InventoryItems.items)
}
::std::string* InventoryItems::add_items() {
  // @@protoc_insertion_point(field_add_mutable:protocol.InventoryItems.items)
  return items_.Add();
}
void InventoryItems::add_items(const ::std::string& value) {
  items_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:protocol.InventoryItems.items)
}
#if LANG_CXX11
void InventoryItems::add_items(::std::string&& value) {
  items_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:protocol.InventoryItems.items)
}
#endif
void InventoryItems::add_items(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  items_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:protocol.InventoryItems.items)
}
void InventoryItems::add_items(const void* value, size_t size) {
  items_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:protocol.InventoryItems.items)
}
const ::google::protobuf::RepeatedPtrField< ::std::string>&
InventoryItems::items() const {
  // @@protoc_insertion_point(field_list:protocol.InventoryItems.items)
  return items_;
}
::google::protobuf::RepeatedPtrField< ::std::string>*
InventoryItems::mutable_items() {
  // @@protoc_insertion_point(field_mutable_list:protocol.InventoryItems.items)
  return &items_;
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol

// @@protoc_insertion_point(global_scope)
