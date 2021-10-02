// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponCoreTypes.h"
#include "WeaponBase.generated.h"

class USoundCue;
class UWeaponFxComponent;

UCLASS()
class LUDUMDARE49_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	FOnMakeShotSignature OnMakeShot;

	UFUNCTION(BlueprintPure, Category="Weapon")
	void GetWeaponData(FWeaponData& Data) const;



private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* WeaponRoot = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* WeaponMuzzle = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	UWeaponFxComponent* WeaponFXComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	FWeaponData WeaponData;

	// Shooting
public:
	float GetTimeBetweenShots() const { return TimeBetweenShots; }
	
protected:
	FTimerHandle ShootingTimerHandle;

	FTimerHandle ShootingCooldownHandle;

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon")
	void OnWeaponShot();

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon")
	void OnBulletShot(const FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd, const bool bCalculateSpread = true) const;

	void GetHitScanData(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

	AController* GetOwnerController() const;

	int32 CalculateDamage() const { return FMath::Max(WeaponData.Damage / WeaponData.BulletsInShot, 1); }

private:
	float TimeBetweenShots = 1.f;

	bool bCanShoot = true;

	UFUNCTION()
	void EnableShooting();

	void ApplyDamage(const FHitResult HitResult, const FVector& Direction);

	// Control
public:
	void StartShooting();
	void StopShooting();

private:
	void MakeShot();

	// Ammo
public:
	bool CanShoot() const { return AmmoData.AmmoCurrent > 0 && bCanShoot; }

	bool AmmoIsFull() const { return AmmoData.AmmoCurrent >= AmmoData.AmmoMax; }

	UFUNCTION(BlueprintGetter, Category="Weapon")
	FWeaponAmmoData GetAmmoData() const { return AmmoData; }

	void IncreaseAmmo(const int32 Amount);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetAmmoData, Category="Weapon")
	FWeaponAmmoData AmmoData;

	void DecreaseAmmo(const int32 Amount);

	// Recoil
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	FRecoilData RecoilData;
	
	FRotator InitialRotation = FRotator::ZeroRotator;
	
	FVector InitialLocation = FVector::ZeroVector;

	void CalculateRecoil(const float DeltaTime);

	// Sounds
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon|Sound")
	USoundCue* ShotSound = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon|Sound")
	USoundCue* EmptySound = nullptr;
};
