#ifndef TEST_DOM_H
#define TEST_DOM_H


namespace TEST_DOM
{

class TestDOM
{
public:
	static TestDOM *create(void);
	virtual void release(void) = 0;
protected:
	virtual	~TestDOM(void)
	{
	}
};

}

#endif
