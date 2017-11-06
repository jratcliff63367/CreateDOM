#include "CreateDOM.h"
#include "StringHelper.h"
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <assert.h>

#if 0
#include "PhysicsDOM.h"
#else

#pragma warning(disable:4100)

namespace CREATE_DOM
{

typedef std::vector< std::string > StringVector;

static const char *getDefaultValueString(const char *df)
{
	const char *ret = df;

	if (_stricmp(df, "TRUE") == 0)
	{
		ret = "true";
	}
	else if (_stricmp(df, "FALSE") == 0)
	{
		ret = "false";
	}
	else
	{
		if (isdigit(*df) && strchr(df, '.'))
		{
			static char gTempFloat[512];
			STRING_HELPER::stringFormat(gTempFloat,512, "%sf", df);
			ret = gTempFloat;
		}
	}

	return ret;
}

class DataItem
{
public:
	bool			mIsArray{ false }; // true if this data item is an array
	bool			mIsPointer{ false };
	std::string		mName;	// name of this data item
	std::string		mType;	// Type of data item
	std::string		mInheritsFrom;
	std::string		mEngineSpecific;
	std::string		mDefaultValue;
	std::string		mMinValue;
	std::string		mMaxValue;
	std::string		mShortDescription;
	std::string		mLongDescription;
};

typedef std::vector< DataItem > DataItemVector;

const char *getTypeString(const char *type)
{
	const char *ret = type;

	if (strcmp(type, "string") == 0)
	{
		ret = "std::string";
	}
	else if (strcmp(type, "u8") == 0)
	{
		ret = "uint8_t";
	}
	else if (strcmp(type, "u16") == 0)
	{
		ret = "uint16_t";
	}
	else if (strcmp(type, "u32") == 0)
	{
		ret = "uint32_t";
	}
	else if (strcmp(type, "u64") == 0)
	{
		ret = "uint64_t";
	}
	else if (strcmp(type, "i8") == 0)
	{
		ret = "int8_t";
	}
	else if (strcmp(type, "i16") == 0)
	{
		ret = "int16_t";
	}
	else if (strcmp(type, "i32") == 0)
	{
		ret = "int32_t";
	}
	else if (strcmp(type, "i64") == 0)
	{
		ret = "int64_t";
	}

	return ret;
}

char upcase(char c)
{
	if (c >= 'a' && c <= 'z')
	{
		c -= 32;
	}
	return c;
}

class Object
{
public:
	void clear(void)
	{
		Object c;
		*this = c;
	}

