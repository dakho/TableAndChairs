#include "TableAndChairs.h"

ATableAndChairs::ATableAndChairs()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Define the mesh pointer, generate the mesh and set it as root component	
	GeneratedMesh = CreateDefaultSubobject<UProceduralBox>("GeneratedMesh");
	GenerateTableAndChairs();
	GeneratedMesh->OnClicked.AddDynamic(this, &ATableAndChairs::OnCompHit);
	SetRootComponent(GeneratedMesh);
	GeneratedMesh->bUseAsyncCooking = true;


}

void ATableAndChairs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get game mode info to reshape the table and chairs object
	AdpProjectGameModeBase* GameMode = Cast<AdpProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	// Define UWorld pointer
	UWorld* World = GetWorld();
	// Get camera rotation around Z-axis
	auto Rot = World->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation().Yaw;
	// Compute elongation
	float DeltaLength = GetTableElongation(GameMode->DeltaX,GameMode->DeltaY, Rot);	
	// Update table and chairs
	if (TableClicked)
	{
		UpdateTableAndChairs( DeltaScale * DeltaLength, AlongXMember, ApplyFrontMember);
	}
}

void ATableAndChairs::OnCompHit(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
	// Define UWorld pointer
	UWorld* World = GetWorld();
	// Define hit result object
	FHitResult HitResult;
	// Get hit result location
	World->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult);
	FVector2D MouseHitLocation = FVector2D(HitResult.Location.X, HitResult.Location.Y);
	// Get table side to closest clicked position
	GetMouseClosestSide(MouseHitLocation);
	// Table is clicked
	TableClicked = true;
}

void ATableAndChairs::GenerateTable()
{
	// Save starting indices and side 
	Indices.push_back(0);
	SideNumber.push_back(0);
	// Generate table shaped mesh
	TCGeomUtilities::GenerateTable(GeneratedMesh, Indices[0]);
}

void ATableAndChairs::ManageChair(int StartingIndex, float DeltaX, float DeltaY, float Angle, bool Generate)
{
	if (Generate)
		// Save starting index if chair is created
		Indices.push_back(StartingIndex);
	// Manage chair shaped mesh
	TCGeomUtilities::ManageChair(GeneratedMesh, StartingIndex, DeltaX, DeltaY, Angle, Generate);

}

void ATableAndChairs::GenerateTableAndChairs()
{
	// Generate Table
	GenerateTable();
	// Generate first chair - side 1
	ManageChair(GeneratedMesh->GetNumSections(), 0, 0, 0, true); 
	SideNumber.push_back(1);
	// Generate second chair - side 2
	ManageChair(GeneratedMesh->GetNumSections(), 0, TableLength, -90, true);
	SideNumber.push_back(2);
	// Generate third chair - side 3
	ManageChair(GeneratedMesh->GetNumSections(), TableLength, TableLength, 180, true);
	SideNumber.push_back(3);
	// Generate fourth chair - side 4
	ManageChair(GeneratedMesh->GetNumSections(), TableLength, 0, 90, true); //chair_4 
	SideNumber.push_back(4);

}

