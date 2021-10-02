// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;

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

};
