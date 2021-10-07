// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/InteractiveActors/DoorBase.h"
#include "Components/TriggerComponents/BaseBoxTriggerComponent.h"
#include "Components/KeyRingComponent.h"
#include "Kismet/KismetTextLibrary.h"

ADoorBase::ADoorBase()
{
	DoorTrigger = CreateDefaultSubobject<UBaseBoxTriggerComponent>("DoorTrigger");
	SetRootComponent(DoorTrigger);

	Messages.Add(CreateDefaultSubobject<UTextRenderComponent>("Message_01"));
	Messages[0]->SetupAttachment(GetRootComponent());
	Messages.Add(CreateDefaultSubobject<UTextRenderComponent>("Message_02"));
	Messages[1]->SetupAttachment(GetRootComponent());
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

		if (!KeyRingComponent->HasKey(RequiredKey))
		{
			SetKeyMessage();
			return;
		}
	}

	Open();
}

void ADoorBase::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                    AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex)
{
	if (IsStateCurrent(EInteractiveActorState::Closed))
	{
		if (bRequireKey)
		{
			SetMessagesHiddenInGame(true);
		}
		return;
	}

	Close();
}

void ADoorBase::SetMessagesTextAndColor(const FText Text, const FColor Color)
{
	for (const auto& Message : Messages)
	{
		Message->SetText(Text);
		Message->SetTextRenderColor(Color);
	}
}

void ADoorBase::SetMessagesHiddenInGame(const bool bIsHiddenInGame)
{
	for (const auto& Message : Messages)
	{
		Message->SetHiddenInGame(bIsHiddenInGame);
		Message->SetVisibility(!bIsHiddenInGame);
	}
}

void ADoorBase::SetKeyMessage()
{
	FString KeyColor = "";

	switch (RequiredKey)
	{
	case EKey::Blue:
		KeyColor = "blue";
		break;

	case EKey::Green:
		KeyColor = "green";
		break;

	case EKey::Yellow:
		KeyColor = "yellow";
		break;
	}
	const FString Text = FString::Printf(TEXT("Require %s key"), *KeyColor);
	SetMessagesTextAndColor(UKismetTextLibrary::Conv_StringToText(Text), MessageColors[RequiredKey]);
	SetMessagesHiddenInGame(false);
}
