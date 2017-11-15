#ifndef PHYSICSDOM_IMPL_H
#define PHYSICSDOM_IMPL_H

// CreateDOM: Schema Generation tool written by John W. Ratcliff, 2017
// Warning:This source file was auto-generated by the CreateDOM tool. Do not try to edit this source file manually!
// The Google DOCs Schema Spreadsheet for this source came from: https://docs.google.com/spreadsheets/d/118I5kdu2XT-6wfCG044937xfEKDyX2oNg04G8Wqi6o0/edit?usp=sharing

#include "PhysicsDOM.h"
#include <string>
#include <vector>
#include <stdint.h>


namespace PHYSICS_DOM
{


// Defines an optional visual mesh binding to a physics node
class VisualBindingImpl : public VisualBinding
{
public:
	std::string	mVisualName; 									// Name of associated visual mesh
	Pose 		mLocalPose;   										// Local relative pose of visual mesh to corresponding physics node
	Vec3 		mLocalScale;											// Local relative scale of visual mesh to corresponding physics node
};


// Describes a key-value pair for custom properties on a node
class KeyValuePairImpl : public KeyValuePair
{
public:
	std::string	mKey;  											// They 'key' identifier; what this property is
	std::string	mValue;											// The value of this property; up to each the user to figure out how to interpret each property relative to the keyword
};

typedef std::vector< KeyValuePair > KeyValuePairVector; // Forward declare the 'KeyValuePair' vector

// A collection of key/value pair properties relative to a particular category
class AdditionalPropertiesImpl : public AdditionalProperties
{
public:
	std::string	mCategory;   									// The category this set of key/value pairs is associated with (example 'physx', 'mujoco', etc.
	KeyValuePairVector mKeyValuePairs;   						// The array of key/value pairs associated with this category
};

typedef std::vector< AdditionalProperties > AdditionalPropertiesVector; // Forward declare the 'AdditionalProperties' vector

// Base class that specifies a unique ID and an optional description name field for an object
class NodeImpl : public Node
{
public:
	// Declare the clone method
	virtual Node *clone() const = 0;

	std::string	mId; 											// Unique Id for this object
	std::string	mName;   										// Optional name for this object
	NodeType 	mType{ NT_NODE };									// The type of node
	VisualBinding  mVisual;										// Optional visual bindings for this node; for exaple some physics components have a corresponding named graphics component
	AdditionalPropertiesVector mAdditionalProperties;  			// An optional set of properties for this node; a set of key-value pairs for each application/engine specific category
};


// Defines the physical material properties of a surface
class PhysicsMaterialImpl : public Node, public PhysicsMaterial
{
public:
	// Declare the constructor.
	PhysicsMaterialImpl()
	{
		Node::mType = NT_PHYSICS_MATERIAL;
	};


