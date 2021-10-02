// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PickupBase.h"
#include "PickupWeapon.generated.h"

class AWeaponBase;
/**
 * 
 */
UCLASS()
class LUDUMDARE49_API APickupWeapon : public APickupBase
{
	GENERATED_BODY()
	
public:
	APickupWeapon();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* WeaponMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AWeaponBase> WeaponClass;
	
	virtual bool ActivatePickup_Implementation(AActor* TargetActor) override;
};
