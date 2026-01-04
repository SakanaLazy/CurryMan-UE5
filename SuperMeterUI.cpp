#include "SuperMeterUI_Prototype.h"
#include "Components/ProgressBar.h"

void USuperMeterUI_Prototype::SetFill(float Value)
{
	if (!FillBar) return;

	FillBar->SetPercent(
		FMath::Clamp(Value, 0.f, 1.f)
	);
}
