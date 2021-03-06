
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
#include "PhysicsDOMDef.h"


#else

#pragma warning(disable:4100)

namespace CREATE_DOM
{

	char lowerCase(char c)
	{
		if (c >= 'A' && c <= 'Z')
		{
			c += 32;
		}
		return c;
	}

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
	void memberNeedsReflection(const StringVector &needsReflectionClasses)
	{
		mNeedsReflection = false;
		if (mIsPointer || mIsString)
		{
			mNeedsReflection = true;
		}
		else
		{
			for (auto &i : needsReflectionClasses)
			{
				if (i == mType)
				{
					mNeedsReflection = true;
					break;
				}
			}
		}
	}
	bool needsReflection(void) const
	{
		bool ret = false;

		if (mIsArray || mIsPointer || mIsString)
		{
			ret = true;
		}

		return ret;
	}
	bool			mIsArray{ false }; // true if this data item is an array
	bool			mIsPointer{ false };
	bool			mIsString{ false };
	bool			mNeedsReflection{ false }; // determines where or not this refers to a data type that needs reflection
	std::string		mMember;	// name of this data item
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

const char *getTypeString(const char *type,bool isDef)
{
	const char *ret = type;

	if (strcmp(type, "string") == 0)
	{
		if (isDef)
		{
			ret = "std::string";
		}
		else
		{
			ret = "const char *";
		}
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

	const char *getPROTOTypeString(const char *type)
	{
		const char *ret = type;

		if (strcmp(type, "string") == 0)
		{
			ret = "string";
		}
		else if (strcmp(type, "u8") == 0)
		{
			ret = "uint32";
		}
		else if (strcmp(type, "u16") == 0)
		{
			ret = "uint32";
		}
		else if (strcmp(type, "u32") == 0)
		{
			ret = "uint32";
		}
		else if (strcmp(type, "u64") == 0)
		{
			ret = "uint64";
		}
		else if (strcmp(type, "i8") == 0)
		{
			ret = "int32";
		}
		else if (strcmp(type, "i16") == 0)
		{
			ret = "int32";
		}
		else if (strcmp(type, "i32") == 0)
		{
			ret = "int32";
		}
		else if (strcmp(type, "i64") == 0)
		{
			ret = "int64";
		}

		return ret;
	}

	const char *getClassNameString(const std::string &name,bool isDef)
	{
		const char *ret = name.c_str();
		static char scratch[512];
		if (isDef)
		{
			STRING_HELPER::stringFormat(scratch, 512, "%sDef", name.c_str());
			ret = scratch;
		}
		return ret;
	}

	const char *getMemberName(const std::string &name, bool isDef)
	{
		const char *ret = name.c_str();
		static char scratch[512];
		if (isDef)
		{
			char temp[512];
			strncpy(temp, name.c_str(), 512);
			temp[0] = upcase(temp[0]); // upper case the first character of the member variable name
			STRING_HELPER::stringFormat(scratch, 512, "m%s", temp);
			ret = scratch;
		}
		return ret;
	}

	bool classNeedsReflection(const std::string &className, const StringVector &needsReflection)
	{
		bool ret = false;

		for (auto &i : needsReflection)
		{
			if (i == className)
			{
				ret = true;
				break;
			}
		}
		return ret;
	}

	void saveCPP(CodePrinter &impl,
				 CodePrinter &dom,
				 StringVector &arrays,
				 const StringVector &_needsReflection,
				StringVector &cloneObjects)
	{


		if (_stricmp(mType.c_str(), "Enum") == 0)
		{
			// it's an enum...
			dom.printCode(0, "\r\n");
			if (!mShortDescription.empty())
			{
				dom.printCode(0, "// %s\r\n", mShortDescription.c_str());
			}
			if (!mLongDescription.empty())
			{
				dom.printCode(0, "// %s\r\n", mLongDescription.c_str());
			}
			dom.printCode(0, "enum %s\r\n", mName.c_str());
			dom.printCode(0, "{\r\n");
			for (auto &i : mItems)
			{
				dom.printCode(1, "%s,",i.mMember.c_str());
				dom.printCode(10, "// %s\r\n",
					i.mShortDescription.c_str());
			}
			dom.printCode(0, "};\r\n");
			dom.printCode(0, "\r\n");
			return;
		}

		bool hasInheritance = !mInheritsFrom.empty();
		bool hasArrays = false;
		bool hasPointer = false;
		bool hasStrings = false;


		// Before we can save out the class definition; we need to first see
		// if this class definition contains any arrays..
		for (auto &i : mItems)
		{
			if (strcmp(i.mType.c_str(), "string") == 0)
			{
				hasStrings = true;
			}
			if (i.mIsPointer)
			{
				hasPointer = true;
			}
			if (i.mIsArray) // ok.. this is an array of object!
			{
				hasArrays = true;
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
					const char *type = getTypeString(i.mType.c_str(),true);
					char temp[512];
					strncpy(temp, i.mType.c_str(),512);
					temp[0] = upcase(temp[0]);
					if (i.mIsPointer)
					{
						if (!i.mIsString)
						{
							impl.printCode(0, "typedef std::vector< %sDef *> %sVectorDef; // Forward declare the '%s' vector for the implementation object pointers\r\n", 
								type, 
								temp, 
								temp);

							impl.printCode(0, "typedef std::vector< %s *> %sVectorDOM; // Forward declare the '%s' vector for the implementation object pointers\r\n",
								type,
								temp,
								temp);

						}
					}
					else if ( !i.mIsString )
					{
						if (i.mNeedsReflection)
						{
							impl.printCode(0, "typedef std::vector< %sDef > %sVectorDef; // Forward declare the '%s' vector\r\n", type, temp, temp);
							impl.printCode(0, "typedef std::vector< %s > %sVectorDOM; // Forward declare the '%s' vector\r\n", type, temp, temp);
						}
						else
						{
							impl.printCode(0, "typedef std::vector< %s > %sVectorDef; // Forward declare the '%s' vector\r\n", type, temp, temp);
						}
					}
					arrays.push_back(i.mType);
				}
			}
		}
		bool needsDef = false;
		if (hasArrays || hasInheritance || hasPointer || hasStrings || mClone || mNeedsReflection)
		{
			needsDef = true;
		}
		auto classDefinition = [this](CodePrinter &cp, bool isDef,bool needsDeepCopy,const StringVector &_needsReflection,StringVector &cloneObjects)
		{
			cp.printCode(0, "\r\n");
			if (!mShortDescription.empty())
			{
				cp.printCode(0, "// %s\r\n", mShortDescription.c_str());
			}
			if (!mLongDescription.empty())
			{
				cp.printCode(0, "// %s\r\n", mLongDescription.c_str());
			}
			cp.printCode(0, "class %s", getClassNameString(mName,isDef)); 
			bool firstInherit = true;
			if (!mInheritsFrom.empty() )
			{
				firstInherit = false;
				cp.printCode(0, " : public %s", getClassNameString(mInheritsFrom.c_str(),isDef));
			}

			if ((mClone || mNeedsReflection) && isDef )
			{
				bool doCloneObject = true;
				// Don't add this if we inherit from an object which already has clone object!
				if (!mInheritsFrom.empty())
				{
					for (auto &i:cloneObjects)
					{
						if (i == mInheritsFrom)
						{
							doCloneObject = false;
							break;
						}
					}
				}
				if (doCloneObject)
				{
					cp.printCode(0, "%s public CloneObject", firstInherit ? ":" : ",");
					cloneObjects.push_back(mName);
				}
			}

			cp.printCode(0, "\r\n");
			cp.printCode(0, "{\r\n");

			cp.printCode(0, "public:\r\n");
		};

		bool needsDeepCopy = mInheritsFrom.empty() ? false : true;
		if ( !needsDeepCopy )
		{
			needsDeepCopy = mClone | mNeedsReflection;
			if (!needsDeepCopy)
			{
				for (auto &i : mItems)
				{
					if (i.mIsArray && i.mIsPointer)
					{
						needsDeepCopy = true;
						break;
					}
					else if (i.mIsPointer)
					{
						needsDeepCopy = true;
						break;
					}
				}
			}
		}

		// If we need an implementation class
		if (needsDef)
		{
			classDefinition(impl, true, needsDeepCopy,_needsReflection,cloneObjects);
		}
		classDefinition(dom, false, needsDeepCopy,_needsReflection,cloneObjects);

		// Defementation of the class body..
		auto classBody = [this](CodePrinter &cp, bool isDef, bool needsDeepCopy, const StringVector &_needsReflection)
		{
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
			bool haveDefaultConstructor = false;
			if (mAssignment)
			{
				haveDefaultConstructor = true;
				cp.printCode(1, "// Declare the constructor.\r\n");
				cp.printCode(1, "%s() { }\r\n", getClassNameString(mName, isDef));
				cp.printCode(0, "\r\n");

				cp.printCode(1, "// Declare the assignment constructor.\r\n");
				cp.printCode(1, "%s(", getClassNameString(mName, isDef));
				bool needComma = false;
				for (auto &i : mItems)
				{
					if (needComma)
					{
						cp.printCode(0, ",");
					}
					cp.printCode(0, "const %s &_%s", i.mType.c_str(), getMemberName(i.mMember, isDef));
					needComma = true;
				}
				cp.printCode(0, ")\r\n");
				cp.printCode(1, "{\r\n");

				for (auto &i : mItems)
				{
					cp.printCode(2, "%s = _%s;\r\n", getMemberName(i.mMember, isDef), getMemberName(i.mMember, isDef));
				}

				cp.printCode(1, "}\r\n");
				cp.printCode(0, "\r\n");
			}

			if (hasInheritedItemsWithDefaultValues)
			{
				if (!haveDefaultConstructor)
				{
					haveDefaultConstructor = true;
					cp.printCode(1, "// Declare the constructor.\r\n");
					cp.printCode(1, "%s()\r\n", getClassNameString(mName, isDef));
					cp.printCode(1, "{\r\n");
					{
						for (auto &i : mItems)
						{
							// If this is an 'inherited' data item. Don't clear it here
							// Because it was already handled in the initializer
							if (!i.mInheritsFrom.empty() && !i.mDefaultValue.empty())
							{
								cp.printCode(2, "%s%s::%s = %s;\r\n",
									i.mInheritsFrom.c_str(),
									isDef ? "Def" : "",
									getMemberName(i.mMember, isDef),
									getDefaultValueString(i.mDefaultValue.c_str()));
							}
						}
					}
					cp.printCode(1, "}\r\n");
					cp.printCode(0, "\r\n");
				}
			}

			// see if any items are an array of pointers...
			bool hasArrayOfPointers = false;
			bool hasPointers = false;
			for (auto &i : mItems)
			{
				if (i.mIsArray && i.mIsPointer)
				{
					hasArrayOfPointers = true;
				}
				else if (i.mIsPointer)
				{
					hasPointers = true;
				}
			}
			if (!haveDefaultConstructor && isDef)
			{
				haveDefaultConstructor = true;
				cp.printCode(0, "\r\n");
				cp.printCode(1, "// Declare the constructor.\r\n");
				cp.printCode(1, "%s() { }\r\n", getClassNameString(mName, isDef));
				cp.printCode(0, "\r\n");
			}
			if (hasArrayOfPointers || hasPointers)
			{
				if (needsDeepCopy)
				{
					if (isDef)
					{
						cp.printCode(0, "\r\n");
						cp.printCode(1, "// Declare the virtual destructor; cleanup any pointers or arrays of pointers\r\n");
						cp.printCode(1, "virtual ~%s()\r\n", getClassNameString(mName, isDef));
						cp.printCode(1, "{\r\n");

						for (auto &i : mItems)
						{
							if (i.mIsArray && i.mIsPointer)
							{
								cp.printCode(2, "for (auto &i:%s) delete i; // Delete all of the object pointers in this array\r\n", getMemberName(i.mMember, isDef));
							}
							else if (i.mIsPointer)
							{
								cp.printCode(2, "delete %s; // Delete this object\r\n", getMemberName(i.mMember, isDef));
							}
						}
						cp.printCode(1, "}\r\n");
						cp.printCode(0, "\r\n");
					}
				}
			}
			else if (!mInheritsFrom.empty())
			{
				if (needsDeepCopy && isDef )
				{
					cp.printCode(0, "\r\n");
					cp.printCode(1, "// Declare the virtual destructor.\r\n");
					cp.printCode(1, "virtual ~%s()\r\n", getClassNameString(mName, isDef));
					cp.printCode(1, "{\r\n");
					cp.printCode(1, "}\r\n");
					cp.printCode(0, "\r\n");
				}
			}
			// create the deep copy constructors and such
			if (needsDeepCopy)
			{
				// Do the deep copy constructor and assignment operators
				if (isDef)
				{
					cp.printCode(0, "\r\n");
					cp.printCode(1, "// Declare the deep copy constructor; handles copying pointers and pointer arrays\r\n");
					cp.printCode(1, "%s(const %s &other)\r\n", getClassNameString(mName, isDef), getClassNameString(mName, isDef));
					cp.printCode(1, "{\r\n");
					cp.printCode(2, "*this = other;\r\n");
					cp.printCode(1, "}\r\n");
					cp.printCode(0, "\r\n");

					if (mInheritsFrom.empty())
					{
						cp.printCode(0, "\r\n");
						cp.printCode(1, "virtual %s * get%s(void) // Declare virtual method to return DOM version of base class.\r\n", mName.c_str(), mName.c_str());
						cp.printCode(1, "{\r\n");
						cp.printCode(2, "return &mDOM; // return the address of the DOM.\r\n");
						cp.printCode(1, "}\r\n");
						cp.printCode(0, "\r\n");
					}
					else
					{
						cp.printCode(0, "\r\n");
						cp.printCode(1, "virtual %s * get%s(void) // Declare virtual method to return DOM version of base class.\r\n", mInheritsFrom.c_str(), mInheritsFrom.c_str());
						cp.printCode(1, "{\r\n");
						cp.printCode(2, "return &mDOM; // return the address of the DOM.\r\n", mInheritsFrom.c_str());
						cp.printCode(1, "}\r\n");
						cp.printCode(0, "\r\n");

						cp.printCode(0, "\r\n");
						cp.printCode(1, "virtual %s * get%s(void) // Declare virtual method to return the DOM version\r\n", mName.c_str(), mName.c_str());
						cp.printCode(1, "{\r\n");
						cp.printCode(2, "return &mDOM; // return the address of the DOM.\r\n");
						cp.printCode(1, "}\r\n");
						cp.printCode(0, "\r\n");

						if (!mMultipleInherticance.empty())
						{
							cp.printCode(0, "\r\n");
							cp.printCode(1, "virtual %s * get%s(void) // Declare virtual method to return the DOM version based on multiple inheritance\r\n", mMultipleInherticance.c_str(), mMultipleInherticance.c_str());
							cp.printCode(1, "{\r\n");
							cp.printCode(2, "return &mDOM; // return the address of the DOM.\r\n");
							cp.printCode(1, "}\r\n");
							cp.printCode(0, "\r\n");
						}

					}

					// **********************************************
					// Start of the clone code
					// **********************************************
					cp.printCode(0, "\r\n");
					cp.printCode(1, "// Declare the virtual clone method using a deep copy\r\n");
					if (mInheritsFrom.empty())
					{
						cp.printCode(1, "virtual CloneObject* clone() const\r\n");
					}
					else
					{
						cp.printCode(1, "virtual CloneObject* clone() const override\r\n");
					}
					cp.printCode(1, "{\r\n");
					cp.printCode(2, "return new %s(*this);\r\n", getClassNameString(mName, isDef));
					cp.printCode(1, "}\r\n");
					cp.printCode(0, "\r\n");


					cp.printCode(1, "// Declare and implement the deep copy assignment operator\r\n");
					cp.printCode(1, "%s& operator=(const %s& other)\r\n", getClassNameString(mName, isDef), getClassNameString(mName, isDef));
					cp.printCode(1, "{\r\n");
					cp.printCode(2, "if (this != &other )\r\n");
					cp.printCode(2, "{\r\n");

					if (!mInheritsFrom.empty())
					{
						cp.printCode(3, "%sDef::operator=(other);\r\n", mInheritsFrom.c_str());
					}

					for (auto &i : mItems)
					{
						if (i.mIsArray && i.mIsPointer)
						{
							cp.printCode(3, "for (auto &i:%s) delete i; // Delete all of the object pointers in this array\r\n", getMemberName(i.mMember, isDef));
							cp.printCode(3, "%s.clear(); // Clear the current array\r\n", getMemberName(i.mMember, isDef));
							cp.printCode(3, "%s.reserve(other.%s.size()); // Reserve number of items for the new array\r\n", getMemberName(i.mMember, isDef), getMemberName(i.mMember, isDef));
							cp.printCode(3, "for (auto &i:other.%s) %s.push_back( static_cast< %sDef *>(i->clone())); // Deep copy object pointers into the array\r\n", getMemberName(i.mMember, isDef), getMemberName(i.mMember, isDef), i.mType.c_str());
						}
						else if (i.mIsPointer)
						{
							cp.printCode(3, "delete %s; // delete any previous pointer.\r\n", getMemberName(i.mMember, isDef));
							cp.printCode(3, "%s = nullptr; // set the pointer to null.\r\n", getMemberName(i.mMember, isDef));
							cp.printCode(3, "if ( other.%s )\r\n", getMemberName(i.mMember, isDef));
							cp.printCode(3, "{\r\n");
							cp.printCode(4, "%s = static_cast<%sDef *>(other.%s->clone()); // perform the deep copy and assignment here\r\n",
								getMemberName(i.mMember, isDef),
								i.mType.c_str(),
								getMemberName(i.mMember, isDef));
							cp.printCode(3, "}\r\n");
						}
						else if (i.mInheritsFrom.empty())
						{
							cp.printCode(3, "%s = other.%s;\r\n", getMemberName(i.mMember, isDef), getMemberName(i.mMember, isDef));
						}
					}

					cp.printCode(2, "}\r\n");
					cp.printCode(2, "return *this;\r\n");
					cp.printCode(1, "}\r\n");
					cp.printCode(0, "\r\n");
				}
				// **********************************************
				// End of the clone code
				// **********************************************
			}
			if ( isDef )
			{
#if 0
				// Get DOM code
				{
					cp.printCode(0, "\r\n");
					cp.printCode(1, "// Declare the helper method to return the DOM version of this class.\r\n");
					cp.printCode(1, "%s *getDOM(void)\r\n", getClassNameString(mName, false));
					cp.printCode(1, "{\r\n");
					cp.printCode(2, " return &mDOM;\r\n");
					cp.printCode(1, "}\r\n");
					cp.printCode(0, "\r\n");
				}
#endif
				// initDOM code
				{
					cp.printCode(1, "// Declare and implement the initDOM method\r\n");
					cp.printCode(1, "virtual void initDOM(void) override\r\n", getClassNameString(mName, isDef), getClassNameString(mName, isDef));
					cp.printCode(1, "{\r\n");

					if (!mInheritsFrom.empty())
					{
						cp.printCode(2, "// Initialize the DOM for the base class.\r\n");
						cp.printCode(2, "%sDef::initDOM();\r\n", mInheritsFrom.c_str());
						cp.printCode(2, "// Copy the elements from the base class DOM to our reflection DOM\r\n");
						cp.printCode(2, "{\r\n");
						cp.printCode(3, "%s *dom = static_cast< %s *>(&mDOM); // Get the DOM base class.\r\n",
							mInheritsFrom.c_str(),
							mInheritsFrom.c_str());
						cp.printCode(3, "*dom = *(%sDef::get%s()); // Assign the base class DOM components.\r\n",
							mInheritsFrom.c_str(),
							mInheritsFrom.c_str());
						cp.printCode(2, "}\r\n");
					}
					if (strcmp(mName.c_str(), "Node") == 0)
					{
						printf("debug me");
					}
					for (auto &i : mItems)
					{
						const char *arrayPostFix = i.mNeedsReflection ? "DOM" : "";
						if (!i.needsReflection() && classNeedsReflection(i.mType, _needsReflection))
						{
							cp.printCode(2, "{\r\n");
							cp.printCode(3, "%sDef *impl = static_cast< %sDef *>(&%s); // static cast to the implementation class.\r\n",
								i.mType.c_str(),
								i.mType.c_str(),
								getMemberName(i.mMember, true));
							cp.printCode(3, "impl->initDOM(); // Initialize DOM components of member variable.\r\n");
							cp.printCode(3, "mDOM.%s = *impl->get%s(); // Copy the DOM struct values.\r\n",
								i.mMember.c_str(),
								i.mType.c_str());
							cp.printCode(2, "}\r\n");
						}
						else if (i.mIsString)
						{
							if (i.mIsArray)
							{
								arrayPostFix = "Def";
								cp.printCode(2, "// Initialize the const char * array from the array of std::strings vector %s\r\n",
									getMemberName(i.mMember, true));
								cp.printCode(2, "%sDef.reserve(%s.size()); // Reserve room for string pointers.\r\n",
									getMemberName(i.mMember, true),
									getMemberName(i.mMember, true));
								cp.printCode(2, "for (auto &i: %s) // For each std::string\r\n", getMemberName(i.mMember, true));
								cp.printCode(3, "%sDef.push_back( i.c_str() ); // Add the const char * for the string.\r\n", getMemberName(i.mMember, true));
								cp.printCode(2, "mDOM.%sCount = uint32_t(%s%s.size()); // Assign the number of strings\r\n",
									getMemberName(i.mMember, false),
									getMemberName(i.mMember, true),
									arrayPostFix);

								cp.printCode(2, "mDOM.%s = mDOM.%sCount ? &%s%s[0] : nullptr; // Assign the pointer array.\r\n",
									getMemberName(i.mMember, false),
									getMemberName(i.mMember, false),
									getMemberName(i.mMember, true),
									arrayPostFix);
							}
							else
							{
								cp.printCode(2, "mDOM.%s = %s.c_str(); // Assign the current string pointer.\r\n", 
									getMemberName(i.mMember, false), 
									getMemberName(i.mMember, true));
							}
						}
						else if (i.mIsArray && i.mIsPointer)
						{
							// If it is an array of pointers...
							cp.printCode(2, "%sDOM.clear();\r\n", getMemberName(i.mMember, true));
							cp.printCode(2, "%sDOM.reserve( %s.size() );\r\n",
								getMemberName(i.mMember, true),
								getMemberName(i.mMember, true));
							cp.printCode(2, "for (auto &i:%s)\r\n", getMemberName(i.mMember, true));
							cp.printCode(2, "{\r\n");
							cp.printCode(3, "i->initDOM();\r\n");
							cp.printCode(3, "%sDOM.push_back( i->get%s() );\r\n",
								getMemberName(i.mMember, true),
								i.mType.c_str());
							cp.printCode(2, "}\r\n");
							cp.printCode(2, "mDOM.%sCount = uint32_t(%s%s.size()); // assign the number of items in the array.\r\n",
								getMemberName(i.mMember, false),
								getMemberName(i.mMember, true),
								arrayPostFix);
							cp.printCode(2, "mDOM.%s = mDOM.%sCount ? &%s%s[0] : nullptr; // Assign the pointer array\r\n",
								getMemberName(i.mMember, false),
								getMemberName(i.mMember, false),
								getMemberName(i.mMember, true),
								arrayPostFix);
						}
						else if (i.mIsArray)
						{

							// If the array needs a deep copy..
							if (i.mNeedsReflection )
							{
								cp.printCode(2, "%sDOM.clear();\r\n", getMemberName(i.mMember, true));
								cp.printCode(2, "%sDOM.reserve( %s.size() );\r\n",
									getMemberName(i.mMember, true),
									getMemberName(i.mMember, true));
								cp.printCode(2, "for (auto &i:%s)\r\n", getMemberName(i.mMember, true));
								cp.printCode(2, "{\r\n");
								cp.printCode(3, "i.initDOM();\r\n");
								cp.printCode(3, "%sDOM.push_back( *(i.get%s()) );\r\n",
									getMemberName(i.mMember, true),
									i.mType.c_str());
								cp.printCode(2, "}\r\n");
							}


							cp.printCode(2, "mDOM.%sCount = uint32_t(%s%s.size()); // assign the number of items in the array.\r\n",
								getMemberName(i.mMember, false),
								getMemberName(i.mMember, true),
								arrayPostFix);
							cp.printCode(2, "mDOM.%s = mDOM.%sCount ? &%s%s[0] : nullptr; // Assign the pointer array\r\n",
								getMemberName(i.mMember, false),
								getMemberName(i.mMember, false),
								getMemberName(i.mMember, true),
								arrayPostFix);
						}
						else if (i.mIsPointer)
						{
							cp.printCode(2, "if ( %s )\r\n", getMemberName(i.mMember, true));
							cp.printCode(2, "{\r\n");
							cp.printCode(3, "%s->initDOM(); // Initialize any DOM components of this object.\r\n",
								getMemberName(i.mMember, true));

							cp.printCode(3, "mDOM.%s = %s->get%s(); // assign the DOM reflection pointer.\r\n",
								getMemberName(i.mMember, false),
								getMemberName(i.mMember, true),
								i.mType.c_str());

							cp.printCode(2, "}\r\n");
						}
						else
						{
							cp.printCode(2, "mDOM.%s = %s; // Simple member variable assignment to the DOM reflection: %s\r\n",
								getMemberName(i.mMember, false),
								getMemberName(i.mMember, true),
								getMemberName(i.mMember, false));
						}
					}
					cp.printCode(1, "}\r\n");
					cp.printCode(0, "\r\n");
				}



				// Do the move constructor and assignment operators
				if ( isDef )
				{
					cp.printCode(0, "\r\n");
					cp.printCode(1, "// Declare the move constructor; handles copying pointers and pointer arrays\r\n");
					cp.printCode(1, "%s(%s &&other)\r\n", getClassNameString(mName,isDef), getClassNameString(mName,isDef));
					cp.printCode(1, "{\r\n");
					cp.printCode(2, "*this = std::move(other);\r\n");
					cp.printCode(1, "}\r\n");
					cp.printCode(0, "\r\n");

					cp.printCode(1, "// Declare and implement the move assignment operator\r\n");
					cp.printCode(1, "%s& operator=(%s&& other)\r\n", getClassNameString(mName,isDef), getClassNameString(mName,isDef));
					cp.printCode(1, "{\r\n");
					cp.printCode(2, "if (this != &other )\r\n");
					cp.printCode(2, "{\r\n");

					if (!mInheritsFrom.empty())
					{
						cp.printCode(3, "%sDef::operator=(std::move(other));\r\n", mInheritsFrom.c_str());
					}

					for (auto &i : mItems)
					{
						if (i.mIsArray && i.mIsPointer)
						{
							cp.printCode(3, "%s = other.%s;\r\n", getMemberName(i.mMember,isDef), getMemberName(i.mMember,isDef));
							cp.printCode(3, "other.%s.clear(); // Clear the 'other' array now that we have moved it\r\n", getMemberName(i.mMember, isDef));
						}
						else if (i.mIsPointer)
						{
							cp.printCode(3, "%s = other.%s;\r\n", getMemberName(i.mMember, isDef), getMemberName(i.mMember, isDef));
							cp.printCode(3, "other.%s = nullptr; // Set 'other' pointer to null since we have moved it\r\n", getMemberName(i.mMember, isDef));
						}
						else if (i.mInheritsFrom.empty())
						{
							cp.printCode(3, "%s = other.%s;\r\n", getMemberName(i.mMember, isDef), getMemberName(i.mMember, isDef));
						}
					}
					cp.printCode(2, "}\r\n");
					cp.printCode(2, "return *this;\r\n");
					cp.printCode(1, "}\r\n");
					cp.printCode(0, "\r\n");
				}
			}

			bool needsDOMVector = false; // True if we need to declare the DOM vector
			for (auto &i : mItems)
			{
				// If this is an 'inherited' data item. Don't clear it here
				// Because it was already handled in the initializer
				if (!i.mInheritsFrom.empty())
				{
					continue;
				}

				bool needsNull = false;

				// Output the member variable declaration.
				if (i.mIsArray)
				{
					if (isDef)
					{
						if (i.mIsString)
						{
							cp.printCode(1, "StringVector");
							needsDOMVector = true;
						}
						else
						{
							needsDOMVector = true;
							char temp[512];
							strncpy(temp, i.mType.c_str(), 512);
							temp[0] = upcase(temp[0]);
							char vectorName[512];
							STRING_HELPER::stringFormat(vectorName, 512, "%sVectorDef", temp);
							cp.printCode(1, "%s", vectorName);
						}
					}
					else
					{
						needsNull = true;
						cp.printCode(1, "uint32_t");
						cp.printCode(4, "%sCount { 0 };\r\n", getMemberName(i.mMember,isDef) );
						if (i.mIsString && !isDef)
						{
							cp.printCode(1, "%s*", getTypeString(i.mType.c_str(), isDef));
						}
						else
						{
							if (i.mIsPointer)
							{
								cp.printCode(1, "%s**", getTypeString(i.mType.c_str(), isDef));
							}
							else
							{
								cp.printCode(1, "%s*", getTypeString(i.mType.c_str(), isDef));
							}
						}
					}
				}
				else
				{
					if (isDef && i.mIsPointer && !i.mIsArray)
					{
						needsNull = true;
						cp.printCode(1, "%s%s", getTypeString(i.mType.c_str(), isDef),
							isDef ? "Def" : "");
					}
					else
					{
						if (i.mIsString && !isDef )
						{
							needsNull = true;
						}
						if (i.mNeedsReflection && !i.mIsString && isDef )
						{
							cp.printCode(1, "%sDef", getTypeString(i.mType.c_str(), isDef));
						}
						else
						{
							cp.printCode(1, "%s", getTypeString(i.mType.c_str(), isDef));
						}
					}
				}

				if (i.mIsPointer && !i.mIsArray)
				{
					needsNull = true;
					cp.printCode(4, "*%s", getMemberName(i.mMember, isDef));
				}
				else
				{
					cp.printCode(4, "%s", getMemberName(i.mMember, isDef));
				}

				if (i.mDefaultValue.empty())
				{
					if ( needsNull )
					{
						cp.printCode(0, "{ nullptr }");
					}
				}
				else
				{
					cp.printCode(0, "{ %s }", getDefaultValueString(i.mDefaultValue.c_str()));
				}

				cp.printCode(0, ";");

				cp.printCode(16, "// %s\r\n", i.mShortDescription.c_str());

			}
			if (isDef)
			{
				cp.printCode(0, "private:\r\n");
				cp.printCode(1, "%s", getClassNameString(mName, false));
				cp.printCode(4, "mDOM; // Declare the DOM version.\r\n");
			}
			if (needsDOMVector)
			{
				cp.printCode(0, "// Declare private temporary array(s) to hold flat DOM version of arrays.\r\n");

				for (auto &i : mItems)
				{
					// If this is an 'inherited' data item. Don't clear it here
					// Because it was already handled in the initializer
					if (!i.mInheritsFrom.empty())
					{
						continue;
					}

					if (!i.needsReflection() && isDef)
						continue;

					// Output the member variable declaration.
					if (i.mIsArray)
					{
						if (isDef)
						{
							if (i.mIsString)
							{
								cp.printCode(1, "ConstCharVector");
								cp.printCode(4, "%sDef; // Scratch array for const char pointers.\r\n",
									getMemberName(i.mMember, true));
							}
							else
							{
								if (i.mNeedsReflection)
								{
									char temp[512];
									strncpy(temp, i.mType.c_str(), 512);
									temp[0] = upcase(temp[0]);
									char vectorName[512];
									STRING_HELPER::stringFormat(vectorName, 512, "%sVectorDOM", temp);
									cp.printCode(1, "%s", vectorName);
									cp.printCode(4, "%sDOM; // Scratch array for const char pointers.\r\n",
										getMemberName(i.mMember, true));
								}
							}
						}
					}
				}
			}
		};

		if (needsDef)
		{
			classBody(impl, true, needsDeepCopy,_needsReflection);
		}
		classBody(dom, false, needsDeepCopy,_needsReflection);

		auto endClass = [this](CodePrinter &cp)
		{
			cp.printCode(0, "};\r\n");
			cp.printCode(0, "\r\n");
		};
		if (needsDef)
		{
			endClass(impl);
		}
		endClass(dom);
	}

	void savePROTO(CodePrinter &cp, StringVector &arrays)
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
			uint32_t id = 0;
			for (auto &i : mItems)
			{
				cp.printCode(1, "%s = %d;", i.mMember.c_str(), id);
				cp.printCode(10, "// %s\r\n",
					i.mShortDescription.c_str());
				id++;
			}

			cp.printCode(0, "}\r\n");
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
		cp.printCode(0, "message %s", mName.c_str());
		// TODO TODO TODOO
		if (!mInheritsFrom.empty())
		{
			//			cp.printCode(0, " : public %s", mInheritsFrom.c_str());
		}
		cp.printCode(0, "\r\n");
		cp.printCode(0, "{\r\n");

		bool hasInheritedItemsWithDefaultValues = false;
		uint32_t id = 1;

		for (auto &i : mItems)
		{
			// If this is an 'inherited' data item. Don't clear it here
			// Because it was already handled in the initializer
			if (!i.mInheritsFrom.empty() && !i.mDefaultValue.empty())
			{
				hasInheritedItemsWithDefaultValues = true;
				continue;
			}
			const char *repeated = "";
			if (i.mIsArray)
			{
				repeated = "repeated ";
			}
			cp.printCode(1, "%s%s %s = %d;\r\n",
				repeated,
				getPROTOTypeString(i.mType.c_str()),
				i.mMember.c_str(),
				id);
			id++;
		}

		if (!mChildren.empty())
		{
			cp.printCode(1, "oneof subtype\r\n");
			cp.printCode(1, "{\r\n");
			for (auto &k : mChildren)
			{
				char scratch[512];
				strncpy(scratch, k.c_str(), 512);
				scratch[0] = lowerCase(scratch[0]);
				cp.printCode(2, "%s %s = %d;\r\n", k.c_str(), scratch, id);
				id++;
			}
			cp.printCode(1, "}\r\n");
			cp.printCode(0, "\r\n");
		}

		cp.printCode(0, "}\r\n");

		cp.printCode(0, "\r\n");


	}