void ATableAndChairs::UpdateTableAndChairs(float Delta, bool AlongX, bool ApplyFront)
{
	if (AlongX) // The input is along table X-axis
	{
		// Count how many chairs there are on the side transversal to the input applied one
		int NumberTransversal = CountChairPerSide(2);
		// Count how many chairs there are on the input applied side
		int NumberParallel = CountChairPerSide(3);
		if (ApplyFront) // The input is in the positive X direction -> Movement applied to side 3
		{
			CurrentActedSide = 3; 
			// Compute new table length 
			float NewXLength = std::max(XLength + Delta, TableLength);
			if (Delta >= 0)
				NewXLength = XLength + Delta;
			// Manage table update
			ManageTableSide(LastTableXPos, LastTableYPos, NewXLength, YLength, CurrentActedSide, NumberTransversal, NumberParallel);
			// Update table X length
			XLength = NewXLength;
		}
		else // The input is in the negative X direction -> Movement applied to side 1
		{
			CurrentActedSide = 1;
			// Compute table new starting position
			if (XLength > TableLength)
				LastTableXPos -= Delta;
			// Manage table update
			ManageTableSide(LastTableXPos, LastTableYPos, XLength, YLength, CurrentActedSide, NumberTransversal, NumberParallel);
			// Update table X length
			XLength = std::max(TableLength, XLength + Delta);

		}
		// Create new chairs on sides 2 and 4 if table dimension can fit them
		if (XLength >= (NumberTransversal + 1) * ChairLength + (NumberTransversal + 2) * ChairDistance && ChairCreated == false)
		{
			// Create another chair on side 2
			ManageChair(GeneratedMesh->GetNumSections(), NumberTransversal * (ChairDistance + ChairLength), YLength, -90, true);
			SideNumber.push_back(2);
			// Create another chair on side 4
			ManageChair(GeneratedMesh->GetNumSections(), XLength + (NumberTransversal - 1) * (ChairDistance + ChairLength), LastTableYPos, 90, true);
			SideNumber.push_back(4);
			ChairCreated = true;
		}
		// Chairs are not created until the table dimension can fit them
		else if (XLength < (NumberTransversal + 1) * ChairLength + (NumberTransversal + 2) * ChairDistance)
		{
			ChairCreated = false;
		}
		// Delete chairs if table dimensions cannot fit them anymore
		if (XLength < (NumberTransversal - 1) * ChairLength + (NumberTransversal + 1) * ChairDistance)
		{
			// Delete chairs
			DeleteChairs(2, ApplyFront);
			// New number of transversal chair
			int NewNumberTransversal = NumberTransversal - 1;
			// Manage table update
			ManageTableSide(LastTableXPos, LastTableYPos, XLength, YLength, 3, NewNumberTransversal, NumberParallel);
		}
	}
	else // The input is along the Y-axis
	{
		// Count how many chairs there are on the side transversal to the input applied one
		int NumberTransversal = CountChairPerSide(1);
		// Count how many chairs there are on the input applied side
		int NumberParallel = CountChairPerSide(2);		
		if (ApplyFront) // The input is in the positive Y direction -> Movement applied to side 2
		{
			CurrentActedSide = 2; 
			// Compute new table length 
			float NewYLength = std::max(YLength + Delta, TableLength);
			if (Delta >= 0)
				NewYLength = YLength + Delta;
			// Manage table update
			ManageTableSide(LastTableXPos, LastTableYPos, XLength, NewYLength, CurrentActedSide, NumberTransversal, NumberParallel);
			// Update table Y length
			YLength = NewYLength;
		}
		else // The input is in the negative Y direction -> Movement applied to side 4
		{
			CurrentActedSide = 4; 
			// Compute table new starting position
			if (YLength > TableLength)
				LastTableYPos -= Delta;
			// Manage table update
			ManageTableSide(LastTableXPos, LastTableYPos, XLength, YLength, CurrentActedSide, NumberTransversal, NumberParallel);
			// Update table Y length
			YLength = std::max(TableLength, YLength + Delta);

		}
		// Create new chairs on sides 1 and 3 if table dimension can fit them
		if (YLength >= (NumberTransversal + 1) * ChairLength + (NumberTransversal + 2) * ChairDistance && ChairCreated == false)
		{
			// Create another chair on side 1
			ManageChair(GeneratedMesh->GetNumSections(), LastTableXPos, NumberTransversal * (ChairDistance + ChairLength), 0, true); //
			SideNumber.push_back(1);
			// Create another chair on side 3
			ManageChair(GeneratedMesh->GetNumSections(), XLength, YLength + (NumberTransversal - 1) * (ChairDistance + ChairLength), 180, true);
			SideNumber.push_back(3);
			ChairCreated = true;
		}
		// Chairs are not created until the table dimension can fit them
		else if (YLength < (NumberTransversal + 1) * ChairLength + (NumberTransversal + 2) * ChairDistance)
		{
			ChairCreated = false;
		}
		// Delete chairs if table dimensions cannot fit them anymore
		if (YLength < (NumberTransversal - 1) * ChairLength + (NumberTransversal + 1) * ChairDistance)
		{
			// Delete chairs
			DeleteChairs(1, ApplyFront);
			// New number of transversal chair
			int NewNumberTransversal = NumberTransversal - 1;
			// Manage table update
			ManageTableSide(LastTableXPos, LastTableYPos, XLength, YLength, 2, NewNumberTransversal, NumberParallel);
		}
	}

}

