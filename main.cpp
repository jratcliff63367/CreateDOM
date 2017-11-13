#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <string.h>

#include "CreateDOM.h"

int main(int argc, const char **argv)
{
	if (argc != 2)
	{
		printf("Usage: CreateDOM <fname.csv>\r\n");
		printf("\r\n");
	}
	else
	{
		const char *csv = argv[1];
		CREATE_DOM::CreateDOM *cdom = CREATE_DOM::CreateDOM::create();
		cdom->parseCSV(csv);
		cdom->saveCPP();
		cdom->saveJSON();
		cdom->savePROTOBUF();
		cdom->release();
	}
}