	void saveExportXML(CodePrinter &cph,CodePrinter &cpp, StringVector &arrays)
	{
		// Save the Export XML code
	}

	// If any of our member variables need reflection, then anyone declaring this object
	// will also need reflection
	void computeNeedsReflection(StringVector &needsReflection)
	{
		mNeedsReflection = false;
		for (auto &i : mItems)
		{
			if (i.needsReflection())
			{
				mNeedsReflection = true;
				break;
			}
		}
		if (mNeedsReflection)
		{
			needsReflection.push_back(mName);
		}
	}

	void computeReflectionMembers(StringVector &needsReflection)
	{
		for (auto &i : mItems)
		{
			i.memberNeedsReflection(needsReflection);
		}
	}

	std::string		mName;
	std::string		mType;
	std::string		mInheritsFrom;
	std::string		mMultipleInherticance;
	std::string		mEngineSpecific;
	std::string		mDefaultValue;
	std::string		mShortDescription;
	std::string		mLongDescription;
	StringVector	mChildren;			// Classes which inherit from this class
	bool			mClone{ false }; // if true, we need to declare the clone virtual method
	bool			mAssignment{ false };
	bool			mNeedsReflection{ false };
	DataItemVector	mItems;
};

typedef std::vector< Object > ObjectVector;