void  ATableAndChairs::DeleteChairs(int SideNumberToFind, bool LastOne)
{
	// Set isIndicesValue to obtain the index
	bool isIndicesValue = false;
	// Get last/first created chair index (according to LastOne) of both sides
	auto Index1 = GetCreatedChairIndex(SideNumberToFind, LastOne, isIndicesValue);
	auto Index2 = GetCreatedChairIndex(SideNumberToFind + 2, LastOne, isIndicesValue);
	// Clear the mesh on both table sides
	ClearChairMesh(Index1, isIndicesValue);
	ClearChairMesh(Index2, isIndicesValue);
	// Clear member value at provided index
	ClearChairInfo(Index1);
	ClearChairInfo(Index2);
}

int ATableAndChairs::CountChairPerSide(int SideNumberToCount)
{
	int count = 0;
	for (int i = 0; i != SideNumber.size(); i++)
	{
		if (SideNumber[i] == SideNumberToCount)
			count++;
	}

	return count;

}

void ATableAndChairs::ClearChairMesh(int StartingIndex, bool isIndicesValue)
{
	int EffIndex = -1;
	if (isIndicesValue)
		EffIndex = StartingIndex;
	else
		EffIndex = Indices[StartingIndex];
	// Clear all mesh section from starting index
	for (int i = EffIndex; i != EffIndex + NBoxChair; i++)
	{
		GeneratedMesh->ClearMeshSection(i);
	}
}

std::vector<int> ATableAndChairs::GetChairMeshIndices(int SideNumberToFind)
{
	// Initialize the vector of indices to be returned
	int Length = CountChairPerSide(SideNumberToFind);
	std::vector<int> indices(Length, 0);
	// Loop to find the indices
	int counter = 0;	
	for (int i = 0; i != SideNumber.size(); i++)
	{
		if (SideNumber[i] == SideNumberToFind)
		{
			indices[counter] = Indices[i];
			counter++;
		}
	}

	return indices;
}

int ATableAndChairs::GetCreatedChairIndex(int SideNumberToFind, bool Reverse, bool GetIndicesValue)
{
	int index = -1;
	if (Reverse) // Look for last created mesh
	{
		for (int i = SideNumber.size() - 1; i != -1; i--)
		{
			if (SideNumber[i] == SideNumberToFind)
			{
				index = i;
				break;
			}
		}
	}
	else // Look for first created mesh
	{
		for (int i = 0; i != SideNumber.size(); i++)
		{
			if (SideNumber[i] == SideNumberToFind)
			{
				index = i;
				break;
			}
		}
	}
	// Return Indices value according to input
	if (GetIndicesValue)
		index = Indices[index];

	return index;
}

void ATableAndChairs::ClearChairInfo(int Index)
{
	Indices.at(Index) = -1;
	SideNumber.at(Index) = -1;
}

