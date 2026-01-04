#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioManager_Prototype.generated.h"

UCLASS()
class AAudioManager_Prototype : public AActor
{
	GENERATED_BODY()

public:
	AAudioManager_Prototype();

	static AAudioManager_Prototype* Instance;

protected:
	virtual void BeginPlay() override;

public:
	// === Clips ===
	UPROPERTY(EditAnywhere, Category = "Audio|Clips")
	class USoundBase* PunchHit;

	UPROPERTY(EditAnywhere, Category = "Audio|Clips")
	USoundBase* SuperActivate;

	UPROPERTY(EditAnywhere, Category = "Audio|Clips")
	USoundBase* KOSound;

	UPROPERTY(EditAnywhere, Category = "Audio|Clips")
	USoundBase* Ambient;

	// === API ===
	void PlayOneShot(USoundBase* Clip, float Volume = 0.8f);

	void PlayHit();
	void PlaySuper();
	void PlayKO();

	void StopMusic();
	void PauseMusic();
	void ResumeMusic();

private:
	UPROPERTY()
	class UAudioComponent* SFXComponent;

	UPROPERTY()
	class UAudioComponent* MusicComponent;
};
