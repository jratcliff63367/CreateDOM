#include "TestProto.h"

#pragma warning(disable:4800)

#include "PhysicsDOM.pb.h"


namespace TEST_PROTO
{

class TestProtoImpl : public TestProto
{
public:
	TestProtoImpl(void)
	{
		{
			CreateDOM::PhysicsDOM dom;
			dom.add_collections();
			dom.add_scenes();


			std::string foo = dom.SerializeAsString();
			FILE *fph = fopen("foo.txt", "wb");
			if (fph)
			{
				fwrite(foo.c_str(), foo.size(), 1, fph);
				fclose(fph);
			}
		}

	}

	virtual ~TestProtoImpl(void)
	{

	}

	virtual void release(void) final
	{
		delete this;
	}
};

TestProto *TestProto::create(void)
{
	TestProtoImpl *ret = new TestProtoImpl;
	return static_cast<TestProto *>(ret);
}

}
