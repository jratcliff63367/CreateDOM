#include "CreateDOM.h"
#include "StringHelper.h"
#include "InParser.h"
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <assert.h>

#pragma warning(disable:4100)

namespace CREATE_DOM
{

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

	}

	// Save the DOM as C++ code
	virtual void saveCPP(const char *fname) final
	{

	}

	// Save the DOM as a JSON schema
	virtual void saveJSON(const char *fname) final
	{

	}

	virtual void release(void) final
	{
		delete this;
	}

};

CreateDOM *CreateDOM::create(void)
{
	CreateDOMImpl *in = new CreateDOMImpl;
	return static_cast<CreateDOM *>(in);
}



}