	void saveCPP(FILE *fph,StringVector &arrays)
	{
		if (_stricmp(mType.c_str(), "Enum") == 0)
		{
			// it's an enum...
			fprintf(fph, "\r\n");
			if (!mShortDescription.empty())
			{
				fprintf(fph, "// %s\r\n", mShortDescription.c_str());
			}
			if (!mLongDescription.empty())
			{
				fprintf(fph, "// %s\r\n", mLongDescription.c_str());
			}
			fprintf(fph, "enum %s\r\n", mName.c_str());
			fprintf(fph, "{\r\n");
			for (auto &i : mItems)
			{
				fprintf(fph, "\t%s,\t\t // %s\r\n",
					i.mName.c_str(),
					i.mShortDescription.c_str());
			}
			fprintf(fph, "};\r\n");
			fprintf(fph, "\r\n");
			return;
		}

		// Before we can save out the class definition; we need to first see
		// if this class definition contains any arrays..
		for (auto &i : mItems)
		{
			if (i.mIsArray) // ok.. this is an array of object!
			{
				// ok, now see if this array is already represented.
				bool found = false;
				for (auto &j : arrays)
				{
					if (j == i.mType)
					{
						found = true;
						break;
					}
				}
				// If this array type not already represented; we need to define it
				if (!found)
				{
					fprintf(fph, "\r\n");
					const char *type = getTypeString(i.mType.c_str());
					char temp[512];
					strncpy(temp, i.mType.c_str(),512);
					temp[0] = upcase(temp[0]);
					if (i.mIsPointer)
					{
						fprintf(fph, "typedef std::vector< %s *> %sVector; // Forward declare the '%s' vector\r\n", type, temp, temp);
					}
					else
					{
						fprintf(fph, "typedef std::vector< %s > %sVector; // Forward declare the '%s' vector\r\n", type, temp, temp);
					}
					fprintf(fph, "\r\n");
					arrays.push_back(i.mType);
				}
			}
		}

		fprintf(fph, "\r\n");
		if (!mShortDescription.empty())
		{
			fprintf(fph, "// %s\r\n", mShortDescription.c_str());
		}
		if (!mLongDescription.empty())
		{
			fprintf(fph, "// %s\r\n", mLongDescription.c_str());
		}
		fprintf(fph, "class %s", mName.c_str());
		if (!mInheritsFrom.empty())
		{
			fprintf(fph, " : public %s", mInheritsFrom.c_str());
		}
		fprintf(fph, "\r\n");
		fprintf(fph, "{\r\n");

		fprintf(fph, "public:\r\n");

		bool hasInheritedItemsWithDefaultValues = false;
		for (auto &i : mItems)
		{
			// If this is an 'inherited' data item. Don't clear it here
			// Because it was already handled in the initializer
			if (!i.mInheritsFrom.empty() && !i.mDefaultValue.empty())
			{
				hasInheritedItemsWithDefaultValues = true;
				break;
			}
		}
		if (hasInheritedItemsWithDefaultValues)
		{
			fprintf(fph, "\t%s(void)\r\n", mName.c_str());
			fprintf(fph, "\t{\r\n");
			for (auto &i : mItems)
			{
				// If this is an 'inherited' data item. Don't clear it here
				// Because it was already handled in the initializer
				if (!i.mInheritsFrom.empty() && !i.mDefaultValue.empty())
				{
					fprintf(fph, "\t\t%s::%s = %s;\r\n",
						i.mInheritsFrom.c_str(),
						i.mName.c_str(),
						getDefaultValueString(i.mDefaultValue.c_str()));
				}
			}
			fprintf(fph, "\t};\r\n");
			fprintf(fph, "\r\n");
		}

		// see if any items are an array of pointers...
		bool hasArrayOfPointers = false;
		for (auto &i : mItems)
		{
			if (i.mIsArray && i.mIsPointer )
			{
				hasArrayOfPointers = true;
			}
		}
		if (hasArrayOfPointers)
		{
			fprintf(fph, "\t~%s(void)\r\n", mName.c_str());
			fprintf(fph, "\t{\r\n");
			for (auto &i : mItems)
			{
				if (i.mIsArray && i.mIsPointer)
				{
					fprintf(fph, "\t\tfor (auto &i:%s) delete i;\r\n", i.mName.c_str());
				}
			}
			fprintf(fph, "\t};\r\n");
			fprintf(fph, "\r\n");
		}

		for (auto &i : mItems)
		{
			// If this is an 'inherited' data item. Don't clear it here
			// Because it was already handled in the initializer
			if (!i.mInheritsFrom.empty())
			{
				continue;
			}
			if (i.mIsArray)
			{
				char temp[512];
				strncpy(temp, i.mType.c_str(), 512);
				temp[0] = upcase(temp[0]);
				char vectorName[512];
				STRING_HELPER::stringFormat(vectorName,512, "%sVector", temp);
				fprintf(fph, "\t%s",
					vectorName);
			}
			else
			{
				fprintf(fph, "\t%s",
					getTypeString(i.mType.c_str()));
			}

			if (i.mIsPointer && !i.mIsArray)
			{
				fprintf(fph, "\t\t*%s",
					i.mName.c_str());
			}
			else
			{
				fprintf(fph, "\t\t%s",
					i.mName.c_str());
			}

			if (i.mDefaultValue.empty())
			{
				if (i.mIsPointer && !i.mIsArray )
				{
					fprintf(fph, "{ nullptr }");
				}
			}
			else
			{
				fprintf(fph, "{ %s }", getDefaultValueString(i.mDefaultValue.c_str()));
			}

			fprintf(fph, ";");

			fprintf(fph, "\t// %s\r\n",
				i.mShortDescription.c_str());
		}

		fprintf(fph, "};\r\n");

		fprintf(fph, "\r\n");
	}

