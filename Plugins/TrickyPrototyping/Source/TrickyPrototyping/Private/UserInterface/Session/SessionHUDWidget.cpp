// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "UserInterface/Session/SessionHUDWidget.h"

#include "Components/TextBlock.h"
#include "Core/Session/SessionGameMode.h"
#include "UserInterface/TransitionScreenWidget.h"

void USessionHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TransitionScreen)
	{
		TransitionScreen->Hide();
	}
}

float USessionHUDWidget::GetSessionTime() const
{
	ASessionGameMode* GameMode = GetSessionGameMode();

	if (!GameMode) return -1.f;

	return GameMode->GetSessionRemainingTime();
}
