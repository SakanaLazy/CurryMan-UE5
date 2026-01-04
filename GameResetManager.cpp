#include "GameResetManager_Prototype.h"
#include "HealthSystem_Prototype.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "TimerManager.h"

AGameResetManager_Prototype* AGameResetManager_Prototype::Instance = nullptr;

AGameResetManager_Prototype::AGameResetManager_Prototype()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGameResetManager_Prototype::BeginPlay()
{
	Super::BeginPlay();

	// Singleton setup
	if (Instance == nullptr)
	{
		Instance = this;
	}
	else
	{
		Destroy();
		return;
	}

	Enemies.Empty();

	// Collect all HealthSystems that are NOT players
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (UHealthSystem_Prototype* HS =
			It->FindComponentByClass<UHealthSystem_Prototype>())
		{
			if (!HS->bIsPlayer)
			{
				Enemies.Add(HS);
			}
		}
	}

	UE_LOG(LogTemp, Log,
		TEXT("[GameResetManager] Found %d enemies."),
		Enemies.Num());
}

void AGameResetManager_Prototype::OnCharacterDied(UHealthSystem_Prototype* Health)
{
	if (bResetting || !Health)
		return;

	// Player died → reset
	if (Health->bIsPlayer)
	{
		UE_LOG(LogTemp, Log,
			TEXT("[GameResetManager] Player KO — restarting game."));
		ResetGame();
		return;
	}

	// Enemy died
	Enemies.Remove(Health);

	UE_LOG(LogTemp, Log,
		TEXT("[GameResetManager] NPC defeated. Remaining: %d"),
		Enemies.Num());

	if (Enemies.Num() == 0)
	{
		UE_LOG(LogTemp, Log,
			TEXT("[GameResetManager] All enemies defeated — restarting game."));
		ResetGame();
	}
}

void AGameResetManager_Prototype::ResetGame()
{
	bResetting = true;

	FTimerHandle ResetTimer;
	GetWorld()->GetTimerManager().SetTimer(
		ResetTimer,
		[this]()
		{
			UGameplayStatics::OpenLevel(this, GameSceneName);
		},
		DelayBeforeReset,
		false
	);
}