void ATableAndChairs::ManageTableSide(float& px, float& py, float& Lx, float& Ly, int SideIndex, int& NumberTransversal, int& NumberParallel)
{
	// Initialize useful variables
	auto IndicesVector = GetChairMeshIndices(SideIndex);
	int TransversalIndex1 = 0;
	int TransversalIndex2 = 0;
	std::vector<int> TransversalIndicesVector1;
	std::vector<int> TransversalIndicesVector2;
	float DeltaPos = 0.f;
	float DeltaPosTransversal1 = 0.f;
	float DeltaPosTransversal2 = 0.f;
	float Rot = 0.f;
	float TransversalRot1 = 0.f;
	float TransversalRot2 = 0.f;
	// Increase table counter top
	GeneratedMesh->UpdateMesh(FVector(px, py, TableHeight - TableThickness), FVector(Lx, Ly, TableThickness), 0);
	// Manage table legs and chairs associated to side which is moved
	// First perform peculiar side operations...
	switch (SideIndex)
	{
	case 1:
		// Move table legs associated - Side 1
		GeneratedMesh->UpdateMesh(FVector(px, py, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), 1); //1
		GeneratedMesh->UpdateMesh(FVector(px, py + Ly - TableThickness, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), 4); //4
		// Chair manipulation variable initialization - Side 1
		DeltaPos = py + Ly / 2 - ChairLength * NumberParallel / 2 - ChairDistance * (NumberParallel - 1) / 2 - ChairDistance;
		Rot = 0;
		TransversalIndex1 = 2;
		TransversalIndex2 = 4;
		// Move all the chairs to follow the table - Side 1
		for (int i = 0; i != IndicesVector.size(); i++)
		{
			ManageChair(IndicesVector[i], px, DeltaPos + i * (ChairLength + ChairDistance), Rot, false);
		}
		break;

	case 2:
		// Move table legs associated - Side 2
		GeneratedMesh->UpdateMesh(FVector(px + Lx - TableThickness, py + Ly - TableThickness, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), 3); //3
		GeneratedMesh->UpdateMesh(FVector(px, py + Ly - TableThickness, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), 4); //4
		// Chair manipulation variable initialization - Side 2
		DeltaPos = px + Lx / 2 - ChairLength * NumberParallel / 2 - ChairDistance * (NumberParallel - 1) / 2 - ChairDistance;
		Rot = -90;
		TransversalIndex1 = 1;
		TransversalIndex2 = 3;
		// Move all the chairs to follow the table - Side 2
		for (int i = 0; i != IndicesVector.size(); i++)
		{
			ManageChair(IndicesVector[i], DeltaPos + i * (ChairLength + ChairDistance), py + Ly, Rot, false);
		}
		break;

	case 3:
		// Move table legs associated - Side 3
		GeneratedMesh->UpdateMesh(FVector(px + Lx - TableThickness, py, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), 2); //2
		GeneratedMesh->UpdateMesh(FVector(px + Lx - TableThickness, py + Ly - TableThickness, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), 3); //3
		// Chair manipulation variable initialization - Side 3
		DeltaPos = py + Ly / 2 + ChairLength * (2 - NumberParallel) / 2 - ChairDistance * (NumberParallel - 1) / 2 + ChairDistance;
		Rot = 180;
		TransversalIndex1 = 2;
		TransversalIndex2 = 4;
		// Move all the chairs to follow the table - Side 3
		for (int i = 0; i != IndicesVector.size(); i++)
		{
			ManageChair(IndicesVector[i], px + Lx, DeltaPos + i * (ChairLength + ChairDistance), Rot, false);
		}
		break;

	case 4:
		// Move table legs associated - Side 4
		GeneratedMesh->UpdateMesh(FVector(px, py, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), 1); //1
		GeneratedMesh->UpdateMesh(FVector(px + Lx - TableThickness, py, 0), FVector(TableThickness, TableThickness, TableHeight - TableThickness), 2); //2
		// Chair manipulation variable initialization - Side 4
		DeltaPos = px + Lx / 2 + ChairLength * (2 - NumberParallel) / 2 - ChairDistance * (NumberParallel - 1) / 2 + ChairDistance;
		Rot = 90;
		TransversalIndex1 = 1;
		TransversalIndex2 = 3;
		// Move all the chairs to follow the table - Side 4 
		for (int i = 0; i != IndicesVector.size(); i++)
		{
			ManageChair(IndicesVector[i], DeltaPos + i * (ChairLength + ChairDistance), py, Rot, false);
		}
		break;
	}
	// ... then operations if side is odd or even
	// Get all chairs on opposite sides starting indices 
	TransversalIndicesVector1 = GetChairMeshIndices(TransversalIndex1);
	TransversalIndicesVector2 = GetChairMeshIndices(TransversalIndex2);
	if (SideIndex % 2 != 0)
	{
		// Set chairs rotation
		TransversalRot1 = -90;
		TransversalRot2 = 90;
		// Set first chairs new relative distance
		DeltaPosTransversal1 = px + Lx / 2 - ChairLength * NumberTransversal / 2 - ChairDistance * (NumberTransversal - 1) / 2 - ChairDistance;
		DeltaPosTransversal2 = px + Lx / 2 + ChairLength * (2 - NumberTransversal) / 2 - ChairDistance * (NumberTransversal - 1) / 2 + ChairDistance;
		// Move the chairs to make them centered
		for (int i = 0; i != TransversalIndicesVector1.size(); i++)
		{
			ManageChair(TransversalIndicesVector1[i], DeltaPosTransversal1 + i * (ChairLength + ChairDistance), py + Ly, TransversalRot1, false);
			ManageChair(TransversalIndicesVector2[i], DeltaPosTransversal2 + i * (ChairLength + ChairDistance), py, TransversalRot2, false);
		}
	}
	else
	{	// Set chairs rotation
		TransversalRot1 = 0;
		TransversalRot2 = 180;
		// Set first chairs new relative distance
		DeltaPosTransversal1 = py + Ly / 2 - ChairLength * NumberTransversal / 2 - ChairDistance * (NumberTransversal - 1) / 2 - ChairDistance;
		DeltaPosTransversal2 = py + Ly / 2 + ChairLength * (2 - NumberTransversal) / 2 - ChairDistance * (NumberTransversal - 1) / 2 + ChairDistance;
		// Move the chairs to make them centered
		for (int i = 0; i != TransversalIndicesVector1.size(); i++)
		{
			ManageChair(TransversalIndicesVector1[i], px, DeltaPosTransversal1 + i * (ChairLength + ChairDistance), TransversalRot1, false);
			ManageChair(TransversalIndicesVector2[i], px + Lx, DeltaPosTransversal2 + i * (ChairLength + ChairDistance), TransversalRot2, false);
		}
	}

}

