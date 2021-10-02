// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponCoreTypes.h"
#include "Weapons/WeaponBase.h"
#include "WeaponComponent.generated.h"

class UTimelineComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LUDUMDARE49_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime,
	                           ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnWeapons(USceneComponent* WeaponScene);

	UFUNCTION(BlueprintGetter, Category="Weapon")
	AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

	UFUNCTION(BlueprintPure, Category="Weapon")
	void GetCurrentWeaponData(FWeaponData& WeaponData) const;


private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<AWeaponBase>> WeaponClasses;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	TArray<FWeaponInventoryData> Weapons;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentWeapon)
	AWeaponBase* CurrentWeapon = nullptr;

	UPROPERTY(VisibleInstanceOnly)
	int32 CurrentWeaponIndex = 0;

	UPROPERTY(VisibleInstanceOnly)
	int32 PreviousWeaponIndex = 0;

	bool bWeaponsSpawned = false;

	// Weapon control
public:
	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FOnWeaponShotSignature OnWeaponShot;

	void EquipNextWeapon();

	void EquipPreviousWeapon();

	void StartShooting();

	void StopShooting();

	bool UnlockWeapon(TSubclassOf<AWeaponBase> WeaponClass);

	bool RestoreAmmo(TSubclassOf<AWeaponBase> WeaponClass, const int32 Amount);

	UFUNCTION(BlueprintPure, Category="Weapon")
	bool GetIsShooting() const { return CanShoot() && bIsShooting; }

protected:
	bool bIsShooting;

	FTimerHandle ReloadTimerHandle;

	void EquipWeapon(const int32 WeaponIndex);

private:
	const float WallCheckDistance = 80.f;

	void OnWeaponMakeShot();

	// Equip animation
public:
	UPROPERTY(BlueprintAssignable)
	FOnEquipWeaponSignature OnWeaponEquipped;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation|Equip", meta=(AllowPrivateAccess="true"))
	UCurveFloat* EquipAnimationCurve = nullptr;

	UPROPERTY()
	UTimelineComponent* EquipAnimationTimeline = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Animation|Equip", meta=(AllowPrivateAccess="true"))
	float EquipProgress = 0.f;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Animation|Equip",
		meta=(AllowPrivateAccess="true", ClampMin="0"))
	float EquipDuration = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category="Animation|Equip")
	float RotationOffset = 70;

	UPROPERTY(EditDefaultsOnly, Category="Animation|Equip")
	float LocationOffset = 70;

	UFUNCTION()
	void AnimateWeaponEquip(const float Value);

	UFUNCTION()
	void OnEquipAnimationFinished();

	void StartEquipAnimation();

	// Weapon parameters
public:
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool CanShoot() const
	{
		return CurrentWeapon->CanShoot() && !bIsEquipping;
	}

	UFUNCTION(BlueprintGetter, Category="Weapon")
	bool GetIsEquipping() const { return bIsEquipping; }

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void GetCurrentWeaponAmmo(FWeaponAmmoData& AmmoData) const;

	UFUNCTION()
	int32 GetCurrentAmmo() const;

protected:
private:
	UPROPERTY(BlueprintGetter=GetIsEquipping, Category="Weapon")
	bool bIsEquipping = false;
};
