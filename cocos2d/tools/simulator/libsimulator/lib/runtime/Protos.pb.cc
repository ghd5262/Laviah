// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Protos.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Protos.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace runtime {

void protobuf_ShutdownFile_Protos_2eproto() {
  delete FileSendProtos::default_instance_;
  delete FileSendComplete::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_Protos_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_Protos_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  FileSendProtos::default_instance_ = new FileSendProtos();
  FileSendComplete::default_instance_ = new FileSendComplete();
  FileSendProtos::default_instance_->InitAsDefaultInstance();
  FileSendComplete::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Protos_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_Protos_2eproto_once_);
void protobuf_AddDesc_Protos_2eproto() {
  ::google::protobuf::::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_Protos_2eproto_once_,
                 &protobuf_AddDesc_Protos_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Protos_2eproto {
  StaticDescriptorInitializer_Protos_2eproto() {
    protobuf_AddDesc_Protos_2eproto();
  }
} static_descriptor_initializer_Protos_2eproto_;
#endif

// ===================================================================

bool FileSendProtos_CompressType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const FileSendProtos_CompressType FileSendProtos::NO_COMPRESS;
const FileSendProtos_CompressType FileSendProtos::ZIP;
const FileSendProtos_CompressType FileSendProtos::CompressType_MIN;
const FileSendProtos_CompressType FileSendProtos::CompressType_MAX;
const int FileSendProtos::CompressType_ARRAYSIZE;
#endif  // _MSC_VER
bool FileSendProtos_VerifyMode_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const FileSendProtos_VerifyMode FileSendProtos::NO_VERIFY;
const FileSendProtos_VerifyMode FileSendProtos::CRC;
const FileSendProtos_VerifyMode FileSendProtos::MD5;
const FileSendProtos_VerifyMode FileSendProtos::VerifyMode_MIN;
const FileSendProtos_VerifyMode FileSendProtos::VerifyMode_MAX;
const int FileSendProtos::VerifyMode_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int FileSendProtos::kFileNameFieldNumber;
const int FileSendProtos::kPackageSeqFieldNumber;
const int FileSendProtos::kPackageSumFieldNumber;
const int FileSendProtos::kContentSizeFieldNumber;
const int FileSendProtos::kCompressTypeFieldNumber;
const int FileSendProtos::kModifiedTimeFieldNumber;
const int FileSendProtos::kVerifyModeFieldNumber;
const int FileSendProtos::kVerifySignFieldNumber;
const int FileSendProtos::kUncompressSizeFieldNumber;
#endif  // !_MSC_VER

FileSendProtos::FileSendProtos()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void FileSendProtos::InitAsDefaultInstance() {
}

FileSendProtos::FileSendProtos(const FileSendProtos& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void FileSendProtos::SharedCtor() {
  _cached_size_ = 0;
  file_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  package_seq_ = 0;
  package_sum_ = 0;
  content_size_ = GOOGLE_ULONGLONG(0);
  compress_type_ = 0;
  modified_time_ = GOOGLE_ULONGLONG(0);
  verify_mode_ = 0;
  verify_sign_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  uncompress_size_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

FileSendProtos::~FileSendProtos() {
  SharedDtor();
}

void FileSendProtos::SharedDtor() {
  if (file_name_ != &::google::protobuf::internal::kEmptyString) {
    delete file_name_;
  }
  if (verify_sign_ != &::google::protobuf::internal::kEmptyString) {
    delete verify_sign_;
  }
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void FileSendProtos::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const FileSendProtos& FileSendProtos::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_Protos_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_Protos_2eproto();
#endif
  return *default_instance_;
}

FileSendProtos* FileSendProtos::default_instance_ = NULL;

FileSendProtos* FileSendProtos::New() const {
  return new FileSendProtos;
}

void FileSendProtos::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_file_name()) {
      if (file_name_ != &::google::protobuf::internal::kEmptyString) {
        file_name_->clear();
      }
    }
    package_seq_ = 0;
    package_sum_ = 0;
    content_size_ = GOOGLE_ULONGLONG(0);
    compress_type_ = 0;
    modified_time_ = GOOGLE_ULONGLONG(0);
    verify_mode_ = 0;
    if (has_verify_sign()) {
      if (verify_sign_ != &::google::protobuf::internal::kEmptyString) {
        verify_sign_->clear();
      }
    }
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    uncompress_size_ = GOOGLE_ULONGLONG(0);
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool FileSendProtos::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string file_name = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_file_name()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_package_seq;
        break;
      }

      // required int32 package_seq = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_package_seq:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &package_seq_)));
          set_has_package_seq();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_package_sum;
        break;
      }

      // required int32 package_sum = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_package_sum:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &package_sum_)));
          set_has_package_sum();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_content_size;
        break;
      }

      // required uint64 content_size = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_content_size:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &content_size_)));
          set_has_content_size();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_compress_type;
        break;
      }

      // required .runtime.FileSendProtos.CompressType compress_type = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_compress_type:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::runtime::FileSendProtos_CompressType_IsValid(value)) {
            set_compress_type(static_cast< ::runtime::FileSendProtos_CompressType >(value));
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_modified_time;
        break;
      }

      // optional uint64 modified_time = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_modified_time:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &modified_time_)));
          set_has_modified_time();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(56)) goto parse_verify_mode;
        break;
      }

      // optional .runtime.FileSendProtos.VerifyMode verify_mode = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_verify_mode:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::runtime::FileSendProtos_VerifyMode_IsValid(value)) {
            set_verify_mode(static_cast< ::runtime::FileSendProtos_VerifyMode >(value));
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(66)) goto parse_verify_sign;
        break;
      }

      // optional string verify_sign = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_verify_sign:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_verify_sign()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(72)) goto parse_uncompress_size;
        break;
      }

      // optional uint64 uncompress_size = 9;
      case 9: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_uncompress_size:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &uncompress_size_)));
          set_has_uncompress_size();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void FileSendProtos::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string file_name = 1;
  if (has_file_name()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->file_name(), output);
  }

  // required int32 package_seq = 2;
  if (has_package_seq()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->package_seq(), output);
  }

  // required int32 package_sum = 3;
  if (has_package_sum()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->package_sum(), output);
  }

  // required uint64 content_size = 4;
  if (has_content_size()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(4, this->content_size(), output);
  }

  // required .runtime.FileSendProtos.CompressType compress_type = 5;
  if (has_compress_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      5, this->compress_type(), output);
  }

  // optional uint64 modified_time = 6;
  if (has_modified_time()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(6, this->modified_time(), output);
  }

  // optional .runtime.FileSendProtos.VerifyMode verify_mode = 7;
  if (has_verify_mode()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      7, this->verify_mode(), output);
  }

  // optional string verify_sign = 8;
  if (has_verify_sign()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      8, this->verify_sign(), output);
  }

  // optional uint64 uncompress_size = 9;
  if (has_uncompress_size()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(9, this->uncompress_size(), output);
  }

}

