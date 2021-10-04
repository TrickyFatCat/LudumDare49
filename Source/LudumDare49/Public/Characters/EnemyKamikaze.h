// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacterBase.h"
#include "EnemyKamikaze.generated.h"

class UWeaponFxComponent;

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API AEnemyKamikaze : public AEnemyCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyKamikaze();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Component")
	UWeaponFxComponent* WeaponFxComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy")
	float ExplosionRadius = 300.f;
	
	virtual void StartAttack() override;

	virtual void FinishAttack() override;

	void SpawnExplosion();
};
