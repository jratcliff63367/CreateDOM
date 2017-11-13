// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ProtoAddressBook.proto

#ifndef PROTOBUF_ProtoAddressBook_2eproto__INCLUDED
#define PROTOBUF_ProtoAddressBook_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3004000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
namespace CreateDOM {
class AddressBook;
class AddressBookDefaultTypeInternal;
extern AddressBookDefaultTypeInternal _AddressBook_default_instance_;
class Person;
class PersonDefaultTypeInternal;
extern PersonDefaultTypeInternal _Person_default_instance_;
class PhoneNumber;
class PhoneNumberDefaultTypeInternal;
extern PhoneNumberDefaultTypeInternal _PhoneNumber_default_instance_;
}  // namespace CreateDOM

namespace CreateDOM {

namespace protobuf_ProtoAddressBook_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static void InitDefaultsImpl();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_ProtoAddressBook_2eproto

enum PhoneType {
  MOBILE = 0,
  HOME = 1,
  WORK = 2,
  PhoneType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  PhoneType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool PhoneType_IsValid(int value);
const PhoneType PhoneType_MIN = MOBILE;
const PhoneType PhoneType_MAX = WORK;
const int PhoneType_ARRAYSIZE = PhoneType_MAX + 1;

const ::google::protobuf::EnumDescriptor* PhoneType_descriptor();
inline const ::std::string& PhoneType_Name(PhoneType value) {
  return ::google::protobuf::internal::NameOfEnum(
    PhoneType_descriptor(), value);
}
inline bool PhoneType_Parse(
    const ::std::string& name, PhoneType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<PhoneType>(
    PhoneType_descriptor(), name, value);
}
// ===================================================================

class PhoneNumber : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CreateDOM.PhoneNumber) */ {
 public:
  PhoneNumber();
  virtual ~PhoneNumber();

  PhoneNumber(const PhoneNumber& from);

