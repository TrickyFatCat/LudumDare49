// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacterBase.h"
#include "EnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API AEnemyMelee : public AEnemyCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyMelee();
	

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UCapsuleComponent* DamageTrigger = nullptr;
	
	UFUNCTION()
	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                                   AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp,
	                                   int32 OtherBodyIndex,
	                                   bool bFromSweep,
	                                   const FHitResult& SweepResult);

	virtual void StartAttack() override;

	virtual void FinishAttack() override;
};
