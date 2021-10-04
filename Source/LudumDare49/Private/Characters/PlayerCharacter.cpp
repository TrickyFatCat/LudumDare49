// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Characters/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/DamageControllerComponent.h"
#include "Components/WeaponComponent.h"
#include "Core/Session/SessionGameMode.h"
#include "Components/KeyRingComponent.h"
#include "Components/InteractionQueueComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/CounterUserWidget.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(GetMesh());

	WeaponSway = CreateDefaultSubobject<USpringArmComponent>("WeaponSway");
	WeaponSway->SetupAttachment(PlayerCamera);

	WeaponScene = CreateDefaultSubobject<USceneComponent>("WeaponScene");
	WeaponScene->SetupAttachment(WeaponSway);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	KeyRingComponent = CreateDefaultSubobject<UKeyRingComponent>("KeyRingComponent");
	InteractionQueue = CreateDefaultSubobject<UInteractionQueueComponent>("InteractionQueue");

	ArmorIcon = CreateDefaultSubobject<UStaticMeshComponent>("ArmorIcon");
	ArmorIcon->SetupAttachment(PlayerCamera);

	HealthIcon = CreateDefaultSubobject<UStaticMeshComponent>("HealthIcon");
	HealthIcon->SetupAttachment(PlayerCamera);

	WeaponIcon = CreateDefaultSubobject<UStaticMeshComponent>("WeaponIcon");
	WeaponIcon->SetupAttachment(PlayerCamera);

	KeyBlue = CreateDefaultSubobject<UStaticMeshComponent>("KeyBlue");
	KeyBlue->SetupAttachment(PlayerCamera);
	KeyBlue->SetHiddenInGame(true);
	
	KeyGreen = CreateDefaultSubobject<UStaticMeshComponent>("KeyGreen");
	KeyGreen->SetupAttachment(PlayerCamera);
	KeyGreen->SetHiddenInGame(true);
	
	KeyYellow = CreateDefaultSubobject<UStaticMeshComponent>("KeyYellow");
	KeyYellow->SetupAttachment(PlayerCamera);
	KeyYellow->SetHiddenInGame(true);

	ArmorWidget = CreateDefaultSubobject<UWidgetComponent>("ArmorWidget");
	ArmorWidget->SetupAttachment(ArmorIcon);
	ArmorWidget->SetWidgetSpace(EWidgetSpace::Screen);
	ArmorWidget->SetDrawAtDesiredSize(true);

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>("HealthWidget");
	HealthWidget->SetupAttachment(HealthIcon);
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidget->SetDrawAtDesiredSize(true);

	WeaponWidget = CreateDefaultSubobject<UWidgetComponent>("WeaponWidget");
	WeaponWidget->SetupAttachment(WeaponIcon);
	WeaponWidget->SetWidgetSpace(EWidgetSpace::Screen);
	WeaponWidget->SetDrawAtDesiredSize(true);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialWeaponRotation = WeaponScene->GetRelativeRotation();
	WeaponComponent->SpawnWeapons(WeaponScene);

	DamageController->OnHealthChanged.AddDynamic(this, &APlayerCharacter::UpdateHealthCount);
	DamageController->OnArmorChanged.AddDynamic(this, &APlayerCharacter::UpdateArmorCount);

	WeaponComponent->OnWeaponShot.AddDynamic(this, &APlayerCharacter::UpdateWeaponCount);
	WeaponComponent->OnWeaponEquipped.AddDynamic(this, &APlayerCharacter::OnWeaponEquipped);
	WeaponComponent->OnWeaponAmmoRestored.AddDynamic(this, &APlayerCharacter::OnWeaponAmmoRestored);

	KeyRingComponent->OnKeyUnlocked.AddDynamic(this, &APlayerCharacter::OnKeyUnlocked);

	UpdateArmorCount(DamageController->GetArmor(), 0);
	UpdateHealthCount(DamageController->GetHealth(), 0);
	UpdateWeaponCount();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// ProcessSwayRotation(DeltaSeconds);
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
	// PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::SetVerticalSway);
	// PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::SetHorizontalSway);

	// Weapon
	PlayerInputComponent->BindAction("EquipNextWeapon",
	                                 IE_Pressed,
	                                 WeaponComponent,
	                                 &UWeaponComponent::EquipNextWeapon);
	PlayerInputComponent->BindAction("EquipPreviousWeapon",
	                                 IE_Pressed,
	                                 WeaponComponent,
	                                 &UWeaponComponent::EquipPreviousWeapon);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, WeaponComponent, &UWeaponComponent::StartShooting);
	PlayerInputComponent->BindAction("Shoot", IE_Released, WeaponComponent, &UWeaponComponent::StopShooting);

	// Interaction
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::StartInteraction);
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

void APlayerCharacter::OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* Damage)
{
	Super::OnDeath(DeathInstigator, DeathCauser, Damage);
	if (GetWorld())
	{
		ASessionGameMode* SessionGameMode = Cast<ASessionGameMode>(GetWorld()->GetAuthGameMode());

		if (SessionGameMode)
		{
			SessionGameMode->FinishSession();
		}
	}
	WeaponComponent->StopShooting();
}

void APlayerCharacter::StartInteraction()
{
	InteractionQueue->Interact();
}

void APlayerCharacter::UpdateArmorCount(float Armor, float DeltaArmor)
{
	UCounterUserWidget* Counter = Cast<UCounterUserWidget>(ArmorWidget->GetUserWidgetObject());

	if (!Counter) return;

	Counter->SetCounter(FMath::CeilToInt(Armor));
}

void APlayerCharacter::UpdateHealthCount(float Health, float DeltaHealth)
{
	UCounterUserWidget* Counter = Cast<UCounterUserWidget>(HealthWidget->GetUserWidgetObject());

	if (!Counter) return;

	Counter->SetCounter(FMath::CeilToInt(Health));
}

void APlayerCharacter::UpdateWeaponCount()
{
	UCounterUserWidget* Counter = Cast<UCounterUserWidget>(WeaponWidget->GetUserWidgetObject());

	if (!Counter) return;

	Counter->SetCounter(WeaponComponent->GetCurrentAmmo());
}

void APlayerCharacter::OnWeaponEquipped(AWeaponBase* NewWeapon)
{
	UpdateWeaponCount();
}

void APlayerCharacter::OnKeyUnlocked(EKey NewKey)
{
	switch (NewKey)
	{
	case (EKey::Blue):
		KeyBlue->SetHiddenInGame(false);
		break;
	case (EKey::Green):
		KeyGreen->SetHiddenInGame(false);
		break;
	case (EKey::Yellow):
		KeyYellow->SetHiddenInGame(false);
		break;
	}
}

void APlayerCharacter::OnWeaponAmmoRestored(AWeaponBase* Weapon)
{
	if (!Weapon->IsA(WeaponComponent->GetCurrentWeapon()->GetClass())) return;

	UpdateWeaponCount();
}
