#include "DynamicFocus_Prototype.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"

ADynamicFocus_Prototype::ADynamicFocus_Prototype()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADynamicFocus_Prototype::BeginPlay()
{
	Super::BeginPlay();

	// Auto-find player by tag if not assigned
	if (!PlayerActor)
	{
		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Players);
		if (Players.Num() > 0)
			PlayerActor = Players[0];
	}

	if (PostProcessVolume)
	{
		bHasDOF = PostProcessVolume->Settings.bOverride_DepthOfFieldFocalDistance;
	}
}

void ADynamicFocus_Prototype::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerActor || !PostProcessVolume)
		return;

	FPostProcessSettings& Settings = PostProcessVolume->Settings;

	// Ensure DOF is active
	Settings.bOverride_DepthOfFieldFocalDistance = true;

	float Distance =
		FVector::Dist(
			GetActorLocation(),
			PlayerActor->GetActorLocation()
		);

	Settings.DepthOfFieldFocalDistance = Distance;
}