int FileSendProtos::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string file_name = 1;
    if (has_file_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->file_name());
    }

    // required int32 package_seq = 2;
    if (has_package_seq()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->package_seq());
    }

    // required int32 package_sum = 3;
    if (has_package_sum()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->package_sum());
    }

    // required uint64 content_size = 4;
    if (has_content_size()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->content_size());
    }

    // required .runtime.FileSendProtos.CompressType compress_type = 5;
    if (has_compress_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->compress_type());
    }

    // optional uint64 modified_time = 6;
    if (has_modified_time()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->modified_time());
    }

    // optional .runtime.FileSendProtos.VerifyMode verify_mode = 7;
    if (has_verify_mode()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->verify_mode());
    }

    // optional string verify_sign = 8;
    if (has_verify_sign()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->verify_sign());
    }

  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    // optional uint64 uncompress_size = 9;
    if (has_uncompress_size()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->uncompress_size());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void FileSendProtos::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const FileSendProtos*>(&from));
}

void FileSendProtos::MergeFrom(const FileSendProtos& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_file_name()) {
      set_file_name(from.file_name());
    }
    if (from.has_package_seq()) {
      set_package_seq(from.package_seq());
    }
    if (from.has_package_sum()) {
      set_package_sum(from.package_sum());
    }
    if (from.has_content_size()) {
      set_content_size(from.content_size());
    }
    if (from.has_compress_type()) {
      set_compress_type(from.compress_type());
    }
    if (from.has_modified_time()) {
      set_modified_time(from.modified_time());
    }
    if (from.has_verify_mode()) {
      set_verify_mode(from.verify_mode());
    }
    if (from.has_verify_sign()) {
      set_verify_sign(from.verify_sign());
    }
  }
  if (from._has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (from.has_uncompress_size()) {
      set_uncompress_size(from.uncompress_size());
    }
  }
}

void FileSendProtos::CopyFrom(const FileSendProtos& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FileSendProtos::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000001f) != 0x0000001f) return false;

  return true;
}

