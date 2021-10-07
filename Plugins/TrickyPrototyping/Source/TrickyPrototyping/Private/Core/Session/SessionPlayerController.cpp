// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Core/Session/SessionPlayerController.h"

#include "Core/Session/SessionGameMode.h"
#include "Kismet/GameplayStatics.h"

ASessionPlayerController::ASessionPlayerController()
{
}

void ASessionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World)
	{
		ASessionGameMode* GameMode = Cast<ASessionGameMode>(World->GetAuthGameMode());

		if (GameMode)
		{
			GameMode->OnSessionStateChanged.AddUObject(this, &ASessionPlayerController::OnSessionStateChanged);
		}
	}

	bShowMouseCursor = false;
}

void ASessionPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASessionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent) return;

	FInputActionBinding& Pause = InputComponent->BindAction("Pause",
	                                                        IE_Pressed,
	                                                        this,
	                                                        &ASessionPlayerController::ProcessGamePause);
	Pause.bExecuteWhenPaused = true;
}

void ASessionPlayerController::OnSessionStateChanged(const ESessionState NewState)
{
	if (NewState == ESessionState::Progress)
	{
		bShowMouseCursor = bShowCursorOnStart;
		SetInputMode(FInputModeGameOnly());
		EnableInput(this);
	}
	else
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
		DisableInput(this);
	}

	if (NewState == ESessionState::GameOver)
	{
		StopMovement();
		GetPawn()->TurnOff();
	}
}

void ASessionPlayerController::ProcessGamePause()
{
	if (!GetWorld()) return;

	if (IsPaused())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		return;
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}