void ATableAndChairs::GetMouseClosestSide(FVector2D& MousePosition)
{
	// Compute each side central position
	FVector2D Pos1 = FVector2D(LastTableXPos, 0.5 * (YLength + LastTableYPos));
	FVector2D Pos2 = FVector2D(0.5*(LastTableXPos + XLength), YLength);
	FVector2D Pos3 = FVector2D(XLength, 0.5 * (YLength + LastTableYPos));
	FVector2D Pos4 = FVector2D(0.5 * (LastTableXPos + XLength), LastTableYPos);
	// Append them to Tarray
	TArray<FVector2D> SidePositions; 
	SidePositions.Add(Pos1);
	SidePositions.Add(Pos2);
	SidePositions.Add(Pos3);
	SidePositions.Add(Pos4);
	// Define vector of distances
	std::vector<float> Distances(4, 0);
	for (int i = 0; i != 4; i++)
	{
		// Compute distances from each table side central position
		Distances[i] = FVector2D::Distance(SidePositions[i], MousePosition);
	}
	// Find side with minimum distance
	int minElementIndex = std::min_element(Distances.begin(), Distances.end()) - Distances.begin();
	minElementIndex++;
	// Print info 
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Acting on table side: " + FString::FromInt(minElementIndex));
	// Set member variables value according to obtained table side
	switch (minElementIndex)
	{
	case 1:
		AlongXMember = true;
		ApplyFrontMember = false;
		CurrentActedSide = 1;
		break; 

	case 2:
		AlongXMember = false;
		ApplyFrontMember = true;
		CurrentActedSide = 2;
		break;

	case 3:
		AlongXMember = true;
		ApplyFrontMember = true;
		CurrentActedSide = 3;
		break;

	case 4:
		AlongXMember = false;
		ApplyFrontMember = false;
		CurrentActedSide = 4;
	}

}

float ATableAndChairs::GetTableElongation(float& DX, float& DY, float& CameraRotation)
{
	// Compute elongation according to acted table side and camera rotation
	const double pi = std::acos(-1);
	float Delta2Return = 0;
	switch (CurrentActedSide)
	{
	case 1:
		Delta2Return = - DY * std::cos(CameraRotation * pi / 180) + DX * std::sin(CameraRotation * pi / 180);
		break;
	case 2:
		Delta2Return = DX * std::cos(CameraRotation * pi / 180) + DY * std::sin(CameraRotation * pi / 180);
		break;
	case 3:
		Delta2Return = DY * std::cos(CameraRotation * pi / 180) - DX * std::sin(CameraRotation * pi / 180);
		break;
	case 4:

		Delta2Return = - DX * std::cos(CameraRotation * pi / 180) - DY * std::sin(CameraRotation * pi / 180);
	}
		
	return Delta2Return;
}