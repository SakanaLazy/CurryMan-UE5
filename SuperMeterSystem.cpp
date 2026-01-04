#include "SuperMeterSystem_Prototype.h"
#include "HitDetector_Prototype.h"
#include "SuperController_Prototype.h"
#include "SuperMeterUI_Prototype.h"

USuperMeterSystem_Prototype::USuperMeterSystem_Prototype()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USuperMeterSystem_Prototype::BeginPlay()
{
	Super::BeginPlay();

	// Lock supers immediately on startup
	if (SuperController)
		SuperController->Deactivate();

	UpdateUI();

	// Subscribe to global hit event
	UHitDetector_Prototype::OnHitLanded.AddUObject(
		this,
		&USuperMeterSystem_Prototype::AddHit
	);
}

void USuperMeterSystem_Prototype::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UHitDetector_Prototype::OnHitLanded.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void USuperMeterSystem_Prototype::AddHit(int32 TotalHits)
{
	CurrentHits = FMath::Min(CurrentHits + 1, HitsToFill);
	UpdateUI();

	if (CurrentHits >= HitsToFill)
	{
		UnlockSuper();
	}
}

void USuperMeterSystem_Prototype::UnlockSuper()
{
	if (SuperController)
	{
		SuperController->Activate();
		UE_LOG(LogTemp, Log, TEXT("Super Ready!"));
	}
}

void USuperMeterSystem_Prototype::UpdateUI()
{
	if (!UIWidget) return;

	if (USuperMeterUI_Prototype* UI =
		Cast<USuperMeterUI_Prototype>(UIWidget))
	{
		UI->SetFill(
			static_cast<float>(CurrentHits) / HitsToFill
		);
	}
}

void USuperMeterSystem_Prototype::ResetMeter()
{
	CurrentHits = 0;
	UpdateUI();

	if (SuperController)
		SuperController->Deactivate();
}
