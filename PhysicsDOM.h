#ifndef PHYSICSDOM_H
#define PHYSICSDOM_H

// CreateDOM: Schema Generation tool written by John W. Ratcliff, 2017
// Warning:This source file was auto-generated by the CreateDOM tool. Do not try to edit this source file manually!
// The Google DOCs Schema Spreadsheet for this source came from: https://docs.google.com/spreadsheets/d/118I5kdu2XT-6wfCG044937xfEKDyX2oNg04G8Wqi6o0/edit?usp=sharing

#include <stdint.h>
#include <vector>


namespace PHYSICS_DOM
{


// Defines a basic 3d vector type
class Vec3
{
public:
	// Declare the constructor.
	Vec3() { }

	// Declare the assignment constructor.
	Vec3(const float &_x,const float &_y,const float &_z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	float  		x{ 0 }; 											// x axis component of the vector
	float  		y{ 0 }; 											// y axis component of the vector
	float  		z{ 0 }; 											// z axis component of the vector
};


// Defines a basic quaternion data type
class Quat
{
public:
	// Declare the constructor.
	Quat() { }

	// Declare the assignment constructor.
	Quat(const float &_x,const float &_y,const float &_z,const float &_w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	float  		x{ 0 }; 											// 
	float  		y{ 0 }; 											// 
	float  		z{ 0 }; 											// 
	float  		w{ 1 }; 											// 
};


// Defines a basic plane equation
class Plane
{
public:
	// Declare the constructor.
	Plane() { }

	// Declare the assignment constructor.
	Plane(const Vec3 &_n,const float &_d)
	{
		n = _n;
		d = _d;
	}

	Vec3 		n{ 0,1,0 };   										// Normal of the plane equation
	float  		d{ 0 }; 											// The distance from the origin of the plane 
};


// Defines a transform; position and rotation as a quaternion
class Pose
{
public:
	// Declare the constructor.
	Pose() { }

	// Declare the assignment constructor.
	Pose(const Quat &_q,const Vec3 &_p)
	{
		q = _q;
		p = _p;
	}

	Quat 		q;  													// Quaternion rotation
	Vec3 		p;  													// Origin position of the pose
};


// Defines an axis aligned bounding box
class Bounds3
{
public:
	// Declare the constructor.
	Bounds3() { }

	// Declare the assignment constructor.
	Bounds3(const Vec3 &_bmin,const Vec3 &_bmax)
	{
		bmin = _bmin;
		bmax = _bmax;
	}

	Vec3 		bmin; 												// Minimum axis of bounding box
	Vec3 		bmax; 												// Maximum axis of bounding box
};


// Defines the type of node we are dealing with
enum NodeType
{
	NT_NODE, 							// The base Node class
	NT_PHYSICS_MATERIAL, 				// A physics material
	NT_GEOMETRY_INSTANCE,  				// Defines an instance of a geometry
	NT_TRIANGLEMESH, 					// Defines the contents of a triangle mesh
	NT_CONVEXHULL ,						// Defines the contents of a convex hull
	NT_HEIGHTFIELD , 					// Defines the contents of a heightfield
	NT_RIGID_BODY,   					// Common properties of both static and dynamic rigid bodies
	NT_RIGID_STATIC, 					// A static rigid body
	NT_RIGID_DYNAMIC,  					// A dynamic rigid body
	NT_BODY_PAIR_FILTERS,  				// A node representing a collection of body pair filters
	NT_JOINT,  							// Base class for a joint
	NT_INSTANCE_COLLECTION,				// Instantiates a collection of nodes
	NT_COLLECTION,   					// Defines a collection of nodes
	NT_SCENE,  							// Defines a collection that gets instantiated on startup into a physics scene
};


// Defines an optional visual mesh binding to a physics node
class VisualBinding
{
public:
	std::string	visualName;										// Name of associated visual mesh
	Pose 		localPose;  											// Local relative pose of visual mesh to corresponding physics node
	Vec3 		localScale;   										// Local relative scale of visual mesh to corresponding physics node
};


// Describes a key-value pair for custom properties on a node
class KeyValuePair
{
public:
	std::string	key; 											// They 'key' identifier; what this property is
	std::string	value;   										// The value of this property; up to each the user to figure out how to interpret each property relative to the keyword
};

typedef std::vector< KeyValuePair > KeyValuePairVector; // Forward declare the 'KeyValuePair' vector

// A collection of key/value pair properties relative to a particular category
class AdditionalProperties
{
public:
	std::string	category;  										// The category this set of key/value pairs is associated with (example 'physx', 'mujoco', etc.
	KeyValuePairVector keyValuePairs;  							// The array of key/value pairs associated with this category
};

typedef std::vector< AdditionalProperties > AdditionalPropertiesVector; // Forward declare the 'AdditionalProperties' vector

// Base class that specifies a unique ID and an optional description name field for an object
class Node
{
public:
	// Declare the clone method
	virtual Node *clone() const = 0;

