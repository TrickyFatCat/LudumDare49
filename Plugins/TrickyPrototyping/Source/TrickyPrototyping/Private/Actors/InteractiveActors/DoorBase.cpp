// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/InteractiveActors/DoorBase.h"
#include "Components/TriggerComponents/BaseBoxTriggerComponent.h"
#include "Components/KeyRingComponent.h"

ADoorBase::ADoorBase()
{
	DoorTrigger = CreateDefaultSubobject<UBaseBoxTriggerComponent>("DoorTrigger");
	SetRootComponent(DoorTrigger);
}

void ADoorBase::BeginPlay()
{
	Super::BeginPlay();

	DoorTrigger->SetIsEnabled(bIsTriggerEnabled);
	
	if (bIsTriggerEnabled)
	{
		DoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoorBase::OnTriggerBeginOverlap);
		DoorTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoorBase::OnTriggerEndOverlap);
	}
}

void ADoorBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADoorBase::Disable()
{
	Super::Disable();

	DoorTrigger->SetIsEnabled(false);	
}

void ADoorBase::Enable()
{
	Super::Enable();

	DoorTrigger->SetIsEnabled(true);
}

void ADoorBase::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                      AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex,
                                      bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	if (IsStateCurrent(EInteractiveActorState::Opened)) return;

	if (bRequireKey)
	{
		UKeyRingComponent* KeyRingComponent = OtherActor->FindComponentByClass<UKeyRingComponent>();

		if (!KeyRingComponent) return;

		if (!KeyRingComponent->HasKey(RequiredKey)) return;
	}

	Open();
}

void ADoorBase::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                    AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex)
{
	if (IsStateCurrent(EInteractiveActorState::Closed)) return;

	Close();
}
