#include "SideScrollerCamera_Prototype.h"
#include "Camera/CameraComponent.h"

ASideScrollerCamera_Prototype::ASideScrollerCamera_Prototype()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = CameraComp;
}

void ASideScrollerCamera_Prototype::BeginPlay()
{
	Super::BeginPlay();

	if (CameraComp && bUseZoom)
	{
		CameraComp->SetFieldOfView(DefaultFOV);
	}
}

void ASideScrollerCamera_Prototype::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Target) return;

	// --- Follow Logic ---
	FVector DesiredPos = Target->GetActorLocation() + Offset;
	FVector SmoothedPos = FMath::VInterpTo(
		GetActorLocation(),
		DesiredPos,
		DeltaTime,
		SmoothSpeed
	);

	// --- Clamp Movement ---
	SmoothedPos.X = FMath::Clamp(SmoothedPos.X, MinX, MaxX);
	SmoothedPos.Z = FMath::Clamp(SmoothedPos.Z, MinZ, MaxZ);

	SetActorLocation(SmoothedPos);
}
