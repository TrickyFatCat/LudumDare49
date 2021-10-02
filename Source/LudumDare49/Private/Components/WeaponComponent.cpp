// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Components/WeaponComponent.h"
#include "Components/TimelineComponent.h"
#include "Characters/PlayerCharacter.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	EquipAnimationTimeline = CreateDefaultSubobject<UTimelineComponent>("EquipAnimationTimeline");
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	EquipAnimationTimeline->SetPlayRate(1.f / (EquipDuration * 0.5f));

	if (EquipAnimationCurve)
	{
		FOnTimelineFloat AnimationProgress;
		AnimationProgress.BindUFunction(this, FName("AnimateWeaponEquip"));
		EquipAnimationTimeline->AddInterpFloat(EquipAnimationCurve, AnimationProgress);

		FOnTimelineEvent AnimationFinished;
		AnimationFinished.BindUFunction(this, FName("OnEquipAnimationFinished"));
		EquipAnimationTimeline->SetTimelineFinishedFunc(AnimationFinished);
	}
}


void UWeaponComponent::TickComponent(float DeltaTime,
                                     ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::SpawnWeapons(USceneComponent* WeaponScene)
{
	if (!GetWorld() || !GetOwner()) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	if (!PlayerCharacter) return;

	for (auto WeaponClass : WeaponClasses)
	{
		AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);

		if (!Weapon) continue;

		Weapon->SetOwner(GetOwner());
		Weapons.Add(FWeaponInventoryData{Weapon, false});
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(WeaponScene, AttachmentTransformRules);
		Weapon->SetActorHiddenInGame(true);
		Weapon->OnMakeShot.AddUObject(this, &UWeaponComponent::OnWeaponMakeShot);
	}
	
	Weapons[CurrentWeaponIndex].bIsAvailable = true;
	EquipWeapon(CurrentWeaponIndex);
}

void UWeaponComponent::GetCurrentWeaponData(FWeaponData& WeaponData) const
{
	CurrentWeapon->GetWeaponData(WeaponData);
}

void UWeaponComponent::EquipNextWeapon()
{
	if (bIsEquipping) return;

	PreviousWeaponIndex = CurrentWeaponIndex;

	do
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	}
	while (!Weapons[CurrentWeaponIndex].bIsAvailable);

	if (PreviousWeaponIndex == CurrentWeaponIndex) return;

	CurrentWeapon->StopShooting();
	StartEquipAnimation();
}

void UWeaponComponent::EquipPreviousWeapon()
{
	if (bIsEquipping) return;

	PreviousWeaponIndex = CurrentWeaponIndex;

	do
	{
		CurrentWeaponIndex = CurrentWeaponIndex == 0 ? Weapons.Num() - 1 : CurrentWeaponIndex - 1;
	}
	while (!Weapons[CurrentWeaponIndex].bIsAvailable);

	if (PreviousWeaponIndex == CurrentWeaponIndex) return;

	CurrentWeapon->StopShooting();
	StartEquipAnimation();
}

void UWeaponComponent::StartShooting()
{
	if (!CanShoot()) return;

	CurrentWeapon->StartShooting();
	bIsShooting = true;
}

void UWeaponComponent::StopShooting()
{
	if (!CurrentWeapon) return;

	CurrentWeapon->StopShooting();
	bIsShooting = false;
}

bool UWeaponComponent::UnlockWeapon(TSubclassOf<AWeaponBase> WeaponClass)
{
	bool Result = false;

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		FWeaponInventoryData& InventoryData = Weapons[i];
		if (!InventoryData.Weapon->IsA(WeaponClass)) continue;

		if (InventoryData.bIsAvailable)
		{
			Result = false;
			break;
		};

		InventoryData.bIsAvailable = true;

		if (!bIsEquipping)
		{
			PreviousWeaponIndex = CurrentWeaponIndex;
			CurrentWeaponIndex = i;
			StartEquipAnimation();
		}

		Result = true;
		break;
	}

	return Result;
}

bool UWeaponComponent::RestoreAmmo(TSubclassOf<AWeaponBase> WeaponClass, const int32 Amount)
{
	bool Result = false;

	if (Amount <= 0) return Result;

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		const FWeaponInventoryData& InventoryData = Weapons[i];

		if (!InventoryData.Weapon->IsA(WeaponClass)) continue;

		if (InventoryData.Weapon->AmmoIsFull())
		{
			Result = false;
			break;
		}

		InventoryData.Weapon->IncreaseAmmo(Amount);
		OnWeaponAmmoRestored.Broadcast(InventoryData.Weapon);
		Result = true;
		break;
	}

	return Result;
}

void UWeaponComponent::EquipWeapon(const int32 WeaponIndex)
{
	if (!GetOwner() || WeaponIndex >= Weapons.Num() || Weapons.Num() == 0) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopShooting();
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	CurrentWeapon = Weapons[WeaponIndex].Weapon;
	CurrentWeapon->SetActorHiddenInGame(false);
	
	FWeaponData WeaponData;
	CurrentWeapon->GetWeaponData(WeaponData);
	OnWeaponEquipped.Broadcast(CurrentWeapon);
}

void UWeaponComponent::OnWeaponMakeShot()
{
	OnWeaponShot.Broadcast();
}

void UWeaponComponent::AnimateWeaponEquip(const float Value)
{
	EquipProgress = Value;

	FVector NewLocation = FVector::ZeroVector;
	FRotator NewRotation = FRotator::ZeroRotator;

	FVector TargetLocation = NewLocation;
	TargetLocation.Z = NewLocation.Z - LocationOffset;
	FRotator TargetRotation = NewRotation;
	TargetRotation.Roll = NewRotation.Roll - RotationOffset;

	NewLocation = NewLocation + TargetLocation * Value;
	NewRotation = NewRotation + TargetRotation * Value;
	
	CurrentWeapon->SetActorRelativeLocation(NewLocation);
	CurrentWeapon->SetActorRelativeRotation(NewRotation);
}

void UWeaponComponent::OnEquipAnimationFinished()
{
	FWeaponData WeaponData;
	CurrentWeapon->GetWeaponData(WeaponData);

	if (EquipProgress >= 1.f)
	{
		EquipWeapon(CurrentWeaponIndex);
		EquipAnimationTimeline->ReverseFromEnd();
		return;
	}

	bIsEquipping = false;

	if (bIsShooting)
	{
		StartShooting();
	}
}

void UWeaponComponent::StartEquipAnimation()
{
	bIsEquipping = true;

	if (EquipDuration <= 0.f)
	{
		EquipWeapon(CurrentWeaponIndex);
		bIsEquipping = false;
		return;
	}

	EquipAnimationTimeline->PlayFromStart();
}

void UWeaponComponent::GetCurrentWeaponAmmo(FWeaponAmmoData& AmmoData) const
{
	AmmoData = CurrentWeapon->GetAmmoData();
}

int32 UWeaponComponent::GetCurrentAmmo() const
{
	FWeaponAmmoData AmmoData;
	GetCurrentWeaponAmmo(AmmoData);
	return AmmoData.AmmoCurrent;
}
