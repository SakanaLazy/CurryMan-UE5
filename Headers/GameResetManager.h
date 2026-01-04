#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameResetManager_Prototype.generated.h"

UCLASS()
class AGameResetManager_Prototype : public AActor
{
	GENERATED_BODY()

public:
	AGameResetManager_Prototype();

	static AGameResetManager_Prototype* Instance;

protected:
	virtual void BeginPlay() override;

public:
	// === Scene Names ===
	UPROPERTY(EditAnywhere, Category = "Scenes")
	FName GameSceneName = "CurryCity";

	// === Delays ===
	UPROPERTY(EditAnywhere, Category = "Timing")
	float DelayBeforeReset = 3.f;

	// === API ===
	void OnCharacterDied(class UHealthSystem_Prototype* Health);

private:
	TArray<class UHealthSystem_Prototype*> Enemies;
	bool bResetting = false;

	void ResetGame();
};
