#include "TCUtilities.h"

// Set table geometric dimension values
float TCGeomUtilities::TableLength = 100;
float TCGeomUtilities::TableHeight = 100;
float TCGeomUtilities::TableThickness = 10;
// Set chair geometric dimension values
float TCGeomUtilities::ChairLength = 40;
float TCGeomUtilities::ChairHeight = 60;
float TCGeomUtilities::ChairThickness = 5;
float TCGeomUtilities::BackRestHeight = 50;
float TCGeomUtilities::ChairDistance = (TableLength - ChairLength) / 2;

void TCGeomUtilities::GenerateTable(UProceduralBox* Mesh, int& StartingIndex)
{
	// Countertop
	Mesh->GenerateMesh(FVector(0, 0, TableHeight - TableThickness), FVector(TableLength, TableLength, TableThickness), StartingIndex);
	// First leg
	Mesh->GenerateMesh(FVector::ZeroVector, FVector(TableThickness, TableThickness, TableHeight - TableThickness), StartingIndex + 1);
	// Second leg
	Mesh->GenerateMesh(FVector(TableLength - TableThickness, 0, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), StartingIndex + 2);
	// Third leg
	Mesh->GenerateMesh(FVector(TableLength - TableThickness, TableLength - TableThickness, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), StartingIndex + 3);
	// Fourth leg
	Mesh->GenerateMesh(FVector(0, TableLength - TableThickness, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), StartingIndex + 4);
}

void TCGeomUtilities::ManageChair(UProceduralBox* Mesh, int StartingIndex, float DeltaX, float DeltaY, float Angle, bool Generate)
{
	// First: find the default starting position and dimension according to the rotation angle provided
	const double pi = std::acos(-1);

	// Point 1
	float p1_x_rot = -ChairLength * std::cos(Angle * pi / 180) - ChairDistance * std::sin(Angle * pi / 180);
	float p1_y_rot = -ChairLength * std::sin(Angle * pi / 180) + ChairDistance * std::cos(Angle * pi / 180);
	// Length 1
	float L1_x_rot = ChairLength * std::cos(Angle * pi / 180) - ChairLength * std::sin(Angle * pi / 180);
	float L1_y_rot = ChairLength * std::sin(Angle * pi / 180) + ChairLength * std::cos(Angle * pi / 180);
	// Length 2
	float L2_x_rot = ChairThickness * std::cos(Angle * pi / 180) - ChairThickness * std::sin(Angle * pi / 180);
	float L2_y_rot = ChairThickness * std::sin(Angle * pi / 180) + ChairThickness * std::cos(Angle * pi / 180);
	// Point 2
	float p2_x_rot = -ChairThickness * std::cos(Angle * pi / 180) - ChairDistance * std::sin(Angle * pi / 180);
	float p2_y_rot = -ChairThickness * std::sin(Angle * pi / 180) + ChairDistance * std::cos(Angle * pi / 180);
	// Point 3
	float p3_x_rot = -ChairThickness * std::cos(Angle * pi / 180) - (ChairDistance + (ChairLength - ChairThickness)) * std::sin(Angle * pi / 180);
	float p3_y_rot = -ChairThickness * std::sin(Angle * pi / 180) + (ChairDistance + (ChairLength - ChairThickness)) * std::cos(Angle * pi / 180);
	// Point 4
	float p4_x_rot = -ChairLength * std::cos(Angle * pi / 180) - (ChairDistance + (ChairLength - ChairThickness)) * std::sin(Angle * pi / 180);
	float p4_y_rot = -ChairLength * std::sin(Angle * pi / 180) + (ChairDistance + (ChairLength - ChairThickness)) * std::cos(Angle * pi / 180);
	// Length 3
	float L3_x_rot = ChairThickness * std::cos(Angle * pi / 180) - ChairLength * std::sin(Angle * pi / 180);
	float L3_y_rot = ChairThickness * std::sin(Angle * pi / 180) + ChairLength * std::cos(Angle * pi / 180);

	if (Generate) // Generate the mesh
	{
		// Use ProceduralBox GenerateMesh method providing the position offset given
		// Seat
		Mesh->GenerateMesh(FVector(DeltaX + p1_x_rot, DeltaY + p1_y_rot, ChairHeight - ChairThickness), FVector(L1_x_rot, L1_y_rot, ChairThickness), StartingIndex);
		// First leg
		Mesh->GenerateMesh(FVector(DeltaX + p1_x_rot, DeltaY + p1_y_rot, 0), FVector(L2_x_rot, L2_y_rot, ChairHeight - ChairThickness), StartingIndex + 1);
		// Second leg
		Mesh->GenerateMesh(FVector(DeltaX + p2_x_rot, DeltaY + p2_y_rot, 0), FVector(L2_x_rot, L2_y_rot, ChairHeight - ChairThickness), StartingIndex + 2);
		// Third leg
		Mesh->GenerateMesh(FVector(DeltaX + p3_x_rot, DeltaY + p3_y_rot, 0), FVector(L2_x_rot, L2_y_rot, ChairHeight - ChairThickness), StartingIndex + 3);
		// Fourth leg
		Mesh->GenerateMesh(FVector(DeltaX + p4_x_rot, DeltaY + p4_y_rot, 0), FVector(L2_x_rot, L2_y_rot, ChairHeight - ChairThickness), StartingIndex + 4);
		// Backrest
		Mesh->GenerateMesh(FVector(DeltaX + p1_x_rot, DeltaY + p1_y_rot, ChairHeight), FVector(L3_x_rot, L3_y_rot, ChairHeight - ChairThickness), StartingIndex + 5);
	}
	else // Update the mesh
	{
		// Use ProceduralBox UpdateMesh method providing the position offset given
		// Seat
		Mesh->UpdateMesh(FVector(DeltaX + p1_x_rot, DeltaY + p1_y_rot, ChairHeight - ChairThickness), FVector(L1_x_rot, L1_y_rot, ChairThickness), StartingIndex);
		// First leg
		Mesh->UpdateMesh(FVector(DeltaX + p1_x_rot, DeltaY + p1_y_rot, 0), FVector(L2_x_rot, L2_y_rot, ChairHeight - ChairThickness), StartingIndex + 1);
		// Second leg
		Mesh->UpdateMesh(FVector(DeltaX + p2_x_rot, DeltaY + p2_y_rot, 0), FVector(L2_x_rot, L2_y_rot, ChairHeight - ChairThickness), StartingIndex + 2);
		// Third leg
		Mesh->UpdateMesh(FVector(DeltaX + p3_x_rot, DeltaY + p3_y_rot, 0), FVector(L2_x_rot, L2_y_rot, ChairHeight - ChairThickness), StartingIndex + 3);
		// Fourth leg
		Mesh->UpdateMesh(FVector(DeltaX + p4_x_rot, DeltaY + p4_y_rot, 0), FVector(L2_x_rot, L2_y_rot, ChairHeight - ChairThickness), StartingIndex + 4);
		// Backrest
		Mesh->UpdateMesh(FVector(DeltaX + p1_x_rot, DeltaY + p1_y_rot, ChairHeight), FVector(L3_x_rot, L3_y_rot, ChairHeight - ChairThickness), StartingIndex + 5);
	}

}
