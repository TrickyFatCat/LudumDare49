// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/Session/GameOverScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Core/TrickyGameInstance.h"
#include "Components/DamageControllerComponent.h"
#include "Core/Session/SessionGameMode.h"


void UGameOverScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_NextLevel)
	{
		Button_NextLevel->OnClicked.AddDynamic(this, &UGameOverScreenWidget::OnNextLevelPressed);
	}
}

void UGameOverScreenWidget::OnTransitionScreenShowed()
{
	Super::OnTransitionScreenShowed();

	if (TransitionCommand == ETransitionCommand::NextLevel)
	{
		OpenNextLevel();
	}
}

void UGameOverScreenWidget::OnNextLevelPressed()
{
	TransitionCommand = ETransitionCommand::NextLevel;
	StartTransition();
}

void UGameOverScreenWidget::OpenNextLevel() const
{
	if (!GetWorld()) return;

	UTrickyGameInstance* GameInstance = GetWorld()->GetGameInstance<UTrickyGameInstance>();

	if (!GameInstance) return;

	const FName NextLevelName = GameInstance->GetNextLevelName();

	if (NextLevelName.IsNone()) return;

	UGameplayStatics::OpenLevel(this, NextLevelName);
}

float UGameOverScreenWidget::GetFinalTime() const
{
	if (!GetWorld()) return -1.f;

	ASessionGameMode* SessionGameMode = Cast<ASessionGameMode>(GetWorld()->GetAuthGameMode());

	if (!SessionGameMode) return -1.f;

	return SessionGameMode->GetFinalTime();
}

bool UGameOverScreenWidget::IsPlayerDead() const
{
	UDamageControllerComponent* DamageController = GetOwningPlayerPawn()->FindComponentByClass<UDamageControllerComponent>();

	if (!DamageController) return false;

	return DamageController->GetIsDead();
}
