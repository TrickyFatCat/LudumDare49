// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Pickups/PickupKey.h"
#include "Components/KeyRingComponent.h"

bool APickupKey::ActivatePickup_Implementation(AActor* TargetActor)
{
	if (!IsValid(TargetActor)) return false;

	UKeyRingComponent* KeyRingComponent = TargetActor->FindComponentByClass<UKeyRingComponent>();

	if (!KeyRingComponent) return false;

	if (KeyRingComponent->HasKey(KeyColor)) return false;

	KeyRingComponent->AddKey(KeyColor);

	return true;
}