void FileSendProtos::Swap(FileSendProtos* other) {
  if (other != this) {
    std::swap(file_name_, other->file_name_);
    std::swap(package_seq_, other->package_seq_);
    std::swap(package_sum_, other->package_sum_);
    std::swap(content_size_, other->content_size_);
    std::swap(compress_type_, other->compress_type_);
    std::swap(modified_time_, other->modified_time_);
    std::swap(verify_mode_, other->verify_mode_);
    std::swap(verify_sign_, other->verify_sign_);
    std::swap(uncompress_size_, other->uncompress_size_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string FileSendProtos::GetTypeName() const {
  return "runtime.FileSendProtos";
}


// ===================================================================

bool FileSendComplete_RESULTTYPE_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const FileSendComplete_RESULTTYPE FileSendComplete::SUCCESS;
const FileSendComplete_RESULTTYPE FileSendComplete::RECV_ERROR;
const FileSendComplete_RESULTTYPE FileSendComplete::UNCOMPRESS_ERROR;
const FileSendComplete_RESULTTYPE FileSendComplete::FOPEN_ERROR;
const FileSendComplete_RESULTTYPE FileSendComplete::FWRITE_ERROR;
const FileSendComplete_RESULTTYPE FileSendComplete::RESULTTYPE_MIN;
const FileSendComplete_RESULTTYPE FileSendComplete::RESULTTYPE_MAX;
const int FileSendComplete::RESULTTYPE_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int FileSendComplete::kFileNameFieldNumber;
const int FileSendComplete::kResultFieldNumber;
const int FileSendComplete::kErrorNumFieldNumber;
#endif  // !_MSC_VER

FileSendComplete::FileSendComplete()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void FileSendComplete::InitAsDefaultInstance() {
}

FileSendComplete::FileSendComplete(const FileSendComplete& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void FileSendComplete::SharedCtor() {
  _cached_size_ = 0;
  file_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  result_ = 0;
  error_num_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

FileSendComplete::~FileSendComplete() {
  SharedDtor();
}

void FileSendComplete::SharedDtor() {
  if (file_name_ != &::google::protobuf::internal::kEmptyString) {
    delete file_name_;
  }
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void FileSendComplete::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const FileSendComplete& FileSendComplete::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_Protos_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_Protos_2eproto();
#endif
  return *default_instance_;
}

FileSendComplete* FileSendComplete::default_instance_ = NULL;

FileSendComplete* FileSendComplete::New() const {
  return new FileSendComplete;
}

void FileSendComplete::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_file_name()) {
      if (file_name_ != &::google::protobuf::internal::kEmptyString) {
        file_name_->clear();
      }
    }
    result_ = 0;
    error_num_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool FileSendComplete::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string file_name = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_file_name()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_result;
        break;
      }

      // required .runtime.FileSendComplete.RESULTTYPE result = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_result:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::runtime::FileSendComplete_RESULTTYPE_IsValid(value)) {
            set_result(static_cast< ::runtime::FileSendComplete_RESULTTYPE >(value));
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_error_num;
        break;
      }

      // required int32 error_num = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_error_num:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &error_num_)));
          set_has_error_num();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void FileSendComplete::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string file_name = 1;
  if (has_file_name()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->file_name(), output);
  }

  // required .runtime.FileSendComplete.RESULTTYPE result = 2;
  if (has_result()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      2, this->result(), output);
  }

  // required int32 error_num = 3;
  if (has_error_num()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->error_num(), output);
  }

}

int FileSendComplete::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string file_name = 1;
    if (has_file_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->file_name());
    }

    // required .runtime.FileSendComplete.RESULTTYPE result = 2;
    if (has_result()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->result());
    }

    // required int32 error_num = 3;
    if (has_error_num()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->error_num());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void FileSendComplete::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const FileSendComplete*>(&from));
}

void FileSendComplete::MergeFrom(const FileSendComplete& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_file_name()) {
      set_file_name(from.file_name());
    }
    if (from.has_result()) {
      set_result(from.result());
    }
    if (from.has_error_num()) {
      set_error_num(from.error_num());
    }
  }
}

void FileSendComplete::CopyFrom(const FileSendComplete& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FileSendComplete::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void FileSendComplete::Swap(FileSendComplete* other) {
  if (other != this) {
    std::swap(file_name_, other->file_name_);
    std::swap(result_, other->result_);
    std::swap(error_num_, other->error_num_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string FileSendComplete::GetTypeName() const {
  return "runtime.FileSendComplete";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace runtime

// @@protoc_insertion_point(global_scope)