	// Declare the virtual destructor.
	virtual ~PhysicsMaterialImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	PhysicsMaterialImpl(const PhysicsMaterialImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new PhysicsMaterialImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	PhysicsMaterialImpl& operator=(const PhysicsMaterialImpl& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			mDisableFriction = other.mDisableFriction;
			mDisableStrongFriction = other.mDisableStrongFriction;
			mDynamicFriction = other.mDynamicFriction;
			mStaticFriction = other.mStaticFriction;
			mRestitution = other.mRestitution;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	PhysicsMaterialImpl(PhysicsMaterialImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	PhysicsMaterialImpl& operator=(PhysicsMaterialImpl&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			mDisableFriction = other.mDisableFriction;
			mDisableStrongFriction = other.mDisableStrongFriction;
			mDynamicFriction = other.mDynamicFriction;
			mStaticFriction = other.mStaticFriction;
			mRestitution = other.mRestitution;
		}
		return *this;
	}

	bool 		mDisableFriction{ false };  							// If true, then friction is disabled for the material
	bool 		mDisableStrongFriction{ false };						// If true then strong friction is disabled for the material
	float  		mDynamicFriction{ 0.5f };   						// The coefficient of dynamic friction.
	float  		mStaticFriction{ 0.5f };  							// The coefficient of static friction
	float  		mRestitution{ 0.5f };   							// The coefficient of resitution.
};

typedef std::vector< Vec3 > Vec3Vector; // Forward declare the 'Vec3' vector

// Describes the data for a convex hull
class ConvexHullImpl : public Node, public ConvexHull
{
public:
	// Declare the constructor.
	ConvexHullImpl()
	{
		Node::mType = NT_CONVEXHULL;
	};


	// Declare the virtual destructor.
	virtual ~ConvexHullImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	ConvexHullImpl(const ConvexHullImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new ConvexHullImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	ConvexHullImpl& operator=(const ConvexHullImpl& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			mPoints = other.mPoints;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	ConvexHullImpl(ConvexHullImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	ConvexHullImpl& operator=(ConvexHullImpl&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			mPoints = other.mPoints;
		}
		return *this;
	}

	Vec3Vector   mPoints;  										// Array of data points describing the convex hull
};

typedef std::vector< uint32_t > U32Vector; // Forward declare the 'U32' vector

// Describes the data for a triangle mesh
class TriangleMeshImpl : public Node, public TriangleMesh
{
public:
	// Declare the constructor.
	TriangleMeshImpl()
	{
		Node::mType = NT_TRIANGLEMESH;
	};


	// Declare the virtual destructor.
	virtual ~TriangleMeshImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	TriangleMeshImpl(const TriangleMeshImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new TriangleMeshImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	TriangleMeshImpl& operator=(const TriangleMeshImpl& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			mPoints = other.mPoints;
			mTriangles = other.mTriangles;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	TriangleMeshImpl(TriangleMeshImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	TriangleMeshImpl& operator=(TriangleMeshImpl&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			mPoints = other.mPoints;
			mTriangles = other.mTriangles;
		}
		return *this;
	}

	Vec3Vector   mPoints;  										// Array of vertices for the triangle mesh
	U32Vector  	mTriangles;										// Array of triangle indices
};


// Defines a box geometry
class BoxGeometryImpl : public Geometry, public BoxGeometry
{
public:
	// Declare the constructor.
	BoxGeometryImpl()
	{
		Geometry::mType = GT_BOX_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~BoxGeometryImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	BoxGeometryImpl(const BoxGeometryImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new BoxGeometryImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	BoxGeometryImpl& operator=(const BoxGeometryImpl& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			mDimensions = other.mDimensions;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	BoxGeometryImpl(BoxGeometryImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	BoxGeometryImpl& operator=(BoxGeometryImpl&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			mDimensions = other.mDimensions;
		}
		return *this;
	}

	Vec3 		mDimensions{ 1,1,1 }; 								// Dimensions of the box
};


// Defines a sphere geometry
class SphereGeometryImpl : public Geometry, public SphereGeometry
{
public:
	// Declare the constructor.
	SphereGeometryImpl()
	{
		Geometry::mType = GT_SPHERE_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~SphereGeometryImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	SphereGeometryImpl(const SphereGeometryImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new SphereGeometryImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	SphereGeometryImpl& operator=(const SphereGeometryImpl& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			mRadius = other.mRadius;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	SphereGeometryImpl(SphereGeometryImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	SphereGeometryImpl& operator=(SphereGeometryImpl&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			mRadius = other.mRadius;
		}
		return *this;
	}

	float  		mRadius{ 1 };   									// The radius of the sphere
};


// Defines a capsule geometry
class CapsuleGeometryImpl : public Geometry, public CapsuleGeometry
{
public:
	// Declare the constructor.
	CapsuleGeometryImpl()
	{
		Geometry::mType = GT_CAPSULE_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~CapsuleGeometryImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	CapsuleGeometryImpl(const CapsuleGeometryImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new CapsuleGeometryImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	CapsuleGeometryImpl& operator=(const CapsuleGeometryImpl& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			mRadius = other.mRadius;
			mHeight = other.mHeight;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	CapsuleGeometryImpl(CapsuleGeometryImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	CapsuleGeometryImpl& operator=(CapsuleGeometryImpl&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			mRadius = other.mRadius;
			mHeight = other.mHeight;
		}
		return *this;
	}

	float  		mRadius{ 1 };   									// The radius of the capsule
	float  		mHeight{ 1 };   									// The height of the capsule
};


// Defines a cylinder geometry
class CylinderGeometryImpl : public Geometry, public CylinderGeometry
{
public:
	// Declare the constructor.
	CylinderGeometryImpl()
	{
		Geometry::mType = GT_CYLINDER_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~CylinderGeometryImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	CylinderGeometryImpl(const CylinderGeometryImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new CylinderGeometryImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	CylinderGeometryImpl& operator=(const CylinderGeometryImpl& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			mRadius = other.mRadius;
			mHeight = other.mHeight;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	CylinderGeometryImpl(CylinderGeometryImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	CylinderGeometryImpl& operator=(CylinderGeometryImpl&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			mRadius = other.mRadius;
			mHeight = other.mHeight;
		}
		return *this;
	}

	float  		mRadius{ 1 };   									// The radius of the cylinder
	float  		mHeight{ 1 };   									// The height of the cylinder
};


// Defines a convex mesh geometry
class ConvexHullGeometryImpl : public Geometry, public ConvexHullGeometry
{
public:
	// Declare the constructor.
	ConvexHullGeometryImpl()
	{
		Geometry::mType = GT_CONVEXHULL_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~ConvexHullGeometryImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	ConvexHullGeometryImpl(const ConvexHullGeometryImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new ConvexHullGeometryImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	ConvexHullGeometryImpl& operator=(const ConvexHullGeometryImpl& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			mScale = other.mScale;
			mConvexMesh = other.mConvexMesh;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	ConvexHullGeometryImpl(ConvexHullGeometryImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	ConvexHullGeometryImpl& operator=(ConvexHullGeometryImpl&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			mScale = other.mScale;
			mConvexMesh = other.mConvexMesh;
		}
		return *this;
	}

	MeshScale  	mScale;											// The scale to apply to this convex mesh
	std::string	mConvexMesh; 									// The name of the convex mesh asset
};


// Defines a triangle mesh geometry
class TriangleMeshGeometryImpl : public Geometry, public TriangleMeshGeometry
{
public:
	// Declare the constructor.
	TriangleMeshGeometryImpl()
	{
		Geometry::mType = GT_TRIANGLEMESH_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~TriangleMeshGeometryImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	TriangleMeshGeometryImpl(const TriangleMeshGeometryImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new TriangleMeshGeometryImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	TriangleMeshGeometryImpl& operator=(const TriangleMeshGeometryImpl& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			mScale = other.mScale;
			mTriangleMesh = other.mTriangleMesh;
			mDoubleSided = other.mDoubleSided;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	TriangleMeshGeometryImpl(TriangleMeshGeometryImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	TriangleMeshGeometryImpl& operator=(TriangleMeshGeometryImpl&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			mScale = other.mScale;
			mTriangleMesh = other.mTriangleMesh;
			mDoubleSided = other.mDoubleSided;
		}
		return *this;
	}

	MeshScale  	mScale;											// The scale of the triangle mesh
	std::string	mTriangleMesh;   								// The name of the triangle mesh asset
	bool 		mDoubleSided{ false };  								// Whether or not this triangle mesh should be treated as double sided for collision detection
};


// Defines a plane equation geometry (position and orientation of the plane come from the geometry instance)
class PlaneGeometryImpl : public Geometry, public PlaneGeometry
{
public:
	// Declare the constructor.
	PlaneGeometryImpl()
	{
		Geometry::mType = GT_PLANE_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~PlaneGeometryImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	PlaneGeometryImpl(const PlaneGeometryImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new PlaneGeometryImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	PlaneGeometryImpl& operator=(const PlaneGeometryImpl& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	PlaneGeometryImpl(PlaneGeometryImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	PlaneGeometryImpl& operator=(PlaneGeometryImpl&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
		}
		return *this;
	}

};


// Defines a single instance of a geometry
class GeometryInstanceImpl : public GeometryInstance
{
public:

	// Declare the constructor.
	GeometryInstanceImpl() { }


	// Declare the virtual destructor; cleanup any pointers or arrays of pointers
	virtual ~GeometryInstanceImpl()
	{
		delete mGeometry; // Delete this object
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	GeometryInstanceImpl(const GeometryInstanceImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual GeometryInstanceImpl* clone() const
	{
		return new GeometryInstanceImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	GeometryInstanceImpl& operator=(const GeometryInstanceImpl& other)
	{
		if (this != &other )
		{
			delete mGeometry; // delete any previous pointer.
			mGeometry = nullptr; // set the pointer to null.
			if ( other.mGeometry )
			{
				mGeometry = static_cast<Geometry *>(other.mGeometry->clone()); // perform the deep copy and assignment here
			}
			mMaterial = other.mMaterial;
			mLocalPose = other.mLocalPose;
			mCollisionFilterSettings = other.mCollisionFilterSettings;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	GeometryInstanceImpl(GeometryInstanceImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	GeometryInstanceImpl& operator=(GeometryInstanceImpl&& other)
	{
		if (this != &other )
		{
			mGeometry = other.mGeometry;
			other.mGeometry = nullptr; // Set 'other' pointer to null since we have moved it
			mMaterial = other.mMaterial;
			mLocalPose = other.mLocalPose;
			mCollisionFilterSettings = other.mCollisionFilterSettings;
		}
		return *this;
	}

	Geometry 	*mGeometry{ nullptr }; 							// The geometry associated with this instance
	std::string	mMaterial;   									// Id of physical material associated with this propery
	Pose 		mLocalPose;   										// The local pose for this geometry instance
	std::string	mCollisionFilterSettings;  						// Describes collision filtering settings; what other types of objects this object will collide with
};

typedef std::vector< GeometryInstance *> GeometryInstanceVector; // Forward declare the 'GeometryInstance' vector

// Defines the common properties for a rigid body
class RigidBodyImpl : public Node, public RigidBody
{
public:
	// Declare the constructor.
	RigidBodyImpl()
	{
		Node::mType = NT_RIGID_BODY;
	};


	// Declare the virtual destructor; cleanup any pointers or arrays of pointers
	virtual ~RigidBodyImpl()
	{
		for (auto &i:mGeometryInstances) delete i; // Delete all of the object pointers in this array
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	RigidBodyImpl(const RigidBodyImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new RigidBodyImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	RigidBodyImpl& operator=(const RigidBodyImpl& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			for (auto &i:mGeometryInstances) delete i; // Delete all of the object pointers in this array
			mGeometryInstances.clear(); // Clear the current array
			for (auto &i:other.mGeometryInstances) mGeometryInstances.push_back( static_cast< GeometryInstance *>(i->clone())); // Deep copy object pointers into the array
			mGlobalPose = other.mGlobalPose;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	RigidBodyImpl(RigidBodyImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	RigidBodyImpl& operator=(RigidBodyImpl&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			mGeometryInstances = other.mGeometryInstances;
			other.mGeometryInstances.clear(); // Clear the 'other' array now that we have moved it
			mGlobalPose = other.mGlobalPose;
		}
		return *this;
	}

	GeometryInstanceVector mGeometryInstances;   				// The set of geometries to instance with this actor
	Pose 		mGlobalPose;											// The global pose for this actor
};


// Defines a static rigid body
class RigidStaticImpl : public RigidBody, public RigidStatic
{
public:
	// Declare the constructor.
	RigidStaticImpl()
	{
		Node::mType = NT_RIGID_STATIC;
	};


	// Declare the virtual destructor.
	virtual ~RigidStaticImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	RigidStaticImpl(const RigidStaticImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual RigidBody* clone() const override
	{
		return new RigidStaticImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	RigidStaticImpl& operator=(const RigidStaticImpl& other)
	{
		if (this != &other )
		{
			RigidBody::operator=(other);
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	RigidStaticImpl(RigidStaticImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	RigidStaticImpl& operator=(RigidStaticImpl&& other)
	{
		if (this != &other )
		{
			RigidBody::operator=(std::move(other));
		}
		return *this;
	}

};


// Defines a dynamic rigid body
class RigidDynamicImpl : public RigidBody, public RigidDynamic
{
public:
	// Declare the constructor.
	RigidDynamicImpl()
	{
		Node::mType = NT_RIGID_DYNAMIC;
	};


	// Declare the virtual destructor.
	virtual ~RigidDynamicImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	RigidDynamicImpl(const RigidDynamicImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual RigidBody* clone() const override
	{
		return new RigidDynamicImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	RigidDynamicImpl& operator=(const RigidDynamicImpl& other)
	{
		if (this != &other )
		{
			RigidBody::operator=(other);
			mDisableGravity = other.mDisableGravity;
			mCenterOfMassLocalPose = other.mCenterOfMassLocalPose;
			mMass = other.mMass;
			mMassSpaceInertiaTensor = other.mMassSpaceInertiaTensor;
			mLinearVelocity = other.mLinearVelocity;
			mAngularVelocity = other.mAngularVelocity;
			mLinearDamping = other.mLinearDamping;
			mAngularDamping = other.mAngularDamping;
			mMaxAngularVelocity = other.mMaxAngularVelocity;
			mKinematic = other.mKinematic;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	RigidDynamicImpl(RigidDynamicImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	RigidDynamicImpl& operator=(RigidDynamicImpl&& other)
	{
		if (this != &other )
		{
			RigidBody::operator=(std::move(other));
			mDisableGravity = other.mDisableGravity;
			mCenterOfMassLocalPose = other.mCenterOfMassLocalPose;
			mMass = other.mMass;
			mMassSpaceInertiaTensor = other.mMassSpaceInertiaTensor;
			mLinearVelocity = other.mLinearVelocity;
			mAngularVelocity = other.mAngularVelocity;
			mLinearDamping = other.mLinearDamping;
			mAngularDamping = other.mAngularDamping;
			mMaxAngularVelocity = other.mMaxAngularVelocity;
			mKinematic = other.mKinematic;
		}
		return *this;
	}

	bool 		mDisableGravity;										// Disables scene gravity for this actor
	Pose 		mCenterOfMassLocalPose;   							// Center of mass and local pose
	float  		mMass;												// Sets the mass of a dynamic actor.
	Vec3 		mMassSpaceInertiaTensor;								// Sets the inertia tensor, using a parameter specified in mass space coordinates.
	Vec3 		mLinearVelocity;										// Sets the linear velocity of the actor.
	Vec3 		mAngularVelocity; 									// Sets the angular velocity of the actor.
	float  		mLinearDamping{ 0 };  								// Sets the linear damping coefficient.
	float  		mAngularDamping{ 0.05f };   						// Sets the angular damping coefficient.
	float  		mMaxAngularVelocity{ 7 };   						// set the maximum angular velocity permitted for this actor.
	bool 		mKinematic{ false };									// If true this is a dynamic object; but currently kinematically controlled
};


// Defines the common properties for a joint
class JointImpl : public Node, public Joint
{
public:
	// Declare the constructor.
	JointImpl()
	{
		Node::mType = NT_JOINT;
	};


	// Declare the virtual destructor.
	virtual ~JointImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	JointImpl(const JointImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new JointImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	JointImpl& operator=(const JointImpl& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			mBody0 = other.mBody0;
			mBody1 = other.mBody1;
			mLocalpose0 = other.mLocalpose0;
			mLocalpose1 = other.mLocalpose1;
			mCollisionEnabled = other.mCollisionEnabled;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	JointImpl(JointImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	JointImpl& operator=(JointImpl&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			mBody0 = other.mBody0;
			mBody1 = other.mBody1;
			mLocalpose0 = other.mLocalpose0;
			mLocalpose1 = other.mLocalpose1;
			mCollisionEnabled = other.mCollisionEnabled;
		}
		return *this;
	}

	std::string	mBody0;											// Id of first rigid body joint is constrained to; if empty string; then constaint to the world
	std::string	mBody1;											// Id of the second rigid body the joint is constrainted to
	Pose 		mLocalpose0;											// The parent relative pose; relative to body0
	Pose 		mLocalpose1;											// The parent relative pose; relative to body1
	bool 		mCollisionEnabled{ false };   						// 
};


// Defines two bodies, by id, that should not collide with each other
class BodyPairFilterImpl : public BodyPairFilter
{
public:
	std::string	mBodyA;											// Id of first body
	std::string	mBodyB ; 										// Id of second body
};

typedef std::vector< BodyPairFilter > BodyPairFilterVector; // Forward declare the 'BodyPairFilter' vector

// A collection of body pair filters
class BodyPairFiltersImpl : public Node, public BodyPairFilters
{
public:
	// Declare the constructor.
	BodyPairFiltersImpl()
	{
		Node::mType = NT_BODY_PAIR_FILTERS;
	};


	// Declare the virtual destructor.
	virtual ~BodyPairFiltersImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	BodyPairFiltersImpl(const BodyPairFiltersImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new BodyPairFiltersImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	BodyPairFiltersImpl& operator=(const BodyPairFiltersImpl& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			mBodyPairs = other.mBodyPairs;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	BodyPairFiltersImpl(BodyPairFiltersImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	BodyPairFiltersImpl& operator=(BodyPairFiltersImpl&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			mBodyPairs = other.mBodyPairs;
		}
		return *this;
	}

	BodyPairFilterVector mBodyPairs; 							// Array of body pair filters
};


class InstanceCollectionImpl : public Node, public InstanceCollection
{
public:
	// Declare the constructor.
	InstanceCollectionImpl()
	{
		Node::mType = NT_INSTANCE_COLLECTION;
	};


	// Declare the virtual destructor.
	virtual ~InstanceCollectionImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	InstanceCollectionImpl(const InstanceCollectionImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new InstanceCollectionImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	InstanceCollectionImpl& operator=(const InstanceCollectionImpl& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			mCollection = other.mCollection;
			mPose = other.mPose;
			mScale = other.mScale;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	InstanceCollectionImpl(InstanceCollectionImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	InstanceCollectionImpl& operator=(InstanceCollectionImpl&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			mCollection = other.mCollection;
			mPose = other.mPose;
			mScale = other.mScale;
		}
		return *this;
	}

	std::string	mCollection; 									// Name of collection to instance
	Pose 		mPose;  												// Pose to instance collection at
	Vec3 		mScale;   											// Scale of instance
};

typedef std::vector< Node *> NodeVector; // Forward declare the 'Node' vector

// A collection of nodes
class CollectionImpl : public Node, public Collection
{
public:
	// Declare the constructor.
	CollectionImpl()
	{
		Node::mType = NT_COLLECTION;
	};


	// Declare the virtual destructor; cleanup any pointers or arrays of pointers
	virtual ~CollectionImpl()
	{
		for (auto &i:mNodes) delete i; // Delete all of the object pointers in this array
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	CollectionImpl(const CollectionImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new CollectionImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	CollectionImpl& operator=(const CollectionImpl& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			for (auto &i:mNodes) delete i; // Delete all of the object pointers in this array
			mNodes.clear(); // Clear the current array
			for (auto &i:other.mNodes) mNodes.push_back( static_cast< Node *>(i->clone())); // Deep copy object pointers into the array
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	CollectionImpl(CollectionImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	CollectionImpl& operator=(CollectionImpl&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			mNodes = other.mNodes;
			other.mNodes.clear(); // Clear the 'other' array now that we have moved it
		}
		return *this;
	}

	NodeVector   mNodes; 										// Array of nodes in this collection
};


// A special type of 'collection' which is instantiated on startup
class SceneImpl : public Collection, public Scene
{
public:
	// Declare the constructor.
	SceneImpl()
	{
		Node::mType = NT_SCENE;
	};


	// Declare the virtual destructor.
	virtual ~SceneImpl()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	SceneImpl(const SceneImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Collection* clone() const override
	{
		return new SceneImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	SceneImpl& operator=(const SceneImpl& other)
	{
		if (this != &other )
		{
			Collection::operator=(other);
			mGravity = other.mGravity;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	SceneImpl(SceneImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	SceneImpl& operator=(SceneImpl&& other)
	{
		if (this != &other )
		{
			Collection::operator=(std::move(other));
			mGravity = other.mGravity;
		}
		return *this;
	}

	Vec3 		mGravity{ 0.0f,-9.8f,0.0f };							// Gravity
};

typedef std::vector< Collection *> CollectionVector; // Forward declare the 'Collection' vector
typedef std::vector< Scene *> SceneVector; // Forward declare the 'Scene' vector

// The root node container
class PhysicsDOMImpl : public PhysicsDOM
{
public:

	// Declare the constructor.
	PhysicsDOMImpl() { }


	// Declare the virtual destructor; cleanup any pointers or arrays of pointers
	virtual ~PhysicsDOMImpl()
	{
		for (auto &i:mCollections) delete i; // Delete all of the object pointers in this array
		for (auto &i:mScenes) delete i; // Delete all of the object pointers in this array
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	PhysicsDOMImpl(const PhysicsDOMImpl &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual PhysicsDOMImpl* clone() const
	{
		return new PhysicsDOMImpl(*this);
	}

	// Declare and implement the deep copy assignment operator
	PhysicsDOMImpl& operator=(const PhysicsDOMImpl& other)
	{
		if (this != &other )
		{
			for (auto &i:mCollections) delete i; // Delete all of the object pointers in this array
			mCollections.clear(); // Clear the current array
			for (auto &i:other.mCollections) mCollections.push_back( static_cast< Collection *>(i->clone())); // Deep copy object pointers into the array
			for (auto &i:mScenes) delete i; // Delete all of the object pointers in this array
			mScenes.clear(); // Clear the current array
			for (auto &i:other.mScenes) mScenes.push_back( static_cast< Scene *>(i->clone())); // Deep copy object pointers into the array
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	PhysicsDOMImpl(PhysicsDOMImpl &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	PhysicsDOMImpl& operator=(PhysicsDOMImpl&& other)
	{
		if (this != &other )
		{
			mCollections = other.mCollections;
			other.mCollections.clear(); // Clear the 'other' array now that we have moved it
			mScenes = other.mScenes;
			other.mScenes.clear(); // Clear the 'other' array now that we have moved it
		}
		return *this;
	}

	CollectionVector mCollections;   							// The array of top level collections
	SceneVector	mScenes; 										// The array of top level scenes; a scene is instantiated into the physics simulation
};



} // End of PHYSICS_DOM namespace

#endif // End of Scene
