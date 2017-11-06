#ifndef CREATE_DOM_H
#define CREATE_DOM_H

// This code snippet will parse an XML and produce a sorted
// list of all unique element names which were found
// as well as all unique attribute names
// It will produce a snippet of source code which create an enumeration
// of all of these types and helper methods to convert from ASCII to enum value
// and from enum value back to ASCII

namespace CREATE_DOM
{

class CreateDOM
{
public:
	static CreateDOM *create(void);

	// Parses this XML and accumulates all of the unique element and attribute names
	virtual void parseCSV(const char *xmlName) = 0;

	// Save the DOM as C++ code
	virtual void saveCPP(void) = 0;

	// Save the DOM as a JSON schema
	virtual void saveJSON(void) = 0;

	virtual void release(void) = 0;
};

}

#endif
