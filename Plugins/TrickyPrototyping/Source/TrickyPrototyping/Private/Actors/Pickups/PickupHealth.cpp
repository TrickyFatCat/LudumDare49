// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/Pickups/PickupHealth.h"

#include "Components/DamageControllerComponent.h"

bool APickupHealth::ActivatePickup_Implementation(AActor* TargetActor)
{
	if (!TargetActor) return false;

	UDamageControllerComponent* DamageController = TargetActor->FindComponentByClass<UDamageControllerComponent>();

	if (!DamageController || DamageController->GetIsDead()) return false;

	if (DamageController->GetNormalizedHealth() >= 1.f && bClampHealToMaxHealth) return false;

	DamageController->IncreaseHealth(HealPower, bClampHealToMaxHealth);
	
	return true;
}