	std::string		mName;
	std::string		mType;
	std::string		mInheritsFrom;
	std::string		mEngineSpecific;
	std::string		mShortDescription;
	std::string		mLongDescription;
	DataItemVector	mItems;
};

typedef std::vector< Object > ObjectVector;

class DOM
{
public:

	void saveCPP(FILE *fph)
	{

		StringVector arrays;

		char temp[512];
		STRING_HELPER::stringFormat(temp, 512, "%s_H", mFilename);
		_strupr(temp);
		fprintf(fph, "#ifndef %s\r\n", temp);
		fprintf(fph, "#define %s\r\n", temp);
		fprintf(fph, "\r\n");
		fprintf(fph, "// Warning : This source file was auto-generated by the CreateDOM tool. Do not try to edit this source file manually!\r\n");
		fprintf(fph, "\r\n");
		fprintf(fph, "#include <stdint.h>\r\n");
		fprintf(fph, "#include <vector>\r\n");
		fprintf(fph, "\r\n");
		fprintf(fph, "\r\n");
		fprintf(fph, "namespace %s\r\n", mNamespace.c_str());
		fprintf(fph, "{\r\n");
		fprintf(fph, "\r\n");

		for (auto &i : mObjects)
		{
			i.saveCPP(fph,arrays);
		}

		fprintf(fph, "\r\n");
		fprintf(fph, "\r\n");
		fprintf(fph, "} // End of %s namespace\r\n", mNamespace.c_str());
		fprintf(fph, "\r\n");
		fprintf(fph, "#endif // End of %s\r\n", temp);

	}

	std::string		mNamespace;
	std::string		mFilename;
	ObjectVector	mObjects;
};

class CreateDOMImpl : public CreateDOM
{
public:
	CreateDOMImpl(void)
	{

	}
	virtual ~CreateDOMImpl(void)
	{

	}


