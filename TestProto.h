#ifndef TEST_PROTO_H
#define TEST_PROTO_H

namespace TEST_PROTO
{

class TestProto
{
public:
	static TestProto *create(void);
	virtual void release(void) = 0;
protected:
	virtual ~TestProto(void)
	{
	}
};


}

#endif
