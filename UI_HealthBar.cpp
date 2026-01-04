#include "UI_HealthBar_Prototype.h"
#include "Components/ProgressBar.h"
#include "HealthSystem_Prototype.h"

void UUI_HealthBar_Prototype::NativeConstruct()
{
	Super::NativeConstruct();

	if (Target)
	{
		Target->OnHealthChanged.AddDynamic(
			this,
			&UUI_HealthBar_Prototype::OnHealthChanged
		);

		OnHealthChanged(Target->CurrentHealth, Target->MaxHealth);
	}

	if (FillBar)
	{
		FillBar->SetPercent(1.f);
		FillBar->SetFillColorAndOpacity(FullHealthColor);
	}
}

void UUI_HealthBar_Prototype::NativeDestruct()
{
	if (Target)
	{
		Target->OnHealthChanged.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UUI_HealthBar_Prototype::OnHealthChanged(float Current, float Max)
{
	if (Max <= 0.f) return;
	TargetFill = Current / Max;
}

void UUI_HealthBar_Prototype::NativeTick(
	const FGeometry& MyGeometry,
	float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentFill = FMath::FInterpTo(
		CurrentFill,
		TargetFill,
		InDeltaTime,
		SmoothSpeed
	);

	if (FillBar)
	{
		FillBar->SetPercent(CurrentFill);
		FillBar->SetFillColorAndOpacity(
			EvaluateHealthColor(CurrentFill)
		);
	}
}

FLinearColor UUI_HealthBar_Prototype::EvaluateHealthColor(float Alpha) const
{
	if (Alpha > 0.5f)
	{
		float T = (Alpha - 0.5f) / 0.5f;
		return FLinearColor::LerpUsingHSV(
			MidHealthColor,
			FullHealthColor,
			T
		);
	}

	float T = Alpha / 0.5f;
	return FLinearColor::LerpUsingHSV(
		LowHealthColor,
		MidHealthColor,
		T
	);
}
