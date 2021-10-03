// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacterBase.h"
#include "Weapons/WeaponCoreTypes.h"
#include "EnemyRange.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API AEnemyRange : public AEnemyCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyRange();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UArrowComponent* ProjectileSpawn = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy")
	FWeaponData WeaponData;

	virtual void StartAttack() override;
	virtual void FinishAttack() override;

	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd, const bool bCalculateSpread);
	
	void GetHitScanData(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	
	void SpawnProjectile();
	
	int32 CalculateDamage() const { return FMath::Max(WeaponData.Damage / WeaponData.BulletsInShot, 1); }
};
