// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PickupBase.h"
#include "Components/KeyRingComponent.h"
#include "PickupKey.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API APickupKey : public APickupBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup")
	EKey KeyColor = EKey::Blue;

	virtual bool ActivatePickup_Implementation(AActor* TargetActor) override;
};
