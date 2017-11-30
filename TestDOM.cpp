#include "TestDOM.h"

#define ENABLED 1

#if ENABLED
#include "PhysicsDOMDef.h"
#endif

namespace TEST_DOM
{

class TestDOMDef : public TestDOM
{
public:
	TestDOMDef(void)
	{
#if ENABLED
		PHYSICS_DOM::PhysicsDOMDef dom;
		PHYSICS_DOM::CollectionDef *c = new PHYSICS_DOM::CollectionDef;
		c->mId = "0";
		PHYSICS_DOM::PhysicsMaterialDef *pm = new PHYSICS_DOM::PhysicsMaterialDef;
		pm->mId = "1";
		c->mNodes.push_back(pm);
		PHYSICS_DOM::BoxGeometryDef *box = new PHYSICS_DOM::BoxGeometryDef;
		PHYSICS_DOM::GeometryInstanceDef *box_instance = new PHYSICS_DOM::GeometryInstanceDef;
		box_instance->mGeometry = box;
		box_instance->mMaterials.push_back("1");
		PHYSICS_DOM::RigidDynamicDef *rd = new PHYSICS_DOM::RigidDynamicDef;
		rd->mId = "2";
		rd->mGeometryInstances.push_back(box_instance);
		c->mNodes.push_back(rd);
		PHYSICS_DOM::SceneDef *s = new PHYSICS_DOM::SceneDef;
		dom.mCollections.push_back(c);
		dom.mScenes.push_back(s);

		PHYSICS_DOM::InstanceCollectionDef *ic = new PHYSICS_DOM::InstanceCollectionDef;
		ic->mId = "3";			// Node '3'
		ic->mCollection = "0";	// Instance node '0' 
		s->mNodes.push_back(ic);

		dom.initDOM();
		PHYSICS_DOM::PhysicsDOM *pdom = dom.getPhysicsDOM();
		if (pdom)
		{
			printf("Created DOM.\r\n");
		}
#endif
	}

	virtual ~TestDOMDef(void)
	{
	}

	virtual void release(void) final
	{
		delete this;
	}

};


TestDOM *TestDOM::create(void)
{
	TestDOMDef *ret = new TestDOMDef;
	return static_cast< TestDOM *>(ret);
}

}
