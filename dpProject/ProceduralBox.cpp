#include "ProceduralBox.h"

UProceduralBox::UProceduralBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bUseComplexAsSimpleCollision = true;
}

void UProceduralBox::GenerateMesh(const FVector& Position, const FVector& Size, int SectionIndex)
{
	// Create the vertices and triangles corresponding to a paparallelepiped
	PpFromBottomLeft(Position.X, Position.Y, Position.Z, Size.X, Size.Y, Size.Z);
	// Create the mesh section
	CreateMeshSection(SectionIndex, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	// Clear vertices and triangles
	ClearSectionData();
}

void UProceduralBox::UpdateMesh(const FVector& Position, const FVector& Size, int SectionIndex)
{
	// Create the vertices and triangles corresponding to a paparallelepiped
	PpFromBottomLeft(Position.X, Position.Y, Position.Z, Size.X, Size.Y, Size.Z);
	// Create the mesh section
	UpdateMeshSection(SectionIndex, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
	// Clear vertices and triangles
	ClearSectionData();
}

void UProceduralBox::AddTriangle(const int& V1, const int& V2, const int& V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

void UProceduralBox::PpFromBottomLeft(const float& px, const float& py, const float& pz, const float& Lx, const float& Ly, const float& Lz)
{
	// 6 faces, 4 verts each (corners)

	// Define the vertices
	Vertices.Add(FVector(px + std::min(0.f, Lx), py + std::min(0.f, Ly), pz + std::min(0.f, Lz))); // lower left - 0
	Vertices.Add(FVector(px + std::min(0.f, Lx), py + std::min(0.f, Ly), pz + std::max(0.f, Lz))); // upper left - 1
	Vertices.Add(FVector(px + std::min(0.f, Lx), py + std::max(0.f, Ly), pz + std::min(0.f, Lz))); // lower right - 2 
	Vertices.Add(FVector(px + std::min(0.f, Lx), py + std::max(0.f, Ly), pz + std::max(0.f, Lz))); // upper right - 3

	Vertices.Add(FVector(px + std::max(0.f, Lx), py + std::min(0.f, Ly), pz + std::min(0.f, Lz))); // lower front left - 4
	Vertices.Add(FVector(px + std::max(0.f, Lx), py + std::min(0.f, Ly), pz + std::max(0.f, Lz))); // upper front left - 5
	Vertices.Add(FVector(px + std::max(0.f, Lx), py + std::max(0.f, Ly), pz + std::max(0.f, Lz))); // upper front right - 6
	Vertices.Add(FVector(px + std::max(0.f, Lx), py + std::max(0.f, Ly), pz + std::min(0.f, Lz))); // lower front right - 7

	// Define the faces
	AddTriangle(0, 2, 3);
	AddTriangle(3, 1, 0);

	AddTriangle(0, 1, 4);
	AddTriangle(4, 1, 5);

	AddTriangle(4, 5, 7);
	AddTriangle(7, 5, 6);

	AddTriangle(7, 6, 3);
	AddTriangle(3, 2, 7);

	AddTriangle(1, 3, 5);
	AddTriangle(6, 5, 3);

	AddTriangle(2, 0, 4);
	AddTriangle(4, 7, 2);

	AddTriangle(0, 2, 3);
	AddTriangle(3, 1, 0);

	AddTriangle(0, 1, 4);
	AddTriangle(4, 1, 5);

	AddTriangle(4, 5, 7);
	AddTriangle(7, 5, 6);

	AddTriangle(7, 6, 3);
	AddTriangle(3, 2, 7);

	AddTriangle(1, 3, 5);
	AddTriangle(5, 6, 3);

	AddTriangle(2, 0, 4);
	AddTriangle(4, 7, 2);

}


void UProceduralBox::ClearSectionData()
{
	Vertices.Empty();
	Triangles.Empty();
}