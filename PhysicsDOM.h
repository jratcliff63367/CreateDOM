#ifndef PHYSICSDOM_H
#define PHYSICSDOM_H

// Warning : This source file was auto-generated by the CreateDOM tool. Do not try to edit this source file manually!

#include <stdint.h>
#include <vector>


namespace PHYSICS_DOM
{


// Defines a basic 3d vector type
class Vec3
{
public:
	float		x{ 0 };	// 
	float		y{ 0 };	// 
	float		z{ 0 };	// 
};


// Defines a basic quaternion data type
class Quat
{
public:
	float		x{ 0 };	// 
	float		y{ 0 };	// 
	float		z{ 0 };	// 
	float		w{ 1 };	// 
};


// Defines a basic plane equation
class Plane
{
public:
	float		a{ 0 };	// X component of the plane equation
	float		b{ 1 };	// Y component of the plane equation
	float		c{ 0 };	// Z component of the plane equation
	float		d{ 0 };	// The distance from the origin of the plane 
};


// Defines a transform; position and rotation as a quaternion
class Pose
{
public:
	Quat		q;	// Quaternion rotation
	Vec3		p;	// Origin position of the pose
};


// Defines the type of node we are dealing with
enum NodeType
{
	NT_NODE,		 // The base Node class
	NT_PHYSICS_MATERIAL,		 // A physics material
	NT_GEOMETRY_INSTANCE,		 // Defines an instance of a geometry
	NT_TRIANGLEMESH,		 // Defines the contents of a triangle mesh
	NT_CONVEXHULL ,		 // Defines the contents of a convex hull
	NT_HEIGHTFIELD ,		 // Defines the contents of a heightfield
	NT_RIGID_BODY,		 // Common properties of both static and dynamic rigid bodies
	NT_RIGID_STATIC,		 // A static rigid body
	NT_RIGID_DYNAMIC,		 // A dynamic rigid body
	NT_BODY_PAIR_FILTERS,		 // A node representing a collection of body pair filters
	NT_JOINT,		 // Base class for a joint
	NT_FIXED_JOINT,		 // A fixed joint
	NT_SPHERICAL_JOINT,		 // A spherical joint
	NT_REVOLUTE_JOINT,		 // A revolute joint
	NT_PRISMATIC_JOINT,		 // A prismatic joint
	NT_DISTANCE_JOINT,		 // A distance joint
	NT_D6_JOINT,		 // A six degree of freedom joint
	NT_INSTANCE_COLLECTION,		 // Instantiates a collection of nodes
	NT_COLLECTION,		 // Defines a collection of nodes
	NT_SCENE,		 // Defines a collection that gets instantiated on startup into a physics scene
};


// Defines an optional visual mesh binding to a physics node
class VisualBinding
{
public:
	std::string		visualName;	// Name of associated visual mesh
	Pose		localPose;	// Local relative pose of visual mesh to corresponding physics node
	Vec3		localScale;	// Local relative scale of visual mesh to corresponding physics node
};


// Base class that specifies a unique ID and an optional description name field for an object
class Node
{
public:
	std::string		id;	// Unique Id for this object
	std::string		name;	// Optional name for this object
	NodeType		type{ NT_NODE };	// The type of node
	VisualBinding		visual;	// 
};


// ShortDescription
class MeshScale
{
public:
	Vec3		scale;	// Scale of the mesh on the X,Y,Z axes
	Quat		rotation;	// Orientation of the mesh as a quaternion
};


// Enumerated type defines how friction between two materials is computed
enum PhysX_CombineMode
{
	CM_AVERAGE,		 // Average: (a + b)/2
	CM_MIN,		 // Minimum: minimum(a,b)
	CM_MULTIPLY,		 // Multiply: a*b
	CM_MAX,		 // Maximum: maximum(a,b)
};


// Defines the physical material properties of a surface
class PhysicsMaterial : public Node
{
public:
	PhysicsMaterial(void)
	{
		Node::type = NT_PHYSICS_MATERIAL;
	};

	bool		disableFriction{ false };	// If true, then friction is disabled for the material
	bool		disableStrongFriction{ false };	// If true then strong friction is disabled for the material
	float		dynamicFriction{ 0.5f };	// The coefficient of dynamic friction.
	float		staticFriction{ 0.5f };	// The coefficient of static friction
	float		restitution{ 0.5f };	// The coefficient of resitution.
	PhysX_CombineMode		physx_frictionCombineMode{ CM_AVERAGE };	// Friction combine mode to set for this material
	PhysX_CombineMode		physx_restitutionCombineMode{ CM_AVERAGE };	// Restitution combine mode to set for this material
};


typedef std::vector< Vec3 > Vec3Vector; // Forward declare the 'Vec3' vector


// Describes the data for a convex hull
class ConvexHull : public Node
{
public:
	ConvexHull(void)
	{
		Node::type = NT_CONVEXHULL;
	};

