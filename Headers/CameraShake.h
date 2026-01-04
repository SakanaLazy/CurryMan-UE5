#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraShake_Prototype.generated.h"

UCLASS()
class ACameraShake_Prototype : public AActor
{
	GENERATED_BODY()

public:
	ACameraShake_Prototype();

	static ACameraShake_Prototype* Instance;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// === API ===
	void Shake(float Duration, float Magnitude);

private:
	FVector OriginalLocation;

	float ShakeDuration = 0.f;
	float ShakeMagnitude = 0.f;
	float ElapsedTime = 0.f;

	bool bIsShaking = false;
};