	// Parses this XML and accumulates all of the unique element and attribute names
	virtual void parseCSV(const char *xmlName) final
	{
		FILE *fph = fopen(xmlName, "rb");
		if (fph == nullptr) return;
		fseek(fph, 0L, SEEK_END);
		size_t flen = ftell(fph);
		fseek(fph, 0L, SEEK_SET);
		if (flen > 0)
		{
			char *data = (char *)malloc(flen + 1);
			size_t r = fread(data, flen, 1, fph);
			if (r == 1)
			{
				data[flen] = 0;
				char *scan = data;
				uint32_t argc = 0;
#define MAX_ARGV 128
				char *argv[MAX_ARGV];

				while (*scan)
				{
					// Skip any linefeeds
					argc = 0;
					while (*scan == 10 || *scan == 13 && *scan )
					{
						scan++;
					}
					if (*scan == 0)
					{
						break;
					}
					for (;;)
					{
						if (*scan == 34) // if it's an open quote..
						{
							scan++;
							argv[argc] = scan;
							while (*scan && *scan != 34)
							{
								scan++;
							}
							if (*scan)
							{
								*scan = 0;
								scan++;
							}
						}
						else
						{
							argv[argc] = scan;
						}
						while (*scan && *scan != ',' && *scan != 10 && *scan != 13)
						{
							scan++;
						}
						if (argc < MAX_ARGV)
						{
							argc++;
						}
						if (*scan)
						{
							char c = *scan;
							*scan = 0;
							scan++;
							if (c == 10 || c == 13)
							{
								processArgs(argc, argv);
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
			}
			free(data);
		}
		fclose(fph);
	}

	// Save the DOM as C++ code
	virtual void saveCPP(void) final
	{
		if (mDOM.mNamespace.empty())
		{
			printf("No namespace specified!\r\n");
			return;
		}
		if (mDOM.mFilename.empty())
		{
			printf("No source filename specified.\r\n");
			return;
		}
		char scratch[512];
		STRING_HELPER::stringFormat(scratch, 512, "%s.h", mDOM.mFilename);
		FILE *fph = fopen(scratch, "wb");
		if (fph == nullptr)
		{
			printf("Failed to open output file: %s for write access.\r\n", scratch);
			return;
		}
		printf("Saving C++ DOM to: %s\r\n", scratch);
		mDOM.saveCPP(fph);


		fclose(fph);
	}

	// Save the DOM as a JSON schema
	virtual void saveJSON(void) final
	{

	}

	virtual void release(void) final
	{
		delete this;
	}

	virtual void processArgs(uint32_t argc,char **argv) final
	{
		if (_stricmp(argv[0],"Filename") == 0 )
		{
			if (argc >= 2)
			{
				mDOM.mFilename = std::string(argv[1]);
			}
		}
		else if (_stricmp(argv[0], "Namespace") == 0)
		{
			if (argc >= 2)
			{
				mDOM.mNamespace = std::string(argv[1]);
			}
		}
		else if (_stricmp(argv[0], "ObjectName") == 0)
		{
			// skip, this is just the header line..
		}
		else
		{
			const char *object = argv[0];
			if (strlen(object)) // if it's a new object 
			{
				if (mHaveObject)
				{
					mDOM.mObjects.push_back(mCurrentObject);
					mCurrentObject.clear();
				}
				mHaveObject = true;
				mCurrentObject.mName = std::string(object);
				if (argc >= 3 )
				{
					mCurrentObject.mType = std::string(argv[2]);
					if (argc >= 4)
					{
						mCurrentObject.mInheritsFrom = std::string(argv[3]);
						if (argc >= 5)
						{
							mCurrentObject.mEngineSpecific = std::string(argv[4]);
							if (argc >= 9)
							{
								mCurrentObject.mShortDescription = std::string(argv[8]);
								if (argc >= 10)
								{
									mCurrentObject.mLongDescription = std::string(argv[9]);
								}
							}
						}
					}
				}
			}
			else if (argc >= 2)
			{
				char *dataItemName = argv[1];
				if (strlen(dataItemName))
				{
					char *pointer = strchr(dataItemName, '*');
					char *array = strchr(dataItemName, '[');
					if (array)
					{
						*array = 0;
					}
					else if (pointer)
					{
						*pointer = 0;
					}
					DataItem di;
					di.mName = std::string(dataItemName);
					di.mIsArray = array ? true : false;
					di.mIsPointer = pointer ? true : false;
					if (argc >= 3)
					{
						di.mType = std::string(argv[2]);
						if (argc >= 4)
						{
							di.mInheritsFrom = std::string(argv[3]);
							if (argc >= 5)
							{
								di.mEngineSpecific = std::string(argv[4]);
								if (argc >= 6)
								{
									di.mDefaultValue = std::string(argv[5]);
									if (argc >= 7)
									{
										di.mMinValue = std::string(argv[6]);
										if (argc >= 8)
										{
											di.mMaxValue = std::string(argv[7]);
											if (argc >= 9)
											{
												di.mShortDescription = std::string(argv[8]);
												if (argc >= 10)
												{
													di.mLongDescription = std::string(argv[9]);
												}
											}
										}
									}
								}
							}
						}
					}
					mCurrentObject.mItems.push_back(di);
				}
			}
		}
	}

	bool	mHaveObject{ false };
	Object	mCurrentObject;
	DOM		mDOM;
};

CreateDOM *CreateDOM::create(void)
{
	CreateDOMImpl *in = new CreateDOMImpl;
	return static_cast<CreateDOM *>(in);
}



}
#endif