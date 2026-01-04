#include "CameraShake_Prototype.h"
#include "Kismet/KismetMathLibrary.h"

ACameraShake_Prototype* ACameraShake_Prototype::Instance = nullptr;

ACameraShake_Prototype::ACameraShake_Prototype()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACameraShake_Prototype::BeginPlay()
{
	Super::BeginPlay();

	Instance = this;
	OriginalLocation = GetActorLocation();
}

void ACameraShake_Prototype::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsShaking)
		return;

	ElapsedTime += DeltaTime;

	if (ElapsedTime >= ShakeDuration)
	{
		// Reset
		SetActorLocation(OriginalLocation);
		bIsShaking = false;
		return;
	}

	float X = FMath::FRandRange(-1.f, 1.f) * ShakeMagnitude;
	float Y = FMath::FRandRange(-1.f, 1.f) * ShakeMagnitude;

	FVector Offset(X, Y, 0.f);
	SetActorLocation(OriginalLocation + Offset);
}

void ACameraShake_Prototype::Shake(float Duration, float Magnitude)
{
	ShakeDuration = Duration;
	ShakeMagnitude = Magnitude;
	ElapsedTime = 0.f;
	bIsShaking = true;
}
