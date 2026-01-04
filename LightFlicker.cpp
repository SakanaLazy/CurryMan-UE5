#include "LightFlicker_Prototype.h"
#include "Components/LightComponent.h"

ULightFlicker_Prototype::ULightFlicker_Prototype()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULightFlicker_Prototype::BeginPlay()
{
	Super::BeginPlay();

	LightComp = GetOwner()
		? GetOwner()->FindComponentByClass<ULightComponent>()
		: nullptr;

	if (!LightComp)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("%s has no LightComponent!"),
			*GetOwner()->GetName());
		Deactivate();
		return;
	}

	TargetIntensity = LightComp->Intensity;
}

void ULightFlicker_Prototype::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!LightComp)
		return;

	Timer += DeltaTime;

	if (Timer >= FlickerSpeed)
	{
		Timer = 0.f;
		TargetIntensity = FMath::FRandRange(
			MinIntensity,
			MaxIntensity
		);
	}

	float NewIntensity = FMath::FInterpTo(
		LightComp->Intensity,
		TargetIntensity,
		DeltaTime,
		1.f / FMath::Max(Smoothness, KINDA_SMALL_NUMBER)
	);

	LightComp->SetIntensity(NewIntensity);
}
