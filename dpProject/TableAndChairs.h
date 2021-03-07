/////////////////////////////////////////////////////////////////////////////////
//	Class:
//		ATableAndChairs
//	Description:
//		Actor class of table with its chairs. Table dimension, chairs number and 
//		positioning can be modified according to provided input
//	SourceFiles: 
//		TableAndChairs.cpp
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralBox.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include "TCUtilities.h"
#include "dpProjectGameModeBase.h"
#include "TableAndChairs.generated.h"

UCLASS()
class DPPROJECT_API ATableAndChairs : public AActor
{
	GENERATED_BODY()

public:
	// Default constructor
	ATableAndChairs();
	// ProceduralBox pointer
	UProceduralBox* GeneratedMesh;
	// Increase/decrease scale factor
	UPROPERTY(EditAnywhere, Category = "GameControls|ScaleFactor", meta = (ClampMin = "5", ClampMax = "40", UIMin = "10", UIMax = "40"))
		float DeltaScale = 20;

	////////////////
	// Tick function
	////////////////
	virtual void Tick(float DeltaTime) override;

	///////////////////////////////////
	// Fuction called when table is hit
	///////////////////////////////////
	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);


protected:
	// Table defualt dimensions
	const float TableLength = TCGeomUtilities::TableLength;
	const float TableHeight = TCGeomUtilities::TableHeight;
	const float TableThickness = TCGeomUtilities::TableThickness;
	// Chair default dimensions
	const float ChairLength = TCGeomUtilities::ChairLength;
	const float ChairHeight = TCGeomUtilities::ChairHeight;
	const float ChairThickness = TCGeomUtilities::ChairThickness;
	const float BackRestHeight = TCGeomUtilities::BackRestHeight;
	const float ChairDistance = TCGeomUtilities::ChairDistance;
	// Number of boxes needed to build table mesh
	const int NBoxTable = 5;
	// Number of boxes needed to build table mesh
	const int NBoxChair = 6;
	// Vector to store all created table/chairs starting mesh indices
	std::vector<int32> Indices;
	// Vector to store Indices element corresponding table side information
	std::vector<int32> SideNumber;
	// Dynamic table dimensions
	float XLength = TableLength;
	float YLength = TableLength;
	// Dynamic table locations
	float LastTableXPos = 0;
	float LastTableYPos = 0;
	// Bool to check if chair is created during table update
	bool ChairCreated = false;
	// Bool to check if table is clicked by player
	bool TableClicked = false;
	// Bool to check if elongation is applied along table X-axis or not (i.e. along Y one)
	bool AlongXMember = false;
	// Bool to check if elongation is applied along positive axis direction or not (i.e. along negative one)
	bool ApplyFrontMember = false;
	// Current table side on which action is performed
	int CurrentActedSide = 0;


	////////////////////////////////////
	// Method to generate the table mesh
	////////////////////////////////////
	void GenerateTable();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Method to generate/update the chair mesh
	// StartingIndex : first mesh section index of the chair mesh, DeltaX : X-position offset
	// DeltaY : Y-position offset, Angle : rotation angle, Generate : generate the mesh if true, update otherwise
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ManageChair(int StartingIndex, float DeltaX, float DeltaY, float Angle, bool Generate);

	//////////////////////////////////////////////////
	// Method to generate both table and chairs meshes
	//////////////////////////////////////////////////
	void GenerateTableAndChairs();

	////////////////////////////////////////////////////////////////////////////////////////////
	// Method to update table dimension, chair position and number
	// Delta : amount of increae/decrease of table size, AlongX : true if input is along X-axis,
	// ApplyFront : true if input is in positive axis direction
	////////////////////////////////////////////////////////////////////////////////////////////
	void UpdateTableAndChairs(float Delta, bool AlongX, bool ApplyFront);

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Method to delete chairs along the input side and the opposite one
	// SideNumberToFind : side number input, LasOne : delete last created if true, the fisrt otherwise
	//////////////////////////////////////////////////////////////////////////////////////////////////
	void DeleteChairs(int SideNumberToFind, bool LastOne);

	//////////////////////////////////////////////////////////////
	// Method to count how many chairs there are on the input side
	//////////////////////////////////////////////////////////////
	int CountChairPerSide(int SideNumberToCount);

	//////////////////////////////////////////////////////////////////////////
	// Method to clear all the section associated to one chair
	// StartingIndex : index of the first chair mesh section,
	// isIndicesValue : true if Indices member should be used, false otherwise
	//////////////////////////////////////////////////////////////////////////
	void ClearChairMesh(int StartingIndex, bool isIndicesValue);

	///////////////////////////////////////////////////////////////////////////////////
	// Method to clear Indices and SideNumber member, i.e their values are set to dummy
	// Index : index to be set to dummy
	///////////////////////////////////////////////////////////////////////////////////
	void ClearChairInfo(int Index);

	////////////////////////////////////////////////////////////////
	// Method to get all chair along the input side starting indices
	////////////////////////////////////////////////////////////////
	std::vector<int> GetChairMeshIndices(int SideNumberToFind);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Method to get the last or the first created chair along the input side
	// SideNumberToFind : side number input, Reverse : get the last created chair if true, the first otherwise
	// GetIndicesValues : true to get Indices member value
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetCreatedChairIndex(int SideNumberToFind, bool Reverse, bool GetIndicesValue);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Method to manage the table side which is going to be updated after input
	// px : table X position, py : table Y position, Lx : table X length, Ly : table Y Length
	// SideIndex : table side which is updated, NumberTransversal : number of chairs along transversal sides,
	// NumberParallel : number of chairs along input side
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ManageTableSide(float& px, float& py, float& Lx, float& Ly, int SideIndex, int& NumberTransversal, int& NumberParallel);

	//////////////////////////////////////////////////////////////////
	// Method to get the table side closest to player clicked location
	// MousePosition : mouse world position
	//////////////////////////////////////////////////////////////////
	void GetMouseClosestSide(FVector2D& MousePosition);

	//////////////////////////////////////////////////////////
	// Method to compute table elongation that must be applied
	// DX : mouse delta along X, DY : mouse delta along Y, 
	// CameraRotation : current camera rotation around Z-axis
	//////////////////////////////////////////////////////////
	float GetTableElongation(float& DX, float& DY, float& CameraRotation);
};
