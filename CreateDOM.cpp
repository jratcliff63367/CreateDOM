#include "CreateDOM.h"
#include "StringHelper.h"
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <stdarg.h>
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

	class CodePrinter
	{
	public:
		CodePrinter(void)
		{

		}
		CodePrinter(FILE *fph)
		{
			mFph = fph;
		}

		void printCode(uint32_t indent, const char *fmt, ...)
		{
			va_list         args;
			char            buffer[4096];
			va_start(args, fmt);
			STRING_HELPER::stringFormatV(buffer, sizeof(buffer), fmt, args);
			va_end(args);
			if (indent)
			{
				size_t currentPos = ftell(mFph) - mLastLineFeed;
				size_t indentLocation = indent * 4;	// This is the column we want to be on..
				if (currentPos < indentLocation)
				{
					// How many spaces do we need to get to the next tab location
					uint32_t nextTab = uint32_t(currentPos % 4);
					for (size_t i = 0; i < nextTab; i++)
					{
						fprintf(mFph, " ");
					}
					currentPos += nextTab;	// Ok, advance the current positions
					if (currentPos < indentLocation)
					{
						uint32_t indentCount = uint32_t(indentLocation - currentPos);
						uint32_t tabCount = indentCount / 4;
						for (size_t i = 0; i < tabCount; i++)
						{
							fprintf(mFph, "\t");
						}
					}
				}
				else
				{
					fprintf(mFph, " ");
				}
			}
			fprintf(mFph, "%s", buffer);
			fflush(mFph);
			const char *scan = buffer;
			while (*scan)
			{
				if (*scan == 10 || *scan == 10)
				{
					mLastLineFeed = ftell(mFph);
					break;
				}
				scan++;
			}

		}


		size_t	mLastLineFeed{ 0 };
		FILE	*mFph{ nullptr };
};

typedef std::vector< std::string > StringVector;

