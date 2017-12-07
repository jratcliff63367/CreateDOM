#ifndef PHYSICS_DOM_EXPORT_PROTO_H
#define PHYSICS_DOM_EXPORT_PROTO_H

namespace PHYSICS_DOM
{

class PhysicsDOM;

class PhysicsDOMExportProto
{
public:
	static PhysicsDOMExportProto *create(void);

	virtual const char *exportProto(const PhysicsDOM &dom,size_t &len) = 0;
	virtual const char *exportProtoJSON(const PhysicsDOM &dom, size_t &len) = 0;

	virtual void release(void) = 0;
};

}

#endif
