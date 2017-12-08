#include "PhysicsDOMExportProto.h"
#pragma warning(disable:4800 4100)
#include "PhysicsDOM.h"
#include "PhysicsDOM.pb.h"
#include "TextProtoUtil.h"

namespace PHYSICS_DOM
{

class PhysicsDOM;

class PhysicsDOMExportProtoImpl : public PhysicsDOMExportProto
{
public:
	PhysicsDOMExportProtoImpl(void)
	{

	}
	virtual ~PhysicsDOMExportProtoImpl(void)
	{

	}

	void copyVec3(CreateDOM::Vec3 *dest, const PHYSICS_DOM::Vec3 &source)
	{
		dest->set_x(source.x);
		dest->set_y(source.y);
		dest->set_z(source.z);
	}

	void copyQuat(CreateDOM::Quat *dest, const PHYSICS_DOM::Quat &source)
	{
		dest->set_x(source.x);
		dest->set_y(source.y);
		dest->set_z(source.z);
		dest->set_w(source.w);
	}


	void copyPose(CreateDOM::Pose *dest, const PHYSICS_DOM::Pose &source)
	{
		copyVec3(dest->mutable_p(), source.p);
		copyQuat(dest->mutable_q(), source.q);
	}

	void copyVisualBinding(CreateDOM::VisualBinding *dest,const PHYSICS_DOM::VisualBinding *source)
	{
		dest->set_visualname(std::string(source->visualName));
		copyPose(dest->mutable_localpose(), source->localPose);
		copyVec3(dest->mutable_localscale(), source->localScale);
	}

	void copyKeyValuePair(CreateDOM::KeyValuePair *dest, const PHYSICS_DOM::KeyValuePair &source)
	{
		dest->set_key(std::string(source.key));
		dest->set_value(std::string(source.value));
	}

	void copyAdditionalProperties(CreateDOM::AdditionalProperties *dest,const PHYSICS_DOM::AdditionalProperties &source)
	{
		dest->set_category(std::string(source.category));
		for (uint32_t i = 0; i < source.keyValuePairsCount; i++)
		{
			copyKeyValuePair(dest->add_keyvaluepairs(), source.keyValuePairs[i]);
		}
	}

	void debugMe(void)
	{

	}

	void copyMaterial(CreateDOM::PhysicsMaterial &destMaterial, const PHYSICS_DOM::PhysicsMaterial &sourceMaterial)
	{
		destMaterial.set_disablefriction(sourceMaterial.disableFriction);
		destMaterial.set_disablestrongfriction(sourceMaterial.disableStrongFriction);
		destMaterial.set_dynamicfriction(sourceMaterial.dynamicFriction);
		destMaterial.set_staticfriction(sourceMaterial.staticFriction);
		destMaterial.set_restitution(sourceMaterial.restitution);
	}
	
