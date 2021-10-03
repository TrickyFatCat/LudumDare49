// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/InteractiveActors/ButtonBase.h"
#include "Components/TriggerComponents/InteractionSphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "TimerManager.h"

AButtonBase::AButtonBase()
{
	ButtonRoot = CreateDefaultSubobject<USceneComponent>("ButtonRoot");
	SetRootComponent(ButtonRoot);

	ButtonTrigger = CreateDefaultSubobject<UInteractionSphereComponent>("ButtonTrigger");
	ButtonTrigger->SetupAttachment(ButtonRoot);
}

void AButtonBase::BeginPlay()
{
	Super::BeginPlay();

	ButtonTrigger->SetIsNormalTrigger(!bRequireInteraction);
	ButtonTrigger->bRequireLineOfSight = bRequireLineOfSight;

	if (!bRequireInteraction)
	{
		ButtonTrigger->OnComponentBeginOverlap.AddDynamic(this, &AButtonBase::OnTriggerBeginOverlap);
		ButtonTrigger->OnComponentEndOverlap.AddDynamic(this, &AButtonBase::OnTriggerEndOverlap);
	}
}

void AButtonBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AButtonBase::Disable()
{
	Super::Disable();

	ButtonTrigger->SetIsEnabled(false);
}

void AButtonBase::Enable()
{
	if (bPressOnce) return;

	Super::Enable();
	ButtonTrigger->SetIsEnabled(true);
}

void AButtonBase::FinishAnimation()
{
	Super::FinishAnimation();

	if (bPressOnce)
	{
		Disable();
		return;
	}

	switch (ButtonBehaviour)
	{
	case EButtonBehaviour::Key:
		if (IsStateCurrent(EInteractiveActorState::Opened))
		{
			if (!GetWorld()) return;

			GetWorld()->GetTimerManager().SetTimer(AutoCloseDelayHandle,
			                                       this,
			                                       &AButtonBase::Close,
			                                       KeyAutoCloseDelayDuration,
			                                       false);
		}
		else if (IsStateCurrent(EInteractiveActorState::Closed))
		{
			ButtonTrigger->SetIsEnabled(true);
		}
		break;

	case EButtonBehaviour::Switch:
		ButtonTrigger->SetIsEnabled(true);
		break;
	}
}

bool AButtonBase::ProcessInteraction_Implementation(APlayerController* PlayerController)
{
	if (!PlayerController || !bRequireInteraction || IsStateCurrent(EInteractiveActorState::Locked)) return false;

	if (!GetIsReversible() && IsStateCurrent(EInteractiveActorState::Transition)) return false;
	
	StartAnimation();

	return true;
}

void AButtonBase::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex,
                                        bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (bRequireInteraction) return;

	if (BeginOverlapDelay > 0.f)
	{
		FTimerManager& TimerManager = GetWorldTimerManager();

		if (TimerManager.IsTimerActive(EndOverlapDelayHandle))
		{
			TimerManager.ClearTimer(EndOverlapDelayHandle);
		}

		TimerManager.SetTimer(BeginOverlapDelayHandle,
		                      this,
		                      &AButtonBase::ProcessTriggerOverlap,
		                      BeginOverlapDelay,
		                      false);

		return;
	}

	ProcessTriggerOverlap();
}

void AButtonBase::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                      AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex)
{
	if (bRequireInteraction) return;

	if (EndOverlapDelay > 0.f)
	{
		FTimerManager& TimerManager = GetWorldTimerManager();

		if (TimerManager.IsTimerActive(BeginOverlapDelayHandle))
		{
			TimerManager.ClearTimer(BeginOverlapDelayHandle);
		}

		if (!GetIsReversible()) return;

		TimerManager.SetTimer(EndOverlapDelayHandle, this, &AButtonBase::ProcessTriggerOverlap, EndOverlapDelay, false);
		
		return;
	}

	ProcessTriggerOverlap();
}

void AButtonBase::ProcessTriggerOverlap()
{
	const EInteractiveActorState State = AnimationTimeline->IsPlaying() && GetIsReversible()
		                                     ? GetStateTarget()
		                                     : GetStateCurrent();

	switch (State)
	{
	case EInteractiveActorState::Closed:
		Open();
		break;

	case EInteractiveActorState::Opened:
		if (ButtonBehaviour == EButtonBehaviour::Key) return;

		Close();
		break;
	}
}
