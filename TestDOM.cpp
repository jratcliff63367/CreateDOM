#include "TestDOM.h"
#include "PhysicsDOM.h"


namespace TEST_DOM
{

class TestDOMImpl : public TestDOM
{
public:
	TestDOMImpl(void)
	{

		PHYSICS_DOM::PhysicsDOM dom;
		PHYSICS_DOM::Collection *c = new PHYSICS_DOM::Collection;
		c->id = "0";
		PHYSICS_DOM::PhysicsMaterial *pm = new PHYSICS_DOM::PhysicsMaterial;
		pm->id = "1";
		c->nodes.push_back(pm);
		PHYSICS_DOM::BoxGeometry *box = new PHYSICS_DOM::BoxGeometry;
		PHYSICS_DOM::GeometryInstance *box_instance = new PHYSICS_DOM::GeometryInstance;
		box_instance->geometry = box;
		box_instance->materials.push_back("1");
		PHYSICS_DOM::RigidDynamic *rd = new PHYSICS_DOM::RigidDynamic;
		rd->id = "2";
		rd->geometryInstances.push_back(box_instance);
		c->nodes.push_back(rd);
		PHYSICS_DOM::Scene *s = new PHYSICS_DOM::Scene;
		dom.collections.push_back(c);
		dom.scenes.push_back(s);

		PHYSICS_DOM::InstanceCollection *ic = new PHYSICS_DOM::InstanceCollection;
		ic->id = "3";			// Node '3'
		ic->collection = "0";	// Instance node '0' 
		s->nodes.push_back(ic);
	}

	virtual ~TestDOMImpl(void)
	{
	}

	virtual void release(void) final
	{
		delete this;
	}

};


TestDOM *TestDOM::create(void)
{
	TestDOMImpl *ret = new TestDOMImpl;
	return static_cast< TestDOM *>(ret);
}

}
