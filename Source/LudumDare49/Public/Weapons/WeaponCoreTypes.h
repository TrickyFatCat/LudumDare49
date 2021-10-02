// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "WeaponCoreTypes.generated.h"

class AWeaponBase;
class AProjectileBase;
class USoundCue;

DECLARE_MULTICAST_DELEGATE(FOnMakeShotSignature)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponShotSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipWeaponSignature, AWeaponBase*, Weapon);

UENUM(BlueprintType)
enum class EBulletType : uint8
{
	HitScan,
	Projectile
};

USTRUCT(BlueprintType)
struct FWeaponInventoryData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="Weapon")
	AWeaponBase* Weapon = nullptr;

	UPROPERTY(EditAnywhere, Category="Weapon")
	bool bIsAvailable = false;
};

USTRUCT(BlueprintType)
struct FRecoilData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil", meta=(ClampMin="0"))
	float RecoilLocationOffset = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil", meta=(ClampMin="0"))
	float RecoilRotationOffset = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil", meta=(ClampMin="0"))
	float RecoilSpeed = 30.f;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> DamageType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	int32 Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	float RateOfFire = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon", meta=(ClampMin="0", ClampMax="180"))
	float Spread = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon", meta=(ClampMin="0"))
	int32 ShotCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon", meta=(ClampMin="0"))
	float ReloadTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Bullets", meta=(ClampMin="1", ClampMax="20"))
	int32 BulletsInShot = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Bullets")
	EBulletType BulletType = EBulletType::HitScan;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Bullets",
		meta=(EditCondition="BulletType==EBulletType::HitScan"))
	float HitScanDistance = 10000.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Bullets",
		meta=(EditCondition="BulletType==EBulletType::HitScan"))
	float HitScanImpulse = 10000.f;
	
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Bullets",
		meta=(EditCondition="BulletType==EBulletType::Projectile"))
	TSubclassOf<AProjectileBase> ProjectileClass = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	bool bIsAmmoInfinite = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Ammo")
	int32 AmmoCurrent = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ammo")
	int32 AmmoMax = 300;
};

USTRUCT(BlueprintType)
struct FHitScanDecalData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decal")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decal")
	FVector Size = FVector(16.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decal")
	float LifeTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decal")
	float FadeOutDuration = 0.5f;
};

USTRUCT(BlueprintType)
struct FHitScanImpactData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="VFX")
	UParticleSystem* ImpactEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SFX")
	USoundCue* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="VFX")
	FHitScanDecalData DecalData;
};

USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Projectile")
	TSubclassOf<UDamageType> DamageType = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="Projectile", meta=(ClampMin="0"))
	int32 Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile")
	bool bIsBouncing = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile")
	bool bIsExplosive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile", meta=(EditCondition="bIsExplosive", ClampMin="0"))
	float ExplosionRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile", meta=(EditCondition="bIsExplosive"))
	bool bDealFullDamage = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile", meta=(EditCondition="bIsExplosive"))
	bool bDamageOwner = false;
};
