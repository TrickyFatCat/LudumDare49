// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Pickups/PickupAmmo.h"
#include "Components/WeaponComponent.h"

APickupAmmo::APickupAmmo()
{
}

bool APickupAmmo::ActivatePickup_Implementation(AActor* TargetActor)
{
	if (!IsValid(TargetActor)) return false;

	UWeaponComponent* WeaponComponent = TargetActor->FindComponentByClass<UWeaponComponent>();

	if (!WeaponComponent) return false;

	return WeaponComponent->RestoreAmmo(WeaponClass, AmmoAmount);
}