static bool isDigit(char c)
{
	bool ret = false;
	if (c == '-')
	{
		ret = true;
	}
	else if (c >= '0' && c <= '9')
	{
		ret = true;
	}
	return ret;
}

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
	else if (_stricmp(df, "MAX_U32") == 0)
	{
		ret = "0xFFFFFFFF";
	}
	else
	{
		if (isDigit(*df) && strchr(df, '.') && !strchr(df,',') )
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

	void saveCPP(CodePrinter &cp,StringVector &arrays)
	{
		if (_stricmp(mType.c_str(), "Enum") == 0)
		{
			// it's an enum...
			cp.printCode(0, "\r\n");
			if (!mShortDescription.empty())
			{
				cp.printCode(0, "// %s\r\n", mShortDescription.c_str());
			}
			if (!mLongDescription.empty())
			{
				cp.printCode(0, "// %s\r\n", mLongDescription.c_str());
			}
			cp.printCode(0, "enum %s\r\n", mName.c_str());
			cp.printCode(0, "{\r\n");
			for (auto &i : mItems)
			{
				cp.printCode(1, "%s,",i.mName.c_str());
				cp.printCode(10, "// %s\r\n",
					i.mShortDescription.c_str());
			}
			cp.printCode(0, "};\r\n");
			cp.printCode(0, "\r\n");
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
					const char *type = getTypeString(i.mType.c_str());
					char temp[512];
					strncpy(temp, i.mType.c_str(),512);
					temp[0] = upcase(temp[0]);
					if (i.mIsPointer)
					{
						cp.printCode(0, "typedef std::vector< %s *> %sVector; // Forward declare the '%s' vector\r\n", type, temp, temp);
					}
					else
					{
						cp.printCode(0, "typedef std::vector< %s > %sVector; // Forward declare the '%s' vector\r\n", type, temp, temp);
					}
					arrays.push_back(i.mType);
				}
			}
		}

		cp.printCode(0, "\r\n");
		if (!mShortDescription.empty())
		{
			cp.printCode(0, "// %s\r\n", mShortDescription.c_str());
		}
		if (!mLongDescription.empty())
		{
			cp.printCode(0, "// %s\r\n", mLongDescription.c_str());
		}
		cp.printCode(0, "class %s", mName.c_str());
		if (!mInheritsFrom.empty())
		{
			cp.printCode(0, " : public %s", mInheritsFrom.c_str());
		}
		cp.printCode(0, "\r\n");
		cp.printCode(0, "{\r\n");

		cp.printCode(0, "public:\r\n");

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
		if (mAssignment)
		{
			cp.printCode(1, "// Declare the constructor.\r\n");
			cp.printCode(1, "%s()\r\n", mName.c_str());
			cp.printCode(1, "{\r\n");
			cp.printCode(1, "}\r\n");
			cp.printCode(0, "\r\n");

			cp.printCode(1, "// Declare the assignment constructor.\r\n");
			cp.printCode(1, "%s(", mName.c_str());
			bool needComma = false;
			for (auto &i : mItems)
			{
				if (needComma)
				{
					cp.printCode(0,",");
				}
				cp.printCode(0, "const %s &_%s", i.mType.c_str(), i.mName.c_str());
				needComma = true;
			}
			cp.printCode(0, ")\r\n");
			cp.printCode(1, "{\r\n");

			for (auto &i : mItems)
			{
				cp.printCode(2, "%s = _%s;\r\n", i.mName.c_str(), i.mName.c_str());
			}

			cp.printCode(1, "}\r\n");
			cp.printCode(0, "\r\n");
		}
		if (hasInheritedItemsWithDefaultValues)
		{
			cp.printCode(1, "// Declare the constructor.\r\n");
			cp.printCode(1, "%s()\r\n", mName.c_str());
			cp.printCode(1, "{\r\n");
			for (auto &i : mItems)
			{
				// If this is an 'inherited' data item. Don't clear it here
				// Because it was already handled in the initializer
				if (!i.mInheritsFrom.empty() && !i.mDefaultValue.empty())
				{
					cp.printCode(2, "%s::%s = %s;\r\n",
						i.mInheritsFrom.c_str(),
						i.mName.c_str(),
						getDefaultValueString(i.mDefaultValue.c_str()));
				}
			}
			cp.printCode(1, "};\r\n");
			cp.printCode(0, "\r\n");
		}

		// see if any items are an array of pointers...
		bool hasArrayOfPointers = false;
		bool hasPointers = false;
		for (auto &i : mItems)
		{
			if (i.mIsArray && i.mIsPointer )
			{
				hasArrayOfPointers = true;
			}
			else if (i.mIsPointer)
			{
				hasPointers = true;
			}
		}
		if (hasArrayOfPointers || hasPointers )
		{
			cp.printCode(0, "\r\n");
			cp.printCode(1, "// Declare the virtual destructor; cleanup any pointers or arrays of pointers\r\n");
			cp.printCode(1, "virtual ~%s()\r\n", mName.c_str());
			cp.printCode(1, "{\r\n");
			for (auto &i : mItems)
			{
				if (i.mIsArray && i.mIsPointer)
				{
					cp.printCode(2, "for (auto &i:%s) delete i; // Delete all of the object pointers in this array\r\n", i.mName.c_str());
				}
				else if (i.mIsPointer)
				{
					cp.printCode(2, "delete %s; // Delete this object\r\n", i.mName.c_str());
				}
			}
			cp.printCode(1, "}\r\n");
			cp.printCode(0, "\r\n");
		}
		else if (!mInheritsFrom.empty())
		{
			cp.printCode(0, "\r\n");
			cp.printCode(1, "// Declare the virtual destructor.\r\n");
			cp.printCode(1, "virtual ~%s()\r\n", mName.c_str());
			cp.printCode(1, "{\r\n");
			cp.printCode(1, "}\r\n");
			cp.printCode(0, "\r\n");
		}
		// create the deep copy constructors and such
		if ( hasArrayOfPointers || hasPointers )
		{
			cp.printCode(0, "\r\n");
			cp.printCode(1, "// Declare the deep copy constructor; handles copying pointers and pointer arrays\r\n");
			cp.printCode(1, "%s(const %s &other)\r\n", mName.c_str(), mName.c_str());
			cp.printCode(1, "{\r\n");
			cp.printCode(2, "*this = other;\r\n");
			cp.printCode(1, "}\r\n");
			cp.printCode(0, "\r\n");

			cp.printCode(0, "\r\n");
			cp.printCode(1, "// Declare the virtual clone method using a deep copy\r\n");
			if (mInheritsFrom.empty())
			{
				cp.printCode(1, "virtual %s* clone() const\r\n", mName.c_str());
			}
			else
			{
				cp.printCode(1, "virtual %s* clone() const override\r\n", mInheritsFrom.c_str());
			}
			cp.printCode(1, "{\r\n");
			cp.printCode(2, "return new %s(*this);\r\n", mName.c_str());
			cp.printCode(1, "}\r\n");
			cp.printCode(0, "\r\n");


			cp.printCode(1, "// Declare and implement the deep copy assignment operator\r\n");
			cp.printCode(1, "%s& operator=(const %s& other)\r\n", mName.c_str(), mName.c_str());
			cp.printCode(1, "{\r\n");
			cp.printCode(2, "if (this != &other )\r\n");
			cp.printCode(2, "{\r\n");

			if (!mInheritsFrom.empty())
			{
				cp.printCode(3, "%s::operator=(other);\r\n", mInheritsFrom.c_str());
			}

			for (auto &i : mItems)
			{
				if (i.mIsArray && i.mIsPointer)
				{
					cp.printCode(3, "for (auto &i:%s) delete i; // Delete all of the object pointers in this array\r\n", i.mName.c_str());
					cp.printCode(3, "%s.clear(); // Clear the current array\r\n", i.mName.c_str());
					cp.printCode(3, "for (auto &i:other.%s) %s.push_back( static_cast< %s *>(i->clone())); // Deep copy object pointers into the array\r\n", i.mName.c_str(), i.mName.c_str(), i.mType.c_str());
				}
				else if (i.mIsPointer)
				{
					cp.printCode(3, "delete %s; // delete any previous pointer.\r\n", i.mName.c_str());
					cp.printCode(3, "%s = static_cast<%s *>(%s->clone()); // perform the deep copy and assignment here\r\n", i.mName.c_str(), i.mType.c_str(), i.mName.c_str());
				}
				else if ( i.mInheritsFrom.empty() )
				{
					cp.printCode(3, "%s = other.%s;\r\n", i.mName.c_str(), i.mName.c_str());
				}
			}

			cp.printCode(2, "}\r\n");
			cp.printCode(2, "return *this;\r\n");
			cp.printCode(1, "}\r\n");
			cp.printCode(0, "\r\n");
		}
		
		if (mClone)
		{
			cp.printCode(1, "// Declare the clone method\r\n");
			cp.printCode(1, "virtual %s *clone() const = 0;\r\n", mName.c_str());
			cp.printCode(0, "\r\n");
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
				cp.printCode(1, "%s",vectorName);
			}
			else
			{
				cp.printCode(1, "%s",getTypeString(i.mType.c_str()));
			}

			if (i.mIsPointer && !i.mIsArray)
			{
				cp.printCode(4, "*%s",i.mName.c_str());
			}
			else
			{
				cp.printCode(4, "%s",i.mName.c_str());
			}

			if (i.mDefaultValue.empty())
			{
				if (i.mIsPointer && !i.mIsArray )
				{
					cp.printCode(0, "{ nullptr }");
				}
			}
			else
			{
				cp.printCode(0, "{ %s }", getDefaultValueString(i.mDefaultValue.c_str()));
			}

			cp.printCode(0, ";");

			cp.printCode(16, "// %s\r\n",i.mShortDescription.c_str());
		}

		cp.printCode(0, "};\r\n");

		cp.printCode(0, "\r\n");
	}

	std::string		mName;
	std::string		mType;
	std::string		mInheritsFrom;
	std::string		mEngineSpecific;
	std::string		mDefaultValue;
	std::string		mShortDescription;
	std::string		mLongDescription;
	bool			mClone{ false }; // if true, we need to declare the clone virtual method
	bool			mAssignment{ false };
	DataItemVector	mItems;
};

