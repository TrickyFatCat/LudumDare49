// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Objects/EntityResource.h"
#include "DamageControllerComponent.generated.h"

class UPhysicalMaterial;

/**
 * A base class for damage calculations.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, float, DeltaHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeathSignature,
                                               AController*,
                                               DeathInstigator,
                                               AActor*,
                                               DeathCauser,
                                               const UDamageType*,
                                               DamageType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArmorChangedSignature, float, NewArmor, float, DeltaArmor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TRICKYPROTOTYPING_API UDamageControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageControllerComponent();

protected:
	virtual void BeginPlay() override;

	// General
public:
protected:
private:
	
	// Health
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealth() const { return HealthObject->GetValue(); }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const { return HealthObject->GetValueMax(); }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetNormalizedHealth() const { return HealthObject->GetNormalizedValue(); }

	UFUNCTION(BlueprintCallable, Category="Health")
	void DecreaseHealth(const float Amount);
	UFUNCTION(BlueprintCallable, Category="Health")
	void IncreaseHealth(const float Amount, const bool bClampToMax = true);
	UFUNCTION(BlueprintCallable, Category="Health")
	void DecreaseMaxHealth(const float Amount, const bool bClampCurrentHealth = true);
	UFUNCTION(BlueprintCallable, Category="Health")
	void IncreaseMaxHealth(const float Amount, const bool bClampCurrentHealth = true);
	void BroadcastOnHealthChanged(const float NewHealth, const float DeltaHealth);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DamageController", meta=(AllowPrivateAccess="true"))
	FResourceData HealthData;
	UPROPERTY()
	UEntityResource* HealthObject = nullptr;
private:

public:
	UPROPERTY(BlueprintAssignable)
	FOnArmorChangedSignature OnArmorChanged;

	UFUNCTION(BlueprintCallable, Category="Armor")
	float GetArmor() const { return ArmorObject->GetValue(); }

	UFUNCTION(BlueprintCallable, Category="Armor")
	float GetMaxArmor() const { return ArmorObject->GetValueMax(); }

	UFUNCTION(BlueprintCallable, Category="Armor")
	float GetNormalizedArmor() const { return ArmorObject->GetNormalizedValue(); }

	UFUNCTION(BlueprintCallable, Category="Armor")
	void DecreaseArmor(const float Amount, AController* Instigator);

	UFUNCTION(BlueprintCallable, Category="Armor")
	void IncreaseArmor(const float Amount, const bool bClampToMax = true);

	UFUNCTION(BlueprintCallable, Category="Armor")
	void DecreaseMaxArmor(const float Amount, const bool bClampCurrentArmor = true);

	UFUNCTION(BlueprintCallable, Category="Armor")
	void IncreaseMaxArmor(const float Amount, const bool bClampCurrentArmor = true);

	void BroadcastOnArmorChanged(const float NewArmor, const float DeltaArmor);

	UFUNCTION(BlueprintGetter, Category="Armor")
	float GetArmorModifier() const { return ArmorModifier; }

	UFUNCTION(BlueprintSetter, Category="Armor")
	void SetArmorModifier(const float Value);

protected:
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetArmorModifier,
		BlueprintSetter=SetArmorModifier,
		meta=(AllowPrivateAccess="true",
		ClampMin="0"))
	float ArmorModifier = 0.75;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DamageController", meta=(AllowPrivateAccess="true"))
	FResourceData ArmorData;

	UPROPERTY()
	UEntityResource* ArmorObject = nullptr;
	
	// Damage calculation
public:
	UPROPERTY()
	FOnDeathSignature OnDeath;

	UFUNCTION(BlueprintPure, Category="Damage")
	bool GetIsDead() const { return GetHealth() <= 0.f; }
	UFUNCTION(BlueprintCallable, Category="Damage")
	float GetGeneralDamageModifier() const { return GeneralDamageModifier; }

	UFUNCTION(BlueprintCallable, Category="Dmage")
	void SetGeneralDamageModifier(const float NewModifier);
protected:
	/** Is used for any damage. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Damage")
	float GeneralDamageModifier = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	bool bUsePointDamageModifier = false;
	/** Is used only for point damage. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage", meta=(EditCondition="bUsePointDamageModifier"))
	TMap<UPhysicalMaterial*, float> PointDamageModifiers;
	float GetPointDamageModifier(AActor* Actor, const FName& BoneName);
	virtual void CalculateDamage(const float Damage,
	                             AActor* DamagedActor,
	                             AController* Instigator,
	                             AActor* Causer,
	                             const UDamageType* DamageType);
	UFUNCTION()
	virtual void OnTakeAnyDamage(AActor* DamageActor,
	                             float Damage,
	                             const UDamageType* DamageType,
	                             AController* InstigatedBy,
	                             AActor* DamageCauser);


	UFUNCTION()
	virtual void OnTakePointDamage(AActor* DamagedActor,
	                               float Damage,
	                               class AController* InstigatedBy,
	                               FVector HitLocation,
	                               class UPrimitiveComponent* FHitComponent,
	                               FName BoneName,
	                               FVector ShotFromDirection,
	                               const class UDamageType* DamageType,
	                               AActor* DamageCauser);

	UFUNCTION()
	virtual void OnTakeRadialDamage(AActor* DamagedActor,
	                                float Damage,
	                                const class UDamageType* DamageType,
	                                FVector Origin,
	                                FHitResult HitInfo,
	                                class AController* InstigatedBy,
	                                AActor* DamageCauser);
private:
};
