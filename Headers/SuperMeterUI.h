#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SuperMeterUI_Prototype.generated.h"

UCLASS()
class USuperMeterUI_Prototype : public UUserWidget
{
	GENERATED_BODY()

public:
	// === References ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UProgressBar* FillBar;

	// === API ===
	UFUNCTION()
	void SetFill(float Value);
};