	Vec3Vector		points;	// Array of data points describing the convex hull
};


typedef std::vector< uint32_t > U32Vector; // Forward declare the 'U32' vector


// Describes the data for a triangle mesh
class TriangleMesh : public Node
{
public:
	TriangleMesh(void)
	{
		Node::type = NT_TRIANGLEMESH;
	};

	Vec3Vector		points;	// Array of vertices for the triangle mesh
	U32Vector		triangles;	// Array of triangle indices
	U32Vector		materialIndices;	// Optional material indices; one for each triangle
};


// The data for a heighfield; as 2d array of 32 bit samples; 16 bits for height, 16 bits for material indices, holes, and other metadata
class HeightField : public Node
{
public:
	HeightField(void)
	{
		Node::type = NT_HEIGHTFIELD;
	};

	uint32_t		rowCount;	// Number of sample rows in the height field samples array.
	uint32_t		columnCount;	// Number of sample columns in the height field samples array.
	U32Vector		samples;	// Heigfield 32 bit samples; low 16 bits is height; high 16 bits determines material index and holes
	float		physx_convexEdgeThreshold{ 0 };	// This threshold is used by the collision detection to determine if a height field edge is convex and can generate contact points.
};


enum GeometryType
{
	GT_BOX_GEOMETRY,		 // A basic sphere primitive
	GT_SPHERE_GEOMETRY,		 // A plane 
	GT_CAPSULE_GEOMETRY,		 // A capsule
	GT_PLANE_GEOMETRY,		 // A simple box primitive
	GT_CYLINDER_GEOMETRY,		 // A cylinder 
	GT_CONVEXHULL_GEOMETRY,		 // A convex hull geometry
	GT_TRIANGLEMESH_GEOMETRY,		 // A triangle mesh (can only be static, not dynamic)
	GT_HEIGHTFIELD_GEOMETRY,		 // A heightfield (can only be static, not dynamic)
};


// Base class for all geometries
class Geometry
{
public:
	GeometryType		type;	// 
};


// Defines a box geometry
class BoxGeometry : public Geometry
{
public:
	BoxGeometry(void)
	{
		Geometry::type = GT_BOX_GEOMETRY;
	};

	float		width{ 1 };	// x dimension of the box
	float		height{ 1 };	// y dimension of the box
	float		depth{ 1 };	// z dimension of the box
};


// Defines a sphere geometry
class SphereGeometry : public Geometry
{
public:
	SphereGeometry(void)
	{
		Geometry::type = GT_SPHERE_GEOMETRY;
	};

	float		radius{ 1 };	// The radius of the sphere
};


// Defines a capsule geometry
class CapsuleGeometry : public Geometry
{
public:
	CapsuleGeometry(void)
	{
		Geometry::type = GT_CAPSULE_GEOMETRY;
	};

	float		radius{ 1 };	// The radius of the capsule
	float		height{ 1 };	// The height of the capsule
};


// Defines a cylinder geometry
class CylinderGeometry : public Geometry
{
public:
	CylinderGeometry(void)
	{
		Geometry::type = GT_CYLINDER_GEOMETRY;
	};

	float		radius{ 1 };	// The radius of the cylinder
	float		height{ 1 };	// The height of the cylinder
};


// Defines a convex mesh geometry
class ConvexHullGeometry : public Geometry
{
public:
	ConvexHullGeometry(void)
	{
		Geometry::type = GT_CONVEXHULL_GEOMETRY;
	};

	MeshScale		scale;	// The scale to apply to this convex mesh
	std::string		convexMesh;	// The name of the convex mesh asset
	float		physx_maxMargin{ 3.40E+38f };	// The maximum margin. Used to limit how much PCM shrinks the geometry by in collision detection.
	bool		physx_tightBounds{ false };	// Use tighter (but more expensive to compute) bounds around the convex geometry.
};


// Defines a triangle mesh geometry
class TriangleMeshGeometry : public Geometry
{
public:
	TriangleMeshGeometry(void)
	{
		Geometry::type = GT_TRIANGLEMESH_GEOMETRY;
	};

	MeshScale		scale;	// The scale of the triangle mesh
	std::string		TriangleMesh;	// The name of the triangle mesh asset
	bool		doubleSided{ false };	// Whether or not this triangle mesh should be treated as double sided for collision detection
};


// Defines a heightfield geometry
class HeightFieldGeometry : public Geometry
{
public:
	HeightFieldGeometry(void)
	{
		Geometry::type = GT_HEIGHTFIELD_GEOMETRY;
	};

