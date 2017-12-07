#pragma once

#include <ostream>
#include <fstream>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/json_util.h>
#include <fcntl.h>

namespace TextProtoUtil
{
	// Returns true if success.
	// If the in_name extension is ".json" it will serialize as a json.
	// Otherwise, default to textproto format.
	bool ReadTextProto(const std::string& in_name, google::protobuf::Message* message);

	// Returns true if success.
	// If the out_name extension is ".json" it will read as a json.
	// Otherwise, default to textproto format.
	bool WriteTextProto(const std::string& out_name, const google::protobuf::Message& message);
}
