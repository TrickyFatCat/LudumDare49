// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Pickups/PickupWeapon.h"
#include "Components/WeaponComponent.h"

APickupWeapon::APickupWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(MeshScene);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool APickupWeapon::ActivatePickup_Implementation(AActor* TargetActor)
{
	if (!IsValid(TargetActor)) return false;

	UWeaponComponent* WeaponComponent =	TargetActor->FindComponentByClass<UWeaponComponent>();

	if (!WeaponComponent) return false;

	return WeaponComponent->UnlockWeapon(WeaponClass);
}
