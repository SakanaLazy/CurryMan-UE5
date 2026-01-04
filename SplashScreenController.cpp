#include "SplashScreenController_Prototype.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ASplashScreenController_Prototype::ASplashScreenController_Prototype()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASplashScreenController_Prototype::BeginPlay()
{
	Super::BeginPlay();

	StateTimer = 0.f;

	if (LogoWidget && LogoWidget->GetUserWidgetObject())
	{
		LogoWidget->GetUserWidgetObject()->SetOpacity(0.f);
	}
}

void ASplashScreenController_Prototype::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!LogoWidget || !LogoWidget->GetUserWidgetObject())
		return;

	StateTimer += DeltaTime;

	switch (CurrentState)
	{
	case ESplashState::FadeIn:
	{
		float Alpha = FMath::Clamp(StateTimer / FadeInDuration, 0.f, 1.f);
		UpdateFade(Alpha);

		if (StateTimer >= FadeInDuration)
		{
			CurrentState = ESplashState::Hold;
			StateTimer = 0.f;
		}
		break;
	}
	case ESplashState::Hold:
	{
		if (StateTimer >= HoldDuration)
		{
			CurrentState = ESplashState::FadeOut;
			StateTimer = 0.f;
		}
		break;
	}
	case ESplashState::FadeOut:
	{
		float Alpha = 1.f - FMath::Clamp(StateTimer / FadeOutDuration, 0.f, 1.f);
		UpdateFade(Alpha);

		if (StateTimer >= FadeOutDuration)
		{
			CurrentState = ESplashState::Wait;
			StateTimer = 0.f;
		}
		break;
	}
	case ESplashState::Wait:
	{
		if (StateTimer >= WaitBeforeLoad)
		{
			UGameplayStatics::OpenLevel(this, NextLevelName);
			CurrentState = ESplashState::Done;
		}
		break;
	}
	default:
		break;
	}
}

void ASplashScreenController_Prototype::UpdateFade(float Alpha)
{
	if (UUserWidget* Widget = LogoWidget->GetUserWidgetObject())
	{
		Widget->SetOpacity(Alpha);
	}
}
