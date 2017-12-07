#include "TextProtoUtil.h"

#include <algorithm>

namespace TextProtoUtil
{
	static bool EndsWith(std::string value, std::string ending, bool case_sensitive)
	{
		if (ending.size() > value.size()) return false;

		if (!case_sensitive)
		{
			std::transform(value.begin(), value.end(), value.begin(), tolower);
			std::transform(ending.begin(), ending.end(), ending.begin(), tolower);
		}

		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	// Checks if the file extension is ".json".
	// It is not case sensitive.
	inline static bool IsJsonExt(const std::string& in_name)
	{
		return EndsWith(in_name, ".json", false);
	}

	bool ReadTextProto(const std::string& in_name, google::protobuf::Message* message)
	{
		if (IsJsonExt(in_name))
		{
			// Reads from json. Uses standard file to string functions.
			std::ifstream stream(in_name);
			std::string json_string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

			google::protobuf::util::JsonParseOptions options;
			if (!google::protobuf::util::JsonStringToMessage(json_string, message, options).ok())
			{
				return false;
			}
		}
		else
		{
			// Read from binary protobuf.
			// Uses Google's FileInputStream, which needs a file descriptor.
			FILE* file = nullptr;
			fopen_s(&file, in_name.c_str(), "r");
			if (!file)
			{
				return false;
			}
			int file_descriptor = _fileno(file);

			google::protobuf::io::FileInputStream file_input(file_descriptor);
			if (!google::protobuf::TextFormat::Parse(&file_input, message))
			{
				return false;
			}

			fclose(file);
		}

		return true;
	}

	bool WriteTextProto(const std::string& out_name, const google::protobuf::Message& message)
	{
		std::string out_string;

		if (IsJsonExt(out_name))
		{
			// Json output.
			google::protobuf::util::JsonPrintOptions json_options;
			json_options.always_print_primitive_fields = true;
			json_options.add_whitespace = true;
			if (!google::protobuf::util::MessageToJsonString(message, &out_string, json_options).ok())
			{
				return false;
			}
		}
		else
		{
			// Textproto output.
			if (!google::protobuf::TextFormat::PrintToString(message, &out_string))
			{
				return false;
			}
		}

		// Write file.
		std::ofstream out_file;
		out_file.open(out_name);
		out_file << out_string;
		out_file.close();

		return true;
	}
}

