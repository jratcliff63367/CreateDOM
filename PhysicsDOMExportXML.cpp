#include "PhysicsDOMExportXML.h"
#include "PhysicsDOM.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>

#pragma warning(disable:4100)

namespace PHYSICS_DOM
{

	class PhysicsDOMExportXMLImpl : public PhysicsDOMExportXML
	{
	public:

		void beginElement(const char *element, std::ostringstream &stream, uint32_t indent,bool lineFeed)
		{
			exportIndent(stream, indent);
			stream << "<";
			stream << std::string(element);
			stream << ">";
			if (lineFeed)
			{
				stream << "\r\n";
			}
		}

		void endElement(const char *element, std::ostringstream &stream, uint32_t indent)
		{
			exportIndent(stream, indent);
			stream << "</";
			stream << std::string(element);
			stream << ">";
			stream << "\r\n";
		}

		void exportIndent(std::ostringstream &stream, uint32_t indent)
		{
			for (uint32_t i = 0; i < indent; i++)
			{
				stream << "  ";
			}
		}

		void exportVec3(const char *memberName, const Vec3 &e, std::ostringstream &stream,uint32_t indent)
		{
			beginElement(memberName, stream, indent,false);

			stream << e.x;
			stream << " ";
			stream << e.y;
			stream << " ";
			stream << e.z;

			endElement(memberName, stream, 0);
		}

		void exportScene(const Scene *e, std::ostringstream &stream,uint32_t indent)
		{
			beginElement("Scene", stream, indent, true);
			exportVec3("gravity", e->gravity,stream,indent+1);
			for (uint32_t i = 0; i < e->nodesCount; i++)
			{
			}
			endElement("Scene", stream, indent);
		}

		void exportPhysicsDOM(const PhysicsDOM *e, std::ostringstream &stream,uint32_t indent)
		{
			beginElement("PhysicsDOM", stream, indent, true);

			for (uint32_t i = 0; i < e->scenesCount; i++)
			{
				exportScene(e->scenes[i], stream,indent+1);
			}

			endElement("PhysicsDOM", stream, indent);
		}

		virtual const char *exportXML(PhysicsDOM &dom,size_t &slen)
		{
			const char *ret = nullptr;
			mXML.clear();
			
			std::ostringstream stream;

			exportPhysicsDOM(&dom, stream,0);

			mXML = stream.str();

			slen = mXML.size();
			ret = mXML.c_str();
			return ret;
		}

		virtual void release(void)
		{
			delete this;
		}

		std::string	mXML;
	};

PhysicsDOMExportXML *PhysicsDOMExportXML::create(void)
{
	PhysicsDOMExportXMLImpl *p = new PhysicsDOMExportXMLImpl;
	return static_cast<PhysicsDOMExportXML *>(p);
}


}