class DOM
{
public:

	void importComplete(void)
	{
		StringVector needsReflection;
		for (auto &i : mObjects)
		{
			i.computeNeedsReflection(needsReflection);
		}
		for (auto &i : mObjects)
		{
			i.computeReflectionMembers(needsReflection);
		}
		// compute multiple inheritance requirements...
		for (auto &i : mObjects)
		{
			if (!i.mInheritsFrom.empty())
			{
				for (auto &j : mObjects)
				{
					if (i.mInheritsFrom == j.mName)
					{
						if (!j.mInheritsFrom.empty())
						{
							i.mMultipleInherticance = j.mInheritsFrom;
						}
					}
				}
			}
		}

	}

	void savePROTO(CodePrinter &cp)
	{
		StringVector arrays;

		cp.printCode(0, "// CreateDOM: Schema Generation tool written by John W. Ratcliff, 2017\r\n");
		cp.printCode(0, "// Warning:This source file was auto-generated by the CreateDOM tool. Do not try to edit this source file manually!\r\n");
		cp.printCode(0, "// The Google DOCs Schema Spreadsheet for this source came from: %s\r\n", mURL.c_str());
		cp.printCode(0, "\r\n");
		cp.printCode(0, "// [START declaration]\r\n");
		cp.printCode(0, "syntax = \"proto3\";\r\n");
		cp.printCode(0, "package CreateDOM;\r\n");
		cp.printCode(0, "// [END declaration]\r\n");
		cp.printCode(0, "\r\n");
		cp.printCode(0, "// [START messages]\r\n");
		cp.printCode(0, "\r\n");

		for (auto &i : mObjects)
		{
			i.savePROTO(cp, arrays);
		}

		cp.printCode(0, "\r\n");
		cp.printCode(0, "// [END messages]\r\n");
		cp.printCode(0, "\r\n");

	}