  inline PhoneNumber& operator=(const PhoneNumber& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  PhoneNumber(PhoneNumber&& from) noexcept
    : PhoneNumber() {
    *this = ::std::move(from);
  }

  inline PhoneNumber& operator=(PhoneNumber&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const PhoneNumber& default_instance();

  static inline const PhoneNumber* internal_default_instance() {
    return reinterpret_cast<const PhoneNumber*>(
               &_PhoneNumber_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(PhoneNumber* other);
  friend void swap(PhoneNumber& a, PhoneNumber& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline PhoneNumber* New() const PROTOBUF_FINAL { return New(NULL); }

  PhoneNumber* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const PhoneNumber& from);
  void MergeFrom(const PhoneNumber& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(PhoneNumber* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string number = 1;
  void clear_number();
  static const int kNumberFieldNumber = 1;
  const ::std::string& number() const;
  void set_number(const ::std::string& value);
  #if LANG_CXX11
  void set_number(::std::string&& value);
  #endif
  void set_number(const char* value);
  void set_number(const char* value, size_t size);
  ::std::string* mutable_number();
  ::std::string* release_number();
  void set_allocated_number(::std::string* number);

  // .CreateDOM.PhoneType type = 2;
  void clear_type();
  static const int kTypeFieldNumber = 2;
  ::CreateDOM::PhoneType type() const;
  void set_type(::CreateDOM::PhoneType value);

  // @@protoc_insertion_point(class_scope:CreateDOM.PhoneNumber)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr number_;
  int type_;
  mutable int _cached_size_;
  friend struct protobuf_ProtoAddressBook_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Person : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CreateDOM.Person) */ {
 public:
  Person();
  virtual ~Person();

  Person(const Person& from);

  inline Person& operator=(const Person& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Person(Person&& from) noexcept
    : Person() {
    *this = ::std::move(from);
  }

  inline Person& operator=(Person&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Person& default_instance();

  static inline const Person* internal_default_instance() {
    return reinterpret_cast<const Person*>(
               &_Person_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(Person* other);
  friend void swap(Person& a, Person& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Person* New() const PROTOBUF_FINAL { return New(NULL); }

  Person* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Person& from);
  void MergeFrom(const Person& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Person* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string name = 1;
  void clear_name();
  static const int kNameFieldNumber = 1;
  const ::std::string& name() const;
  void set_name(const ::std::string& value);
  #if LANG_CXX11
  void set_name(::std::string&& value);
  #endif
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  ::std::string* mutable_name();
  ::std::string* release_name();
  void set_allocated_name(::std::string* name);

  // string email = 3;
  void clear_email();
  static const int kEmailFieldNumber = 3;
  const ::std::string& email() const;
  void set_email(const ::std::string& value);
  #if LANG_CXX11
  void set_email(::std::string&& value);
  #endif
  void set_email(const char* value);
  void set_email(const char* value, size_t size);
  ::std::string* mutable_email();
  ::std::string* release_email();
  void set_allocated_email(::std::string* email);

  // .CreateDOM.PhoneNumber phones = 4;
  bool has_phones() const;
  void clear_phones();
  static const int kPhonesFieldNumber = 4;
  const ::CreateDOM::PhoneNumber& phones() const;
  ::CreateDOM::PhoneNumber* mutable_phones();
  ::CreateDOM::PhoneNumber* release_phones();
  void set_allocated_phones(::CreateDOM::PhoneNumber* phones);

  // int32 id = 2;
  void clear_id();
  static const int kIdFieldNumber = 2;
  ::google::protobuf::int32 id() const;
  void set_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:CreateDOM.Person)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr name_;
  ::google::protobuf::internal::ArenaStringPtr email_;
  ::CreateDOM::PhoneNumber* phones_;
  ::google::protobuf::int32 id_;
  mutable int _cached_size_;
  friend struct protobuf_ProtoAddressBook_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class AddressBook : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:CreateDOM.AddressBook) */ {
 public:
  AddressBook();
  virtual ~AddressBook();

  AddressBook(const AddressBook& from);

  inline AddressBook& operator=(const AddressBook& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  AddressBook(AddressBook&& from) noexcept
    : AddressBook() {
    *this = ::std::move(from);
  }

  inline AddressBook& operator=(AddressBook&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const AddressBook& default_instance();

  static inline const AddressBook* internal_default_instance() {
    return reinterpret_cast<const AddressBook*>(
               &_AddressBook_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    2;

  void Swap(AddressBook* other);
  friend void swap(AddressBook& a, AddressBook& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline AddressBook* New() const PROTOBUF_FINAL { return New(NULL); }

  AddressBook* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const AddressBook& from);
  void MergeFrom(const AddressBook& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(AddressBook* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // .CreateDOM.Person people = 1;
  bool has_people() const;
  void clear_people();
  static const int kPeopleFieldNumber = 1;
  const ::CreateDOM::Person& people() const;
  ::CreateDOM::Person* mutable_people();
  ::CreateDOM::Person* release_people();
  void set_allocated_people(::CreateDOM::Person* people);

  // @@protoc_insertion_point(class_scope:CreateDOM.AddressBook)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::CreateDOM::Person* people_;
  mutable int _cached_size_;
  friend struct protobuf_ProtoAddressBook_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// PhoneNumber

// string number = 1;
inline void PhoneNumber::clear_number() {
  number_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PhoneNumber::number() const {
  // @@protoc_insertion_point(field_get:CreateDOM.PhoneNumber.number)
  return number_.GetNoArena();
}
inline void PhoneNumber::set_number(const ::std::string& value) {
  
  number_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:CreateDOM.PhoneNumber.number)
}
#if LANG_CXX11
inline void PhoneNumber::set_number(::std::string&& value) {
  
  number_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:CreateDOM.PhoneNumber.number)
}
#endif
inline void PhoneNumber::set_number(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  number_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:CreateDOM.PhoneNumber.number)
}
inline void PhoneNumber::set_number(const char* value, size_t size) {
  
  number_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:CreateDOM.PhoneNumber.number)
}
inline ::std::string* PhoneNumber::mutable_number() {
  
  // @@protoc_insertion_point(field_mutable:CreateDOM.PhoneNumber.number)
  return number_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PhoneNumber::release_number() {
  // @@protoc_insertion_point(field_release:CreateDOM.PhoneNumber.number)
  
  return number_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PhoneNumber::set_allocated_number(::std::string* number) {
  if (number != NULL) {
    
  } else {
    
  }
  number_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), number);
  // @@protoc_insertion_point(field_set_allocated:CreateDOM.PhoneNumber.number)
}

// .CreateDOM.PhoneType type = 2;
inline void PhoneNumber::clear_type() {
  type_ = 0;
}
inline ::CreateDOM::PhoneType PhoneNumber::type() const {
  // @@protoc_insertion_point(field_get:CreateDOM.PhoneNumber.type)
  return static_cast< ::CreateDOM::PhoneType >(type_);
}
inline void PhoneNumber::set_type(::CreateDOM::PhoneType value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:CreateDOM.PhoneNumber.type)
}

// -------------------------------------------------------------------

// Person

// string name = 1;
inline void Person::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Person::name() const {
  // @@protoc_insertion_point(field_get:CreateDOM.Person.name)
  return name_.GetNoArena();
}
inline void Person::set_name(const ::std::string& value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:CreateDOM.Person.name)
}
#if LANG_CXX11
inline void Person::set_name(::std::string&& value) {
  
  name_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:CreateDOM.Person.name)
}
#endif
inline void Person::set_name(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:CreateDOM.Person.name)
}
inline void Person::set_name(const char* value, size_t size) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:CreateDOM.Person.name)
}
inline ::std::string* Person::mutable_name() {
  
  // @@protoc_insertion_point(field_mutable:CreateDOM.Person.name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Person::release_name() {
  // @@protoc_insertion_point(field_release:CreateDOM.Person.name)
  
  return name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Person::set_allocated_name(::std::string* name) {
  if (name != NULL) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:CreateDOM.Person.name)
}

// int32 id = 2;
inline void Person::clear_id() {
  id_ = 0;
}
inline ::google::protobuf::int32 Person::id() const {
  // @@protoc_insertion_point(field_get:CreateDOM.Person.id)
  return id_;
}
inline void Person::set_id(::google::protobuf::int32 value) {
  
  id_ = value;
  // @@protoc_insertion_point(field_set:CreateDOM.Person.id)
}

// string email = 3;
inline void Person::clear_email() {
  email_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Person::email() const {
  // @@protoc_insertion_point(field_get:CreateDOM.Person.email)
  return email_.GetNoArena();
}
inline void Person::set_email(const ::std::string& value) {
  
  email_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:CreateDOM.Person.email)
}
#if LANG_CXX11
inline void Person::set_email(::std::string&& value) {
  
  email_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:CreateDOM.Person.email)
}
#endif
inline void Person::set_email(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  email_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:CreateDOM.Person.email)
}
inline void Person::set_email(const char* value, size_t size) {
  
  email_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:CreateDOM.Person.email)
}
inline ::std::string* Person::mutable_email() {
  
  // @@protoc_insertion_point(field_mutable:CreateDOM.Person.email)
  return email_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Person::release_email() {
  // @@protoc_insertion_point(field_release:CreateDOM.Person.email)
  
  return email_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Person::set_allocated_email(::std::string* email) {
  if (email != NULL) {
    
  } else {
    
  }
  email_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), email);
  // @@protoc_insertion_point(field_set_allocated:CreateDOM.Person.email)
}

// .CreateDOM.PhoneNumber phones = 4;
inline bool Person::has_phones() const {
  return this != internal_default_instance() && phones_ != NULL;
}
inline void Person::clear_phones() {
  if (GetArenaNoVirtual() == NULL && phones_ != NULL) delete phones_;
  phones_ = NULL;
}
inline const ::CreateDOM::PhoneNumber& Person::phones() const {
  const ::CreateDOM::PhoneNumber* p = phones_;
  // @@protoc_insertion_point(field_get:CreateDOM.Person.phones)
  return p != NULL ? *p : *reinterpret_cast<const ::CreateDOM::PhoneNumber*>(
      &::CreateDOM::_PhoneNumber_default_instance_);
}
inline ::CreateDOM::PhoneNumber* Person::mutable_phones() {
  
  if (phones_ == NULL) {
    phones_ = new ::CreateDOM::PhoneNumber;
  }
  // @@protoc_insertion_point(field_mutable:CreateDOM.Person.phones)
  return phones_;
}
inline ::CreateDOM::PhoneNumber* Person::release_phones() {
  // @@protoc_insertion_point(field_release:CreateDOM.Person.phones)
  
  ::CreateDOM::PhoneNumber* temp = phones_;
  phones_ = NULL;
  return temp;
}
inline void Person::set_allocated_phones(::CreateDOM::PhoneNumber* phones) {
  delete phones_;
  phones_ = phones;
  if (phones) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:CreateDOM.Person.phones)
}

// -------------------------------------------------------------------

// AddressBook

// .CreateDOM.Person people = 1;
inline bool AddressBook::has_people() const {
  return this != internal_default_instance() && people_ != NULL;
}
inline void AddressBook::clear_people() {
  if (GetArenaNoVirtual() == NULL && people_ != NULL) delete people_;
  people_ = NULL;
}
inline const ::CreateDOM::Person& AddressBook::people() const {
  const ::CreateDOM::Person* p = people_;
  // @@protoc_insertion_point(field_get:CreateDOM.AddressBook.people)
  return p != NULL ? *p : *reinterpret_cast<const ::CreateDOM::Person*>(
      &::CreateDOM::_Person_default_instance_);
}
inline ::CreateDOM::Person* AddressBook::mutable_people() {
  
  if (people_ == NULL) {
    people_ = new ::CreateDOM::Person;
  }
  // @@protoc_insertion_point(field_mutable:CreateDOM.AddressBook.people)
  return people_;
}
inline ::CreateDOM::Person* AddressBook::release_people() {
  // @@protoc_insertion_point(field_release:CreateDOM.AddressBook.people)
  
  ::CreateDOM::Person* temp = people_;
  people_ = NULL;
  return temp;
}
inline void AddressBook::set_allocated_people(::CreateDOM::Person* people) {
  delete people_;
  people_ = people;
  if (people) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:CreateDOM.AddressBook.people)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


}  // namespace CreateDOM

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::CreateDOM::PhoneType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::CreateDOM::PhoneType>() {
  return ::CreateDOM::PhoneType_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ProtoAddressBook_2eproto__INCLUDED
