#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <string.h>

#include "CreateDOM.h"
#include "TestProto.h"
#include "TestDOM.h"

int main(int argc, const char **argv)
{
	{
		TEST_DOM::TestDOM *t = TEST_DOM::TestDOM::create();
		t->release();
	}
	{
		TEST_PROTO::TestProto *t = TEST_PROTO::TestProto::create();
		t->release();
	}
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

