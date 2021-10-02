// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Components/KeyRingComponent.h"

UKeyRingComponent::UKeyRingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UKeyRingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UKeyRingComponent::AddKey(const EKey Key)
{
	if (HasKey(Key)) return;

	KeyRing[Key] = true;
	OnKeyUnlocked.Broadcast(Key);
}

bool UKeyRingComponent::HasKey(const EKey Key)
{
	return KeyRing[Key];
}


