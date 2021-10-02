// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PickupBase.h"
#include "PickupArmor.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API APickupArmor : public APickupBase
{
	GENERATED_BODY()
	
protected:
	virtual bool ActivatePickup_Implementation(AActor* TargetActor) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true"))
	int32 ArmorAmount = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup", meta=(AllowPrivateAccess="true"))
	bool bClampToMax = true;
};