	void copyNode(const PHYSICS_DOM::Node *n, CreateDOM::Node *cn)
	{
		cn->set_id(std::string(n->id));
		cn->set_name(std::string(n->name));
		cn->set_type(CreateDOM::NodeType(n->type));
		copyVisualBinding(cn->mutable_visual(), &n->visual);
		for (uint32_t i = 0; i < n->additionalPropertiesCount; i++)
		{
			const PHYSICS_DOM::AdditionalProperties &ap = n->additionalProperties[i];
			copyAdditionalProperties(cn->add_additionalproperties(),ap);
		}
		switch (n->type)
		{
			case NT_NODE:
				{
				debugMe();
				}
				break;
			case NT_PHYSICS_MATERIAL:
				{
					CreateDOM::PhysicsMaterial *destMaterial = cn->mutable_physicsmaterial();
					const PHYSICS_DOM::PhysicsMaterial *sourceMaterial = static_cast<const PHYSICS_DOM::PhysicsMaterial *>(n);
					copyMaterial(*destMaterial, *sourceMaterial);
				}
				break;
			case NT_GEOMETRY_INSTANCE:
				{
				debugMe();
				}
				break;
			case NT_TRIANGLEMESH:
				{
				debugMe();
				}
				break;
			case NT_CONVEXHULL:
				{
				debugMe();
				}
				break;
			case NT_HEIGHTFIELD:
				{
				debugMe();
				}
				break;
			case NT_RIGID_BODY:
				{
				debugMe();
				}
				break;
			case NT_RIGID_STATIC:
				{
				debugMe();
				}
				break;
			case NT_RIGID_DYNAMIC:
				{
				debugMe();
				}
				break;
			case NT_BODY_PAIR_FILTERS:
				{
				debugMe();
				}
				break;
			case NT_JOINT:
				{
				debugMe();
				}
				break;
			case NT_FIXED_JOINT:
				{
				debugMe();
				}
				break;
			case NT_SPHERICAL_JOINT:
				{
				debugMe();
				}
				break;
			case NT_HINGE_JOINT:
				{
				debugMe();
				}
				break;
			case NT_PRISMATIC_JOINT:
				{
				debugMe();
				}
				break;
			case NT_DISTANCE_JOINT:
				{
				debugMe();
				}
				break;
			case NT_BALL_AND_SOCKET_JOINT:
				{
				debugMe();
				}
				break;
			case NT_D6_JOINT:
				{
				debugMe();
				}
				break;
			case NT_INSTANCE_COLLECTION:
				{
				debugMe();
				}
				break;
			case NT_COLLECTION:
				{
					CreateDOM::Collection *destCollection = cn->mutable_collection();
					const PHYSICS_DOM::Collection *sourceCollection = static_cast<const PHYSICS_DOM::Collection *>(n);
					for (uint32_t j = 0; j < sourceCollection->nodesCount; j++)
					{
						const PHYSICS_DOM::Node *fromNode = sourceCollection->nodes[j];
						CreateDOM::Node *toNode = destCollection->add_nodes();
						copyNode(fromNode, toNode);
					}
				}
				break;
			case NT_SCENE:
				{
				CreateDOM::Scene *destScene = cn->mutable_scene();
				const PHYSICS_DOM::Scene *sourceScene = static_cast<const PHYSICS_DOM::Scene *>(n);
				copyVec3(destScene->mutable_gravity(), sourceScene->gravity);
				for (uint32_t j = 0; j < sourceScene->nodesCount; j++)
				{
					const PHYSICS_DOM::Node *fromNode = sourceScene->nodes[j];
					CreateDOM::Node *toNode = destScene->add_nodes();
					copyNode(fromNode, toNode);
				}
				}
				break;
		}
	}

	void getProtoPhysicsDOM(const PHYSICS_DOM::PhysicsDOM &dom,CreateDOM::PhysicsDOM &cdom)
	{
		for (uint32_t i = 0; i < dom.collectionsCount; i++)
		{
			PHYSICS_DOM::Collection *c = dom.collections[i];
			CreateDOM::Node* nc = cdom.add_collections();
			copyNode(c, nc);
		}

		for (uint32_t i = 0; i < dom.scenesCount; i++)
		{
			PHYSICS_DOM::Scene *s = dom.scenes[i];
			CreateDOM::Node* ns = cdom.add_scenes();
			copyNode(s, ns);
		}

	}

	virtual const char *exportProto(const PHYSICS_DOM::PhysicsDOM &dom, size_t &len) final
	{
		const char *ret = nullptr;

		CreateDOM::PhysicsDOM cdom;
		getProtoPhysicsDOM(dom, cdom);
		mExport.clear();
		google::protobuf::TextFormat::PrintToString(cdom, &mExport);

		len = mExport.size();
		ret = mExport.c_str();

		return ret;
	}

	virtual const char *exportProtoJSON(const PHYSICS_DOM::PhysicsDOM &dom, size_t &len) final
	{
		const char *ret = nullptr;

		mExport.clear();
		CreateDOM::PhysicsDOM cdom;
		getProtoPhysicsDOM(dom, cdom);

		google::protobuf::util::JsonPrintOptions json_options;
		json_options.always_print_primitive_fields = true;
		json_options.add_whitespace = true;
		google::protobuf::util::MessageToJsonString(cdom, &mExport, json_options).ok();
		len = mExport.size();
		ret = mExport.c_str();

		return ret;
	}


	virtual void release(void) final
	{
		delete this;
	}

	std::string	mExport;
};

PhysicsDOMExportProto *PhysicsDOMExportProto::create(void)
{
	auto ret = new PhysicsDOMExportProtoImpl;
	return static_cast<PhysicsDOMExportProto *>(ret);
}


}

