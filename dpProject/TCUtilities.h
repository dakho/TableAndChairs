////////////////////////////////////////////////////////////////////////////
//	Class:
//		TCGeomUtilities
//	Description:
//		Static class with useful methods and members to manipulate table and
//		chairs geometry
//	SourceFiles: 
//		TCUtilities.cpp
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include <cmath>
#include "ProceduralBox.h"


class DPPROJECT_API TCGeomUtilities
{
public:

	// Table geometric parameters
	static float TableLength;
	static float TableHeight;
	static float TableThickness;
	// Chairs geometric parameters
	static float ChairLength;
	static float ChairHeight;
	static float ChairThickness;
	static float BackRestHeight;
	static float ChairDistance;

	////////////////////////////////////////////////
	// Method to generate the table mesh
	// Mesh : mesh class used to generate the table, 
	// StartingIndex : starting mesh index
	////////////////////////////////////////////////
	static void GenerateTable(UProceduralBox* Mesh, int& StartingIndex);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Method to generate and update one chair mesh
	// Mesh : mesh class used to generate the table, StartingIndex : starting mesh index, DeltaX : X-position offset,
	// DeltaY : Y-position offset, Angle : rotation angle, Generate : generate the mesh if true, update otherwise
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void ManageChair(UProceduralBox* Mesh, int StartingIndex, float DeltaX, float DeltaY, float Angle, bool Generate);

private:
	// Hide Default Constructor
	TCGeomUtilities() {}

};