	std::string		heightField;	// The id of the heightfield data asset
	float		heightScale;	// The scaling factor for the height field in vertical direction (y direction in local space).
	float		rowScale;	// The scaling factor for the height field in the row direction (x direction in local space).
	float		columnScale;	// The scaling factor for the height field in the column direction (z direction in local space).
	bool		doubleSided{ false };	// Whether or not this heighfield should be treated as double sided for collision detection
};


// Defines a plane equation geometry (position and orientation of the plane come from the geometry instance)
class PlaneGeometry : public Geometry
{
public:
	PlaneGeometry(void)
	{
		Geometry::type = GT_PLANE_GEOMETRY;
	};

};


typedef std::vector< std::string > StringVector; // Forward declare the 'String' vector


// Defines a single instance of a geometry
class GeometryInstance
{
public:
	Geometry		*geometry{ nullptr };	// The geometry associated with this instance
	StringVector		materials;	// Array of material id associated with this geometry instance
	Pose		localPose;	// The local pose for this geometry instance
	std::string		simulationFilterData;	// Collision filtering state for simulation
	std::string		physx_queryFilterData;	// Collision filtering state for queries
	float		physx_contactOffset{ 0.02f };	// Contact offset for this shape
	float		physx_restOffset{ 0 };	// Two shapes will come to rest at a distance equal to the sum of their restOffset values. If the restOffset is 0, they should converge to touching  exactly.  Having a restOffset greater than zero is useful to have objects slide smoothly, so that they do not get hung up on irregularities of  each others' surfaces.
	bool		physx_simulationShape{ true };	// The shape will partake in collision in the physical simulation.
	bool		physx_sceneQueryShape{ true };	// The shape will partake in scene queries (ray casts, overlap tests, sweeps, ...).
	bool		physx_triggerShape{ true };	// The shape is a trigger which can send reports whenever other shapes enter/leave its volume.
	bool		physx_visualization{ true };	// Enable debug renderer for this shape
	bool		physx_particleDrain{ false };	// Sets the shape to be a particle drain.
};


typedef std::vector< GeometryInstance *> GeometryInstanceVector; // Forward declare the 'GeometryInstance' vector


// Defines the common properties for a rigid body
class RigidBody : public Node
{
public:
	RigidBody(void)
	{
		Node::type = NT_RIGID_BODY;
	};

	~RigidBody(void)
	{
		for (auto &i:geometryInstances) delete i;
	};

	GeometryInstanceVector		geometryInstances;	// The set of geometries to instance with this actor
	Pose		globalPose;	// The global pose for this actor
	uint8_t		physx_dominanceGroup{ 0 };	// Assigns dynamic actors a dominance group identifier.
	uint8_t		physx_ownerClient{ 0 };	// the owner client of an actor.
	bool		physx_visualization{ true };	// Enables debug visualization for this object
	bool		physx_sendSleepNotifies{ true };	// Enables the sending of PxSimulationEventCallback::onWake() and PxSimulationEventCallback::onSleep() notify events
	bool		physx_disableSimulation{ false };	// Disables simulation for the actor.
};


// Defines a static rigid body
class RigidStatic : public RigidBody
{
public:
	RigidStatic(void)
	{
		Node::type = NT_RIGID_STATIC;
	};

};


// Defines a dynamic rigid body
class RigidDynamic : public RigidBody
{
public:
	RigidDynamic(void)
	{
		Node::type = NT_RIGID_DYNAMIC;
	};