typedef std::vector< Object > ObjectVector;

class DOM
{
public:

	void saveCPP(CodePrinter &cp)
	{

		StringVector arrays;

		char temp[512];
		STRING_HELPER::stringFormat(temp, 512, "%s_H", mFilename);
		_strupr(temp);
		cp.printCode(0, "#ifndef %s\r\n", temp);
		cp.printCode(0, "#define %s\r\n", temp);
		cp.printCode(0, "\r\n");
		cp.printCode(0, "// CreateDOM: Schema Generation tool written by John W. Ratcliff, 2017\r\n");
		cp.printCode(0, "// Warning:This source file was auto-generated by the CreateDOM tool. Do not try to edit this source file manually!\r\n");
		cp.printCode(0, "// The Google DOCs Schema Spreadsheet for this source came from: %s\r\n", mURL.c_str());
		cp.printCode(0, "\r\n");
		cp.printCode(0, "#include <stdint.h>\r\n");
		cp.printCode(0, "#include <vector>\r\n");
		cp.printCode(0, "\r\n");
		cp.printCode(0, "\r\n");
		cp.printCode(0, "namespace %s\r\n", mNamespace.c_str());
		cp.printCode(0, "{\r\n");
		cp.printCode(0, "\r\n");

		for (auto &i : mObjects)
		{
			i.saveCPP(cp,arrays);
		}

		cp.printCode(0, "\r\n");
		cp.printCode(0, "\r\n");
		cp.printCode(0, "} // End of %s namespace\r\n", mNamespace.c_str());
		cp.printCode(0, "\r\n");
		cp.printCode(0, "#endif // End of %s\r\n", temp);

	}

	std::string		mNamespace;
	std::string		mFilename;
	std::string		mURL;
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
		if (mHaveObject)
		{
			mDOM.mObjects.push_back(mCurrentObject);
			mCurrentObject.clear();
			mHaveObject = false;
		}
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

		CodePrinter cp(fph);

		mDOM.saveCPP(cp);


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
		else if (_stricmp(argv[0], "URL") == 0)
		{
			if (argc >= 2)
			{
				mDOM.mURL = std::string(argv[1]);
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
							if (argc >= 6)
							{
								if ( _stricmp(argv[5],"CLONE") == 0 )
								{
									mCurrentObject.mClone = true;
								}
								if (_stricmp(argv[5], "ASSIGNMENT") == 0)
								{
									mCurrentObject.mAssignment = true;
								}
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