	std::string	id;												// Unique Id for this object
	std::string	name;  											// Optional name for this object
	NodeType 	type{ NT_NODE };   								// The type of node
	VisualBinding  visual;   									// Optional visual bindings for this node; for exaple some physics components have a corresponding named graphics component
	AdditionalPropertiesVector additionalProperties; 			// An optional set of properties for this node; a set of key-value pairs for each application/engine specific category
};


// ShortDescription
class MeshScale
{
public:
	Vec3 		scale;  												// Scale of the mesh on the X,Y,Z axes
	Quat 		rotation; 											// Orientation of the mesh as a quaternion
};


// Defines the physical material properties of a surface
class PhysicsMaterial : public Node
{
public:
	// Declare the constructor.
	PhysicsMaterial()
	{
		Node::type = NT_PHYSICS_MATERIAL;
	};


	// Declare the virtual destructor.
	virtual ~PhysicsMaterial()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	PhysicsMaterial(const PhysicsMaterial &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new PhysicsMaterial(*this);
	}

	// Declare and implement the deep copy assignment operator
	PhysicsMaterial& operator=(const PhysicsMaterial& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			disableFriction = other.disableFriction;
			disableStrongFriction = other.disableStrongFriction;
			dynamicFriction = other.dynamicFriction;
			staticFriction = other.staticFriction;
			restitution = other.restitution;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	PhysicsMaterial(PhysicsMaterial &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	PhysicsMaterial& operator=(PhysicsMaterial&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			disableFriction = other.disableFriction;
			disableStrongFriction = other.disableStrongFriction;
			dynamicFriction = other.dynamicFriction;
			staticFriction = other.staticFriction;
			restitution = other.restitution;
		}
		return *this;
	}

	bool 		disableFriction{ false }; 							// If true, then friction is disabled for the material
	bool 		disableStrongFriction{ false };   					// If true then strong friction is disabled for the material
	float  		dynamicFriction{ 0.5f };  							// The coefficient of dynamic friction.
	float  		staticFriction{ 0.5f }; 							// The coefficient of static friction
	float  		restitution{ 0.5f };  								// The coefficient of resitution.
};

typedef std::vector< Vec3 > Vec3Vector; // Forward declare the 'Vec3' vector

// Describes the data for a convex hull
class ConvexHull : public Node
{
public:
	// Declare the constructor.
	ConvexHull()
	{
		Node::type = NT_CONVEXHULL;
	};


	// Declare the virtual destructor.
	virtual ~ConvexHull()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	ConvexHull(const ConvexHull &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new ConvexHull(*this);
	}

	// Declare and implement the deep copy assignment operator
	ConvexHull& operator=(const ConvexHull& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			points = other.points;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	ConvexHull(ConvexHull &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	ConvexHull& operator=(ConvexHull&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			points = other.points;
		}
		return *this;
	}

	Vec3Vector   points; 										// Array of data points describing the convex hull
};

typedef std::vector< uint32_t > U32Vector; // Forward declare the 'U32' vector

// Describes the data for a triangle mesh
class TriangleMesh : public Node
{
public:
	// Declare the constructor.
	TriangleMesh()
	{
		Node::type = NT_TRIANGLEMESH;
	};


	// Declare the virtual destructor.
	virtual ~TriangleMesh()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	TriangleMesh(const TriangleMesh &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new TriangleMesh(*this);
	}

