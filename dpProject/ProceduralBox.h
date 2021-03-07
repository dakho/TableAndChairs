///////////////////////////////////////////////////////////////////////////////
//	Class:
//		UProceduralBox
//	Description:
//		Procedural mesh derived class. It provides the tools to generate a mesh
//		with box shape.
//	SourceFiles: 
//		ProceduralBox.cpp
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include <algorithm>
#include "ProceduralMeshComponent.h"
#include "ProceduralBox.generated.h"

UCLASS()
class DPPROJECT_API UProceduralBox : public UProceduralMeshComponent
{
		GENERATED_BODY()
public:
	// Default Constructor
	UProceduralBox();
	// Constructor
	UProceduralBox(const FObjectInitializer & ObjectInitializer);

	///////////////////////////////////////////////////////////////
	// Method to generate a mesh with a box shape
	// Position : reference corner position, Size : size of the box
	// SectionIndex : index used to generate the mesh
	///////////////////////////////////////////////////////////////
	void GenerateMesh(const FVector & Position, const FVector & Size, int SectionIndex);

	///////////////////////////////////////////////////////////////
	// Method to update a mesh with a box shape
	// Position : reference corner position, Size : size of the box
	// SectionIndex : index used to generate the mesh
	///////////////////////////////////////////////////////////////
	void UpdateMesh(const FVector & Position, const FVector & Size, int SectionIndex);

protected:
	// The vertices of the mesh
	TArray<FVector> Vertices;
	// The triangles of the mesh
	TArray<int32> Triangles;

	///////////////////////////////////////////
	// Method to add the vertices to a triangle
	// Vi-th : i-th vertex of the triangle
	///////////////////////////////////////////
	void AddTriangle(const int& V1, const int& V2, const int& V3);

	///////////////////////////////////////////////////////////////////////////
	// Method to construct a parallelepiped starting from bottom left corner
	// pi-th : position along the i-th axis, Li-th : length along the i-th axis
	///////////////////////////////////////////////////////////////////////////
	void PpFromBottomLeft(const float& px, const float& py, const float& pz, const float& Lx, const float& Ly, const float& Lz);

	/////////////////////////////////////////////////////
	// Method to clear Vertices and Triangles of the mesh
	/////////////////////////////////////////////////////
	void ClearSectionData();
};
