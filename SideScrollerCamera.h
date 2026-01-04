#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SideScrollerCamera_Prototype.generated.h"

UCLASS()
class ASideScrollerCamera_Prototype : public AActor
{
	GENERATED_BODY()

public:
	ASideScrollerCamera_Prototype();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// === Follow Settings ===
	UPROPERTY(EditAnywhere, Category = "Follow")
	AActor* Target;

	UPROPERTY(EditAnywhere, Category = "Follow")
	FVector Offset = FVector(600.f, 200.f, -1000.f);

	UPROPERTY(EditAnywhere, Category = "Follow")
	float SmoothSpeed = 5.f;

	// === Camera Bounds ===
	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MinX = -1000.f;

	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MaxX = 2500.f;

	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MinZ = -500.f;

	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MaxZ = 500.f;

	// === Optional FOV Control ===
	UPROPERTY(EditAnywhere, Category = "Zoom")
	bool bUseZoom = false;

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float DefaultFOV = 60.f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomFOV = 50.f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomSpeed = 2.f;

private:
	UPROPERTY()
	class UCameraComponent* CameraComp;
};
