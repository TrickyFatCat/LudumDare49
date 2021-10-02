// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Pickups/PickupArmor.h"
#include "Components/DamageControllerComponent.h"

bool APickupArmor::ActivatePickup_Implementation(AActor* TargetActor)
{
	
	if (!TargetActor) return false;

	UDamageControllerComponent* DamageController = TargetActor->FindComponentByClass<UDamageControllerComponent>();

	if (!DamageController) return false;

	if (DamageController->GetNormalizedArmor() >= 1.f || DamageController->GetIsDead()) return false;

	DamageController->IncreaseArmor(ArmorAmount, bClampToMax);
	
	return true;
}