	void saveCPP(CodePrinter &impl,CodePrinter &dom)
	{

		StringVector arrays;
		StringVector needsReflection;
		for (auto &i : mObjects)
		{
			if (i.mNeedsReflection)
			{
				needsReflection.push_back(i.mName);
			}
		}

		auto headerBegin = [this](CodePrinter &cp,bool isDef)
		{
			char temp[512];
			STRING_HELPER::stringFormat(temp, 512, "%s%s_H", mFilename.c_str(), isDef ? "_IMPL" : "");
			_strupr(temp);
			cp.printCode(0, "#ifndef %s\r\n", temp);
			cp.printCode(0, "#define %s\r\n", temp);
			cp.printCode(0, "\r\n");
			cp.printCode(0, "// CreateDOM: Schema Generation tool written by John W. Ratcliff, 2017\r\n");
			cp.printCode(0, "// Warning:This source file was auto-generated by the CreateDOM tool. Do not try to edit this source file manually!\r\n");
			cp.printCode(0, "// The Google DOCs Schema Spreadsheet for this source came from: %s\r\n", mURL.c_str());
			cp.printCode(0, "\r\n");
			if (isDef)
			{
				STRING_HELPER::stringFormat(temp, 512, "%s.h", mFilename.c_str());
				cp.printCode(0, "#include \"%s\"\r\n", temp);
				cp.printCode(0, "#include <string>\r\n");
				cp.printCode(0, "#include <vector>\r\n");
			}
			cp.printCode(0, "#include <stdint.h>\r\n");
			cp.printCode(0, "\r\n");
			cp.printCode(0, "\r\n");
			cp.printCode(0, "namespace %s\r\n", mNamespace.c_str());
			cp.printCode(0, "{\r\n");
			cp.printCode(0, "\r\n");

			if (isDef)
			{
				cp.printCode(0, "\r\n");
				cp.printCode(0, "// Forward declare the two types of string vector containers.\r\n");
				cp.printCode(0, "typedef std::vector< std::string > StringVector;\r\n");
				cp.printCode(0, "typedef std::vector< const char * > ConstCharVector;\r\n");
				cp.printCode(0, "\r\n");
				cp.printCode(0, "// Declare the clone-object class for deep copies\r\n");
				cp.printCode(0, "// of objects by the implementation classes\r\n");
				cp.printCode(0, "// Not to be used with the base DOM classes;\r\n");
				cp.printCode(0, "// they do not support deep copies\r\n");
				cp.printCode(0, "// Also declares the virtual method to init the DOM contents.\r\n");
				cp.printCode(0, "class CloneObject\r\n");
				cp.printCode(0, "{\r\n");
				cp.printCode(0, "public:\r\n");
				cp.printCode(1, "// Declare the default virtual clone method; not implemented for DOM objects; only used for the implementation versions.\r\n");
				cp.printCode(1, "virtual CloneObject *clone(void) const { return nullptr; };\r\n");
				cp.printCode(1, "// Declare the default initDOM method; which is only needed for some implementation objects.\r\n");
				cp.printCode(1, "virtual void initDOM(void) {  };\r\n");
				cp.printCode(0, "};\r\n");
			}

		};

		headerBegin(impl, true);
		headerBegin(dom, false);
		StringVector cloneObjects;
		for (auto &i : mObjects)
		{
			i.saveCPP(impl,dom,arrays,needsReflection,cloneObjects);
		}

		auto headerEnd = [this](CodePrinter &cp, bool isDef)
		{
			char temp[512];
			cp.printCode(0, "\r\n");
			cp.printCode(0, "\r\n");
			cp.printCode(0, "} // End of %s namespace\r\n", mNamespace.c_str());
			cp.printCode(0, "\r\n");
			cp.printCode(0, "#endif // End of %s\r\n", temp);
		};
		headerEnd(impl, true);
		headerEnd(dom, false);

	}