	bool		disableGravity;	// Disables scene gravity for this actor
	Pose		centerOfMassLocalPose;	// Center of mass and local pose
	float		mass;	// Sets the mass of a dynamic actor.
	Vec3		massSpaceInertiaTensor;	// Sets the inertia tensor, using a parameter specified in mass space coordinates.
	Vec3		linearVelocity;	// Sets the linear velocity of the actor.
	Vec3		angularVelocity;	// Sets the angular velocity of the actor.
	float		linearDamping{ 0 };	// Sets the linear damping coefficient.
	float		angularDamping{ 0.05f };	// Sets the angular damping coefficient.
	float		maxAngularVelocity{ 7 };	// set the maximum angular velocity permitted for this actor.
	bool		kinematic{ false };	// If true this is a dynamic object; but currently kinematically controlled
	float		physx_sleepThreshold{ 5.00E-05f };	// Sets the mass-normalized kinetic energy threshold below which an actor may go to sleep.
	float		physx_wakeCounter{ 0.4f };	// Sets the wake counter for the actor.
	float		physx_stabilizationThreshold{ 1.00E-05f };	// Sets the mass-normalized kinetic energy threshold below which an actor may participate in stabilization.
	bool		physx_kinematicTargetForSceneQueries{ true };	// Use the kinematic target transform for scene queries.
	bool		physx_enableCCD{ false };	// Enables swept integration for the actor.
	bool		physx_enableCCDFriction{ false };	// Enabled CCD in swept integration for the actor.
	bool		physx_poseIntegrationPreview{ false };	// Register a rigid body for reporting pose changes by the simulation at an early stage.
	bool		physx_speculativeCCD{ false };	// Register a rigid body to dynamicly adjust contact offset based on velocity. This can be used to achieve a CCD effect.
	bool		physx_enableCCDMaxContactImpulse{ false };	// Permit CCD to limit maxContactImpulse. This is useful for use-cases like a destruction system but can cause visual artefacts so is not enabled by default.
	float		physx_minCCDAdvanceCoefficient;	// 
	bool		physx_lockLinearX{ false };	// Lock linear movement on the X-axis
	bool		physx_lockLinearY{ false };	// Lock linear movement on the Y-axis
	bool		physx_lockLinearZ{ false };	// Lock linear movement on the Z-axis
	bool		physx_lockAngularX{ false };	// Local angular roation on the X-axis
	bool		physx_lockAngularY{ false };	// Local angular roation on the Y-axis
	bool		physx_lockAngularZ{ false };	// Local angular roation on the Z-axis
	float		physx_maxDepenetrationVelocity;	// 
	float		physx_maxContactImpulse;	// 
	float		physx_contactReportThreshold;	// Sets the force threshold for contact reports.
	uint32_t		physx_minPositionIters{ 4 };	// Sets the solver iteration counts for the body. 
	uint32_t		physx_minVelocityIters{ 1 };	// Sets the solver iteration counts for the body. 
};


// Defines the common properties for a joint
class Joint : public Node
{
public:
	Joint(void)
	{
		Node::type = NT_JOINT;
	};

	std::string		body0;	// Id of first rigid body joint is constrained to; if empty string; then constaint to the world
	std::string		body1;	// Id of the second rigid body the joint is constrainted to
	Pose		localpose0;	// The parent relative pose; relative to body0
	Pose		localpose1;	// The parent relative pose; relative to body1
	bool		collisionEnabled{ false };	// 
	float		physx_breakForce;	// 
	float		physx_breakTorque;	// 
	float		physx_inverseMassScale0;	// 
	float		physx_inverseInertiaScale0;	// 
	float		physx_inverseMassScale1;	// 
	float		physx_inverseInertiaScale1;	// 
	bool		physx_broken{ false };	// 
	bool		physx_projectToBody0{ false };	// 
	bool		physx_projectToBody1{ false };	// 
	bool		physx_visualization{ true };	// 
	bool		physx_driveLimitsAreForces{ false };	// 
	bool		physx_improvedSlerp{ false };	// 
	bool		physx_disablePreProcessing{ true };	// 
	bool		physx_gpuCompatible{ false };	// 
};


class FixedJoint : public Joint
{
public:
	FixedJoint(void)
	{
		Joint::type = NT_FIXED_JOINT;
	};

	float		projectionLinearTolerance;	// 
	float		projectionAngularTolerance;	// 
};


// Defines two bodies, by id, that should not collide with each other
class BodyPairFilter
{
public:
	std::string		bodyA;	// Id of first body
	std::string		bodyB ;	// Id of second body
};


typedef std::vector< BodyPairFilter > BodyPairFilterVector; // Forward declare the 'BodyPairFilter' vector


// A collection of body pair filters
class BodyPairFilters : public Node
{
public:
	BodyPairFilters(void)
	{
		Node::type = NT_BODY_PAIR_FILTERS;
	};

	BodyPairFilterVector		bodyPairs;	// Array of body pair filters
};


class InstanceCollection : public Node
{
public:
	InstanceCollection(void)
	{
		Node::type = NT_INSTANCE_COLLECTION;
	};

	std::string		collection;	// Name of collection to instance
	Vec3		position;	// Position to instance the collection at
	Quat		orientation;	// Orientation of instance
	Vec3		scale;	// Scale of instance
};


typedef std::vector< Node *> NodeVector; // Forward declare the 'Node' vector


// A collection of nodes
class Collection : public Node
{
public:
	Collection(void)
	{
		Node::type = NT_COLLECTION;
	};

	~Collection(void)
	{
		for (auto &i:nodes) delete i;
	};

	NodeVector		nodes;	// Array of nodes in this collection
};


// A special type of 'collection' which is instantiated on startup
class Scene : public Collection
{
public:
	Scene(void)
	{
		Node::type = NT_SCENE;
	};

};



} // End of PHYSICS_DOM namespace

#endif // End of PHYSICSDOM_H
