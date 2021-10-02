// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Weapons/WeaponBase.h"
#include "Components/KeyRingComponent.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UWeaponComponent;
class UInteractionQueueComponent;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UCameraComponent* PlayerCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USceneComponent* WeaponScene = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UWeaponComponent* WeaponComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UKeyRingComponent* KeyRingComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UInteractionQueueComponent* InteractionQueue = nullptr;

	// Movement
protected:
	void MoveForward(const float AxisValue);

	void MoveRight(const float AxisValue);

	// Weapon sway

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AllowPrivateAccess="true"))
	float SwayPower = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AllowPrivateAccess="true"))
	float SwaySpeed = 75.f;

	UPROPERTY(VisibleInstanceOnly)
	FRotator InitialWeaponRotation = FRotator::ZeroRotator;

	void SetHorizontalSway(const float AxisValue);

	void SetVerticalSway(const float AxisValue);

	void ProcessSwayRotation(const float DeltaTime) const;

	// Death
protected:
	virtual void OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType) override;

	// Interaction
private:
	void StartInteraction();

	// Interface
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* ArmorIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* HealthIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* WeaponIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UWidgetComponent* ArmorWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UWidgetComponent* HealthWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UWidgetComponent* WeaponWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* KeyBlue = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* KeyGreen = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* KeyYellow = nullptr;
	
	UFUNCTION()
	void UpdateArmorCount(float Armor, float DeltaArmor);

	UFUNCTION()
	void UpdateHealthCount(float Health, float DeltaHealth);

	UFUNCTION()
	void UpdateWeaponCount();

	UFUNCTION()
	void OnWeaponEquipped(AWeaponBase* NewWeapon);

	UFUNCTION()
	void OnKeyUnlocked(EKey NewKey);

	UFUNCTION()
	void OnWeaponAmmoRestored(AWeaponBase* Weapon);
};
