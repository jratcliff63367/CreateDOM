#include "TestDOM.h"

#define ENABLED 1

#if ENABLED
#include "PhysicsDOMImpl.h"
#endif

namespace TEST_DOM
{

class TestDOMImpl : public TestDOM
{
public:
	TestDOMImpl(void)
	{
#if ENABLED
		PHYSICS_DOM::PhysicsDOMImpl dom;
		PHYSICS_DOM::CollectionImpl *c = new PHYSICS_DOM::CollectionImpl;
		c->mId = "0";
		PHYSICS_DOM::PhysicsMaterialImpl *pm = new PHYSICS_DOM::PhysicsMaterialImpl;
		pm->mId = "1";
		c->mNodes.push_back(pm);
		PHYSICS_DOM::BoxGeometryImpl *box = new PHYSICS_DOM::BoxGeometryImpl;
		PHYSICS_DOM::GeometryInstanceImpl *box_instance = new PHYSICS_DOM::GeometryInstanceImpl;
		box_instance->mGeometry = box;
		box_instance->mMaterials.push_back("1");
		PHYSICS_DOM::RigidDynamicImpl *rd = new PHYSICS_DOM::RigidDynamicImpl;
		rd->mId = "2";
		rd->mGeometryInstances.push_back(box_instance);
		c->mNodes.push_back(rd);
		PHYSICS_DOM::SceneImpl *s = new PHYSICS_DOM::SceneImpl;
		dom.mCollections.push_back(c);
		dom.mScenes.push_back(s);

		PHYSICS_DOM::InstanceCollectionImpl *ic = new PHYSICS_DOM::InstanceCollectionImpl;
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
