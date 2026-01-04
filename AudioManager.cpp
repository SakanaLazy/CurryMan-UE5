#include "AudioManager_Prototype.h"
#include "Components/AudioComponent.h"

AAudioManager_Prototype* AAudioManager_Prototype::Instance = nullptr;

AAudioManager_Prototype::AAudioManager_Prototype()
{
	PrimaryActorTick.bCanEverTick = false;

	SFXComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SFXComponent"));
	MusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicComponent"));

	RootComponent = SFXComponent;

	MusicComponent->SetupAttachment(RootComponent);
}

void AAudioManager_Prototype::BeginPlay()
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

	// Music settings
	if (MusicComponent)
	{
		MusicComponent->bAutoActivate = false;
		MusicComponent->SetVolumeMultiplier(0.35f);
		MusicComponent->bIsUISound = true;
	}

	// Start ambient music
	if (Ambient && MusicComponent && !MusicComponent->IsPlaying())
	{
		MusicComponent->SetSound(Ambient);
		MusicComponent->Play();
	}
}

void AAudioManager_Prototype::PlayOneShot(USoundBase* Clip, float Volume)
{
	if (!Clip || !SFXComponent) return;

	SFXComponent->SetSound(Clip);
	SFXComponent->SetVolumeMultiplier(Volume);
	SFXComponent->Play();
}

void AAudioManager_Prototype::PlayHit()
{
	PlayOneShot(PunchHit, 0.1f);
}

void AAudioManager_Prototype::PlaySuper()
{
	PlayOneShot(SuperActivate, 0.05f);
}

void AAudioManager_Prototype::PlayKO()
{
	PlayOneShot(KOSound, 0.15f);
}

void AAudioManager_Prototype::StopMusic()
{
	if (MusicComponent)
		MusicComponent->Stop();
}

void AAudioManager_Prototype::PauseMusic()
{
	if (MusicComponent)
		MusicComponent->SetPaused(true);
}

void AAudioManager_Prototype::ResumeMusic()
{
	if (MusicComponent)
		MusicComponent->SetPaused(false);
}
