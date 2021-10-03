// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "EnemyCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API AEnemyCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void AttackPlayer();

private:
	UFUNCTION()
	void OnAnyDamage(AActor* DamageActor,
	                 float Damage,
	                 const UDamageType* DamageType,
	                 AController* InstigatedBy,
	                 AActor* DamageCauser);

	
	void ReportDamageEvent(const float Damage, const AController* InstigatedBy, const AActor* Causer) const;
};