	// Declare and implement the deep copy assignment operator
	TriangleMesh& operator=(const TriangleMesh& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			points = other.points;
			triangles = other.triangles;
			materialIndices = other.materialIndices;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	TriangleMesh(TriangleMesh &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	TriangleMesh& operator=(TriangleMesh&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			points = other.points;
			triangles = other.triangles;
			materialIndices = other.materialIndices;
		}
		return *this;
	}

	Vec3Vector   points; 										// Array of vertices for the triangle mesh
	U32Vector  	triangles;   									// Array of triangle indices
	U32Vector  	materialIndices; 								// Optional material indices; one for each triangle
};


enum GeometryType
{
	GT_BOX_GEOMETRY, 					// A basic sphere primitive
	GT_SPHERE_GEOMETRY,					// A plane 
	GT_CAPSULE_GEOMETRY, 				// A capsule
	GT_PLANE_GEOMETRY,   				// A simple box primitive
	GT_CYLINDER_GEOMETRY,  				// A cylinder 
	GT_CONVEXHULL_GEOMETRY,				// A convex hull geometry
	GT_TRIANGLEMESH_GEOMETRY,  			// A triangle mesh (can only be static, not dynamic)
};


// Base class for all geometries
class Geometry
{
public:
	// Declare the clone method
	virtual Geometry *clone() const = 0;

