// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Characters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(GetMesh());

	WeaponScene = CreateDefaultSubobject<USceneComponent>("WeaponScene");
	WeaponScene->SetupAttachment(PlayerCamera);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialWeaponRotation = WeaponScene->GetRelativeRotation();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ProcessSwayRotation(DeltaSeconds);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);

	// Aiming
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::SetVerticalSway);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::SetHorizontalSway);
}

void APlayerCharacter::MoveForward(const float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerCharacter::MoveRight(const float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void APlayerCharacter::SetHorizontalSway(const float AxisValue)
{
	FRotator FinalRotation = WeaponScene->GetRelativeRotation();
	FinalRotation.Yaw = FinalRotation.Yaw - AxisValue * SwayPower;
	WeaponScene->SetRelativeRotation(FinalRotation);
}

void APlayerCharacter::SetVerticalSway(const float AxisValue)
{
	FRotator FinalRotation = WeaponScene->GetRelativeRotation();
	FinalRotation.Roll = FinalRotation.Roll - AxisValue * SwayPower;
	WeaponScene->SetRelativeRotation(FinalRotation);
}

void APlayerCharacter::ProcessSwayRotation(const float DeltaTime) const
{
	const FRotator TargetRotation = FRotator(WeaponScene->GetRelativeRotation().Pitch,
	                                         InitialWeaponRotation.Yaw,
	                                         InitialWeaponRotation.Roll);
	const FRotator FinalRotation = FMath::RInterpTo(WeaponScene->GetRelativeRotation(),
	                                                TargetRotation,
	                                                DeltaTime,
	                                                SwaySpeed);
	WeaponScene->SetRelativeRotation(FinalRotation);
}
