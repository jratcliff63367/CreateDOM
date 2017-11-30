#ifndef PHYSICS_DOM_EXPORT_XML_H
#define PHYSICS_DOM_EXPORT_XML_H

namespace PHYSICS_DOM
{

class PhysicsDOM;

class PhysicsDOMExportXML
{
public:
	static PhysicsDOMExportXML *create(void);
	virtual const char *exportXML(PhysicsDOM &dom,size_t &len) = 0;
	virtual void release(void) = 0;
};

}

#endif