	GeometryType type;   										// 
};


// Defines a box geometry
class BoxGeometry : public Geometry
{
public:
	// Declare the constructor.
	BoxGeometry()
	{
		Geometry::type = GT_BOX_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~BoxGeometry()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	BoxGeometry(const BoxGeometry &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new BoxGeometry(*this);
	}

	// Declare and implement the deep copy assignment operator
	BoxGeometry& operator=(const BoxGeometry& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			dimensions = other.dimensions;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	BoxGeometry(BoxGeometry &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	BoxGeometry& operator=(BoxGeometry&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			dimensions = other.dimensions;
		}
		return *this;
	}

	Vec3 		dimensions{ 1,1,1 };									// Dimensions of the box
};


// Defines a sphere geometry
class SphereGeometry : public Geometry
{
public:
	// Declare the constructor.
	SphereGeometry()
	{
		Geometry::type = GT_SPHERE_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~SphereGeometry()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	SphereGeometry(const SphereGeometry &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new SphereGeometry(*this);
	}

	// Declare and implement the deep copy assignment operator
	SphereGeometry& operator=(const SphereGeometry& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			radius = other.radius;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	SphereGeometry(SphereGeometry &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	SphereGeometry& operator=(SphereGeometry&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			radius = other.radius;
		}
		return *this;
	}

	float  		radius{ 1 };  										// The radius of the sphere
};


// Defines a capsule geometry
class CapsuleGeometry : public Geometry
{
public:
	// Declare the constructor.
	CapsuleGeometry()
	{
		Geometry::type = GT_CAPSULE_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~CapsuleGeometry()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	CapsuleGeometry(const CapsuleGeometry &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new CapsuleGeometry(*this);
	}

	// Declare and implement the deep copy assignment operator
	CapsuleGeometry& operator=(const CapsuleGeometry& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			radius = other.radius;
			height = other.height;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	CapsuleGeometry(CapsuleGeometry &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	CapsuleGeometry& operator=(CapsuleGeometry&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			radius = other.radius;
			height = other.height;
		}
		return *this;
	}

	float  		radius{ 1 };  										// The radius of the capsule
	float  		height{ 1 };  										// The height of the capsule
};


// Defines a cylinder geometry
class CylinderGeometry : public Geometry
{
public:
	// Declare the constructor.
	CylinderGeometry()
	{
		Geometry::type = GT_CYLINDER_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~CylinderGeometry()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	CylinderGeometry(const CylinderGeometry &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new CylinderGeometry(*this);
	}

	// Declare and implement the deep copy assignment operator
	CylinderGeometry& operator=(const CylinderGeometry& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			radius = other.radius;
			height = other.height;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	CylinderGeometry(CylinderGeometry &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	CylinderGeometry& operator=(CylinderGeometry&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			radius = other.radius;
			height = other.height;
		}
		return *this;
	}

	float  		radius{ 1 };  										// The radius of the cylinder
	float  		height{ 1 };  										// The height of the cylinder
};


// Defines a convex mesh geometry
class ConvexHullGeometry : public Geometry
{
public:
	// Declare the constructor.
	ConvexHullGeometry()
	{
		Geometry::type = GT_CONVEXHULL_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~ConvexHullGeometry()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	ConvexHullGeometry(const ConvexHullGeometry &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new ConvexHullGeometry(*this);
	}

	// Declare and implement the deep copy assignment operator
	ConvexHullGeometry& operator=(const ConvexHullGeometry& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			scale = other.scale;
			convexMesh = other.convexMesh;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	ConvexHullGeometry(ConvexHullGeometry &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	ConvexHullGeometry& operator=(ConvexHullGeometry&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			scale = other.scale;
			convexMesh = other.convexMesh;
		}
		return *this;
	}

	MeshScale  	scale;   										// The scale to apply to this convex mesh
	std::string	convexMesh;										// The name of the convex mesh asset
};


// Defines a triangle mesh geometry
class TriangleMeshGeometry : public Geometry
{
public:
	// Declare the constructor.
	TriangleMeshGeometry()
	{
		Geometry::type = GT_TRIANGLEMESH_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~TriangleMeshGeometry()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	TriangleMeshGeometry(const TriangleMeshGeometry &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new TriangleMeshGeometry(*this);
	}

	// Declare and implement the deep copy assignment operator
	TriangleMeshGeometry& operator=(const TriangleMeshGeometry& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
			scale = other.scale;
			TriangleMesh = other.TriangleMesh;
			doubleSided = other.doubleSided;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	TriangleMeshGeometry(TriangleMeshGeometry &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	TriangleMeshGeometry& operator=(TriangleMeshGeometry&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
			scale = other.scale;
			TriangleMesh = other.TriangleMesh;
			doubleSided = other.doubleSided;
		}
		return *this;
	}

	MeshScale  	scale;   										// The scale of the triangle mesh
	std::string	TriangleMesh;  									// The name of the triangle mesh asset
	bool 		doubleSided{ false }; 								// Whether or not this triangle mesh should be treated as double sided for collision detection
};


// Defines a plane equation geometry (position and orientation of the plane come from the geometry instance)
class PlaneGeometry : public Geometry
{
public:
	// Declare the constructor.
	PlaneGeometry()
	{
		Geometry::type = GT_PLANE_GEOMETRY;
	};


	// Declare the virtual destructor.
	virtual ~PlaneGeometry()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	PlaneGeometry(const PlaneGeometry &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Geometry* clone() const override
	{
		return new PlaneGeometry(*this);
	}

	// Declare and implement the deep copy assignment operator
	PlaneGeometry& operator=(const PlaneGeometry& other)
	{
		if (this != &other )
		{
			Geometry::operator=(other);
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	PlaneGeometry(PlaneGeometry &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	PlaneGeometry& operator=(PlaneGeometry&& other)
	{
		if (this != &other )
		{
			Geometry::operator=(std::move(other));
		}
		return *this;
	}

};

typedef std::vector< std::string > StringVector; // Forward declare the 'String' vector

// Defines a single instance of a geometry
class GeometryInstance
{
public:

	// Declare the constructor.
	GeometryInstance() { }


	// Declare the virtual destructor; cleanup any pointers or arrays of pointers
	virtual ~GeometryInstance()
	{
		delete geometry; // Delete this object
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	GeometryInstance(const GeometryInstance &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual GeometryInstance* clone() const
	{
		return new GeometryInstance(*this);
	}

	// Declare and implement the deep copy assignment operator
	GeometryInstance& operator=(const GeometryInstance& other)
	{
		if (this != &other )
		{
			delete geometry; // delete any previous pointer.
			geometry = nullptr; // set the pointer to null.
			if ( other.geometry )
			{
				geometry = static_cast<Geometry *>(other.geometry->clone()); // perform the deep copy and assignment here
			}
			materials = other.materials;
			localPose = other.localPose;
			collisionFilterSettings = other.collisionFilterSettings;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	GeometryInstance(GeometryInstance &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	GeometryInstance& operator=(GeometryInstance&& other)
	{
		if (this != &other )
		{
			geometry = other.geometry;
			other.geometry = nullptr; // Set 'other' pointer to null since we have moved it
			materials = other.materials;
			localPose = other.localPose;
			collisionFilterSettings = other.collisionFilterSettings;
		}
		return *this;
	}

	Geometry 	*geometry{ nullptr };								// The geometry associated with this instance
	StringVector materials;										// Array of material id associated with this geometry instance
	Pose 		localPose;  											// The local pose for this geometry instance
	std::string	collisionFilterSettings; 						// Describes collision filtering settings; what other types of objects this object will collide with
};

typedef std::vector< GeometryInstance *> GeometryInstanceVector; // Forward declare the 'GeometryInstance' vector

// Defines the common properties for a rigid body
class RigidBody : public Node
{
public:
	// Declare the constructor.
	RigidBody()
	{
		Node::type = NT_RIGID_BODY;
	};


	// Declare the virtual destructor; cleanup any pointers or arrays of pointers
	virtual ~RigidBody()
	{
		for (auto &i:geometryInstances) delete i; // Delete all of the object pointers in this array
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	RigidBody(const RigidBody &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new RigidBody(*this);
	}

	// Declare and implement the deep copy assignment operator
	RigidBody& operator=(const RigidBody& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			for (auto &i:geometryInstances) delete i; // Delete all of the object pointers in this array
			geometryInstances.clear(); // Clear the current array
			for (auto &i:other.geometryInstances) geometryInstances.push_back( static_cast< GeometryInstance *>(i->clone())); // Deep copy object pointers into the array
			globalPose = other.globalPose;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	RigidBody(RigidBody &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	RigidBody& operator=(RigidBody&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			geometryInstances = other.geometryInstances;
			other.geometryInstances.clear(); // Clear the 'other' array now that we have moved it
			globalPose = other.globalPose;
		}
		return *this;
	}

	GeometryInstanceVector geometryInstances;  					// The set of geometries to instance with this actor
	Pose 		globalPose;   										// The global pose for this actor
};


// Defines a static rigid body
class RigidStatic : public RigidBody
{
public:
	// Declare the constructor.
	RigidStatic()
	{
		Node::type = NT_RIGID_STATIC;
	};


	// Declare the virtual destructor.
	virtual ~RigidStatic()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	RigidStatic(const RigidStatic &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual RigidBody* clone() const override
	{
		return new RigidStatic(*this);
	}

	// Declare and implement the deep copy assignment operator
	RigidStatic& operator=(const RigidStatic& other)
	{
		if (this != &other )
		{
			RigidBody::operator=(other);
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	RigidStatic(RigidStatic &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	RigidStatic& operator=(RigidStatic&& other)
	{
		if (this != &other )
		{
			RigidBody::operator=(std::move(other));
		}
		return *this;
	}

};


// Defines a dynamic rigid body
class RigidDynamic : public RigidBody
{
public:
	// Declare the constructor.
	RigidDynamic()
	{
		Node::type = NT_RIGID_DYNAMIC;
	};


	// Declare the virtual destructor.
	virtual ~RigidDynamic()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	RigidDynamic(const RigidDynamic &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual RigidBody* clone() const override
	{
		return new RigidDynamic(*this);
	}

	// Declare and implement the deep copy assignment operator
	RigidDynamic& operator=(const RigidDynamic& other)
	{
		if (this != &other )
		{
			RigidBody::operator=(other);
			disableGravity = other.disableGravity;
			centerOfMassLocalPose = other.centerOfMassLocalPose;
			mass = other.mass;
			massSpaceInertiaTensor = other.massSpaceInertiaTensor;
			linearVelocity = other.linearVelocity;
			angularVelocity = other.angularVelocity;
			linearDamping = other.linearDamping;
			angularDamping = other.angularDamping;
			maxAngularVelocity = other.maxAngularVelocity;
			kinematic = other.kinematic;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	RigidDynamic(RigidDynamic &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	RigidDynamic& operator=(RigidDynamic&& other)
	{
		if (this != &other )
		{
			RigidBody::operator=(std::move(other));
			disableGravity = other.disableGravity;
			centerOfMassLocalPose = other.centerOfMassLocalPose;
			mass = other.mass;
			massSpaceInertiaTensor = other.massSpaceInertiaTensor;
			linearVelocity = other.linearVelocity;
			angularVelocity = other.angularVelocity;
			linearDamping = other.linearDamping;
			angularDamping = other.angularDamping;
			maxAngularVelocity = other.maxAngularVelocity;
			kinematic = other.kinematic;
		}
		return *this;
	}

	bool 		disableGravity;   									// Disables scene gravity for this actor
	Pose 		centerOfMassLocalPose;  								// Center of mass and local pose
	float  		mass;   											// Sets the mass of a dynamic actor.
	Vec3 		massSpaceInertiaTensor;   							// Sets the inertia tensor, using a parameter specified in mass space coordinates.
	Vec3 		linearVelocity;   									// Sets the linear velocity of the actor.
	Vec3 		angularVelocity;										// Sets the angular velocity of the actor.
	float  		linearDamping{ 0 }; 								// Sets the linear damping coefficient.
	float  		angularDamping{ 0.05f };  							// Sets the angular damping coefficient.
	float  		maxAngularVelocity{ 7 };  							// set the maximum angular velocity permitted for this actor.
	bool 		kinematic{ false };   								// If true this is a dynamic object; but currently kinematically controlled
};


// Defines the common properties for a joint
class Joint : public Node
{
public:
	// Declare the constructor.
	Joint()
	{
		Node::type = NT_JOINT;
	};


	// Declare the virtual destructor.
	virtual ~Joint()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	Joint(const Joint &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new Joint(*this);
	}

	// Declare and implement the deep copy assignment operator
	Joint& operator=(const Joint& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			body0 = other.body0;
			body1 = other.body1;
			localpose0 = other.localpose0;
			localpose1 = other.localpose1;
			collisionEnabled = other.collisionEnabled;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	Joint(Joint &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	Joint& operator=(Joint&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			body0 = other.body0;
			body1 = other.body1;
			localpose0 = other.localpose0;
			localpose1 = other.localpose1;
			collisionEnabled = other.collisionEnabled;
		}
		return *this;
	}

	std::string	body0;   										// Id of first rigid body joint is constrained to; if empty string; then constaint to the world
	std::string	body1;   										// Id of the second rigid body the joint is constrainted to
	Pose 		localpose0;   										// The parent relative pose; relative to body0
	Pose 		localpose1;   										// The parent relative pose; relative to body1
	bool 		collisionEnabled{ false };  							// 
};


// Defines two bodies, by id, that should not collide with each other
class BodyPairFilter
{
public:
	std::string	bodyA;   										// Id of first body
	std::string	bodyB ;											// Id of second body
};

typedef std::vector< BodyPairFilter > BodyPairFilterVector; // Forward declare the 'BodyPairFilter' vector

// A collection of body pair filters
class BodyPairFilters : public Node
{
public:
	// Declare the constructor.
	BodyPairFilters()
	{
		Node::type = NT_BODY_PAIR_FILTERS;
	};


	// Declare the virtual destructor.
	virtual ~BodyPairFilters()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	BodyPairFilters(const BodyPairFilters &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new BodyPairFilters(*this);
	}

	// Declare and implement the deep copy assignment operator
	BodyPairFilters& operator=(const BodyPairFilters& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			bodyPairs = other.bodyPairs;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	BodyPairFilters(BodyPairFilters &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	BodyPairFilters& operator=(BodyPairFilters&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			bodyPairs = other.bodyPairs;
		}
		return *this;
	}

	BodyPairFilterVector bodyPairs;								// Array of body pair filters
};


class InstanceCollection : public Node
{
public:
	// Declare the constructor.
	InstanceCollection()
	{
		Node::type = NT_INSTANCE_COLLECTION;
	};


	// Declare the virtual destructor.
	virtual ~InstanceCollection()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	InstanceCollection(const InstanceCollection &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new InstanceCollection(*this);
	}

	// Declare and implement the deep copy assignment operator
	InstanceCollection& operator=(const InstanceCollection& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			collection = other.collection;
			pose = other.pose;
			scale = other.scale;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	InstanceCollection(InstanceCollection &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	InstanceCollection& operator=(InstanceCollection&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			collection = other.collection;
			pose = other.pose;
			scale = other.scale;
		}
		return *this;
	}

	std::string	collection;										// Name of collection to instance
	Pose 		pose; 												// Pose to instance collection at
	Vec3 		scale;  												// Scale of instance
};

typedef std::vector< Node *> NodeVector; // Forward declare the 'Node' vector

// A collection of nodes
class Collection : public Node
{
public:
	// Declare the constructor.
	Collection()
	{
		Node::type = NT_COLLECTION;
	};


	// Declare the virtual destructor; cleanup any pointers or arrays of pointers
	virtual ~Collection()
	{
		for (auto &i:nodes) delete i; // Delete all of the object pointers in this array
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	Collection(const Collection &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Node* clone() const override
	{
		return new Collection(*this);
	}

	// Declare and implement the deep copy assignment operator
	Collection& operator=(const Collection& other)
	{
		if (this != &other )
		{
			Node::operator=(other);
			for (auto &i:nodes) delete i; // Delete all of the object pointers in this array
			nodes.clear(); // Clear the current array
			for (auto &i:other.nodes) nodes.push_back( static_cast< Node *>(i->clone())); // Deep copy object pointers into the array
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	Collection(Collection &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	Collection& operator=(Collection&& other)
	{
		if (this != &other )
		{
			Node::operator=(std::move(other));
			nodes = other.nodes;
			other.nodes.clear(); // Clear the 'other' array now that we have moved it
		}
		return *this;
	}

	NodeVector   nodes;											// Array of nodes in this collection
};


// A special type of 'collection' which is instantiated on startup
class Scene : public Collection
{
public:
	// Declare the constructor.
	Scene()
	{
		Node::type = NT_SCENE;
	};


	// Declare the virtual destructor.
	virtual ~Scene()
	{
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	Scene(const Scene &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual Collection* clone() const override
	{
		return new Scene(*this);
	}

	// Declare and implement the deep copy assignment operator
	Scene& operator=(const Scene& other)
	{
		if (this != &other )
		{
			Collection::operator=(other);
			gravity = other.gravity;
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	Scene(Scene &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	Scene& operator=(Scene&& other)
	{
		if (this != &other )
		{
			Collection::operator=(std::move(other));
			gravity = other.gravity;
		}
		return *this;
	}

	Vec3 		gravity{ 0.0f,-9.8f,0.0f };   						// Gravity
};

typedef std::vector< Collection *> CollectionVector; // Forward declare the 'Collection' vector
typedef std::vector< Scene *> SceneVector; // Forward declare the 'Scene' vector

// The root node container
class PhysicsDOM
{
public:

	// Declare the constructor.
	PhysicsDOM() { }


	// Declare the virtual destructor; cleanup any pointers or arrays of pointers
	virtual ~PhysicsDOM()
	{
		for (auto &i:collections) delete i; // Delete all of the object pointers in this array
		for (auto &i:scenes) delete i; // Delete all of the object pointers in this array
	}


	// Declare the deep copy constructor; handles copying pointers and pointer arrays
	PhysicsDOM(const PhysicsDOM &other)
	{
		*this = other;
	}


	// Declare the virtual clone method using a deep copy
	virtual PhysicsDOM* clone() const
	{
		return new PhysicsDOM(*this);
	}

	// Declare and implement the deep copy assignment operator
	PhysicsDOM& operator=(const PhysicsDOM& other)
	{
		if (this != &other )
		{
			for (auto &i:collections) delete i; // Delete all of the object pointers in this array
			collections.clear(); // Clear the current array
			for (auto &i:other.collections) collections.push_back( static_cast< Collection *>(i->clone())); // Deep copy object pointers into the array
			for (auto &i:scenes) delete i; // Delete all of the object pointers in this array
			scenes.clear(); // Clear the current array
			for (auto &i:other.scenes) scenes.push_back( static_cast< Scene *>(i->clone())); // Deep copy object pointers into the array
		}
		return *this;
	}


	// Declare the move constructor; handles copying pointers and pointer arrays
	PhysicsDOM(PhysicsDOM &&other)
	{
		*this = std::move(other);
	}

	// Declare and implement the move assignment operator
	PhysicsDOM& operator=(PhysicsDOM&& other)
	{
		if (this != &other )
		{
			collections = other.collections;
			other.collections.clear(); // Clear the 'other' array now that we have moved it
			scenes = other.scenes;
			other.scenes.clear(); // Clear the 'other' array now that we have moved it
		}
		return *this;
	}

	CollectionVector collections;  								// The array of top level collections
	SceneVector	scenes;											// The array of top level scenes; a scene is instantiated into the physics simulation
};



} // End of PHYSICS_DOM namespace

#endif // End of PHYSICSDOM_H
