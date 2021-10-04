// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/Session/SessionHUDWidget.h"

#include "Components/TextBlock.h"
#include "Core/Session/SessionGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UserInterface/TransitionScreenWidget.h"
#include "Sound/SoundCue.h"

void USessionHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TransitionScreen)
	{
		TransitionScreen->Hide();
		TransitionScreen->OnHidden.AddDynamic(this, &USessionHUDWidget::StartPlayingMusic);
	}
}

float USessionHUDWidget::GetSessionTime() const
{
	ASessionGameMode* GameMode = GetSessionGameMode();

	if (!GameMode) return -1.f;

	return GameMode->GetSessionRemainingTime();
}

void USessionHUDWidget::StartPlayingMusic()
{
	UGameplayStatics::PlaySound2D(GetWorld(), GameMusic);
}