	std::string		mNamespace;
	std::string		mFilename;
	std::string		mURL;
	std::string		mExportXML;
	ObjectVector	mObjects;
};

class CreateDOMDef : public CreateDOM
{
public:
	CreateDOMDef(void)
	{

	}
	virtual ~CreateDOMDef(void)
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
		//
		mDOM.importComplete();
		//
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
		STRING_HELPER::stringFormat(scratch, 512, "%sDef.h", mDOM.mFilename.c_str());
		FILE *fimpl = fopen(scratch, "wb");
		if (fimpl == nullptr)
		{
			printf("Failed to open output file: %s for write access.\r\n", scratch);
			return;
		}
		printf("Saving C++ DOM Defementation to: %s\r\n", scratch);

		STRING_HELPER::stringFormat(scratch, 512, "%s.h", mDOM.mFilename.c_str());
		FILE *fdom = fopen(scratch, "wb");
		if (fdom == nullptr)
		{
			printf("Failed to open output file: %s for write access.\r\n", scratch);
			return;
		}
		printf("Saving C++ DOM to: %s\r\n", scratch);


		CodePrinter impl(fimpl);
		CodePrinter dom(fdom);

		mDOM.saveCPP(impl,dom);


		fclose(fimpl);
		fclose(fdom);
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
		else if (_stricmp(argv[0], "ExportXML") == 0)
		{
			if (argc >= 2)
			{
				mDOM.mExportXML = std::string(argv[1]);
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
						{
							for (auto &j : mDOM.mObjects)
							{
								if (j.mName == mCurrentObject.mInheritsFrom)
								{
									j.mChildren.push_back(mCurrentObject.mName);
								}
							}
						}
						if (argc >= 5)
						{
							mCurrentObject.mEngineSpecific = std::string(argv[4]);
							if (argc >= 6)
							{
								if ( _stricmp(argv[5],"CLONE") == 0 )
								{
									mCurrentObject.mClone = true;
								}
								else if (_stricmp(argv[5], "ASSIGNMENT") == 0)
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
					di.mMember = std::string(dataItemName);
					di.mIsArray = array ? true : false;
					di.mIsPointer = pointer ? true : false;
					if (argc >= 3)
					{
						di.mType = std::string(argv[2]);
						di.mIsString = strcmp(di.mType.c_str(), "string") == 0;
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

	// Save the DOM as a protobuf file
	virtual void savePROTOBUF(void) final
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
		STRING_HELPER::stringFormat(scratch, 512, "%s.proto", mDOM.mFilename.c_str());
		FILE *fph = fopen(scratch, "wb");
		if (fph == nullptr)
		{
			printf("Failed to open output file: %s for write access.\r\n", scratch);
			return;
		}
		printf("Saving protobufs DOM to: %s\r\n", scratch);

		CodePrinter cp(fph);

		mDOM.savePROTO(cp);


		fclose(fph);

	}

	bool	mHaveObject{ false };
	Object	mCurrentObject;
	DOM		mDOM;
};

CreateDOM *CreateDOM::create(void)
{
//	testDeepCopy();
	CreateDOMDef *in = new CreateDOMDef;
	return static_cast<CreateDOM *>(in);
}



}
#endif