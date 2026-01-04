#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplashScreenController_Prototype.generated.h"

UCLASS()
class ASplashScreenController_Prototype : public AActor
{
	GENERATED_BODY()

public:
	ASplashScreenController_Prototype();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// === References ===
	UPROPERTY(EditAnywhere, Category = "References")
	class UWidgetComponent* LogoWidget;

	// === Timing Settings ===
	UPROPERTY(EditAnywhere, Category = "Timing")
	float FadeInDuration = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Timing")
	float HoldDuration = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Timing")
	float FadeOutDuration = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Timing")
	float WaitBeforeLoad = 0.3f;

	// === Next Level ===
	UPROPERTY(EditAnywhere, Category = "Level")
	FName NextLevelName = "CurryCity";

private:
	enum class ESplashState
	{
		FadeIn,
		Hold,
		FadeOut,
		Wait,
		Done
	};

	ESplashState CurrentState = ESplashState::FadeIn;
	float StateTimer = 0.f;

	void UpdateFade(float Alpha);
};
