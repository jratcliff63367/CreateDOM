#ifndef DEEPCOPY_H
#define DEEPCOPY_H

// CreateDOM: Schema Generation tool written by John W. Ratcliff, 2017
// Warning:This source file was auto-generated by the CreateDOM tool. Do not try to edit this source file manually!
// The Google DOCs Schema Spreadsheet for this source came from: https://docs.google.com/spreadsheets/d/118I5kdu2XT-6wfCG044937xfEKDyX2oNg04G8Wqi6o0/edit?usp=sharing

#include <stdint.h>


namespace DEEP_COPY
{


enum GeometryType
{
	GT_BOX_GEOMETRY, 					// A basic sphere primitive
	GT_SPHERE_GEOMETRY,					// A plane 
};


// Base class for all geometries
class Geometry
{
public:
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
	}

	float  		width{ 1 }; 										// 
	float  		depth{ 1 }; 										// 
	float  		height{ 1 };  										// Dimensions of the box
};


// Defines a sphere geometry
class SphereGeometry : public Geometry
{
public:
	// Declare the constructor.
	SphereGeometry()
	{
		Geometry::type = GT_SPHERE_GEOMETRY;
	}

	float  		radius{ 1 };  										// The radius of the sphere
};


// Defines a single instance of a geometry
class GeometryInstance
{
public:
	uint32_t 	geometriesCount { 0 };
	Geometry**   geometries{ nullptr };							// The geometry associated with this instance
};



} // End of DEEP_COPY namespace

#endif // End of Geometry
