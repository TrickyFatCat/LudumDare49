// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Weapons/WeaponBase.h"

#include "GameFramework/Character.h"
#include "Core/ProjectUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/ProjectileBase.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponRoot = CreateDefaultSubobject<USceneComponent>("WeaponRoot");
	SetRootComponent(WeaponRoot);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetRootComponent());

	WeaponMuzzle = CreateDefaultSubobject<USceneComponent>("WeaponMuzzle");
	WeaponMuzzle->SetupAttachment(WeaponMesh);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = WeaponData.RateOfFire <= 0.f ? 1.f : 1.f / WeaponData.RateOfFire;
	AmmoData.AmmoCurrent = AmmoData.AmmoMax;
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::GetWeaponData(FWeaponData& Data) const
{
	Data = WeaponData;
}

bool AWeaponBase::GetTraceData(FVector& TraceStart, FVector& TraceEnd, const bool bCalculateSpread) const
{
	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	if (!FProjectUtils::GetPlayerViewPoint(GetOwner(), ViewLocation, ViewRotation)) return false;

	// if (!FTrickyUtils::GetPlayerViewPoint(GetOwner(), ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	FVector TraceDirection = ViewRotation.Vector();

	if (WeaponData.Spread > 0.f && bCalculateSpread)
	{
		const float SpreadAngleRad = FMath::DegreesToRadians(WeaponData.Spread / 2);
		TraceDirection = FMath::VRandCone(TraceDirection, SpreadAngleRad);
	}

	TraceEnd = TraceStart + TraceDirection * WeaponData.HitScanDistance;
	return true;
}

void AWeaponBase::GetHitScanData(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	CollisionQueryParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(HitResult,
	                                     TraceStart,
	                                     TraceEnd,
	                                     ECollisionChannel::ECC_Visibility,
	                                     CollisionQueryParams);
}

AController* AWeaponBase::GetOwnerController() const
{
	APawn* Pawn = Cast<APawn>(GetOwner());

	return Pawn ? Pawn->GetController() : nullptr;
}

void AWeaponBase::EnableShooting()
{
	bCanShoot = true;
}

void AWeaponBase::ApplyDamage(const FHitResult HitResult, const FVector& Direction)
{
	AActor* TargetActor = HitResult.GetActor();

	if (!TargetActor) return;

	UGameplayStatics::ApplyPointDamage(TargetActor,
	                                   CalculateDamage(),
	                                   Direction,
	                                   HitResult,
	                                   GetOwnerController(),
	                                   this,
	                                   WeaponData.DamageType);

	UMeshComponent* MeshComponent = Cast<UMeshComponent>(HitResult.Component);

	if (MeshComponent && MeshComponent->IsSimulatingPhysics())
	{
		MeshComponent->AddImpulse(Direction * WeaponData.HitScanImpulse);
	}
}

void AWeaponBase::StartShooting()
{
	if (!CanShoot()) return;

	MakeShot();
	GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &AWeaponBase::MakeShot, TimeBetweenShots, true);
}

void AWeaponBase::StopShooting()
{
	if (!GetWorldTimerManager().IsTimerActive(ShootingTimerHandle)) return;

	const float RemainingTime = GetWorldTimerManager().GetTimerRemaining(ShootingTimerHandle);
	GetWorldTimerManager().ClearTimer(ShootingTimerHandle);

	if (!CanShoot()) return;
	bCanShoot = false;
	GetWorldTimerManager().SetTimer(ShootingCooldownHandle, this, &AWeaponBase::EnableShooting, RemainingTime, false);
}

void AWeaponBase::MakeShot()
{
	if (AmmoData.AmmoCurrent <= 0)
	{
		StopShooting();
		return;
	}

	FVector TraceStart, TraceEnd;
	FHitResult HitResult;

	for (int32 i = 1; i <= WeaponData.BulletsInShot; ++i)
	{
		if (!GetTraceData(TraceStart, TraceEnd, i != 1 || WeaponData.BulletsInShot == 1))
		{
			StopShooting();
			continue;
		}

		GetHitScanData(HitResult, TraceStart, TraceEnd);
		const FVector MuzzleLocation = WeaponMuzzle->GetComponentLocation();
		const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
		const FVector Direction = (EndPoint - MuzzleLocation).GetSafeNormal();

		if (WeaponData.BulletType == EBulletType::HitScan)
		{
			if (HitResult.bBlockingHit)
			{
				ApplyDamage(HitResult, Direction);
				// WeaponFXComponent->PlayImpactFX(HitResult);
			}
		}
		else
		{
			const FTransform SpawnTransform(FRotator::ZeroRotator, MuzzleLocation);
			AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(
				WeaponData.ProjectileClass,
				SpawnTransform);

			if (Projectile)
			{
				// Projectile->SetDirectionAndDamage(Direction, WeaponData.DamageType, CalculateDamage());
				// Projectile->SetOwner(GetOwner());
				// Projectile->FinishSpawning(SpawnTransform);
			}
		}

		OnBulletShot(HitResult, TraceStart, HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd);
	}

	OnWeaponShot();
	OnMakeShot.Broadcast();
	DecreaseAmmo(WeaponData.ShotCost);
}

void AWeaponBase::IncreaseAmmo(const int32 Amount)
{
	if (Amount <= 0 || AmmoData.AmmoCurrent >= AmmoData.AmmoMax) return;

	AmmoData.AmmoCurrent += Amount;
	AmmoData.AmmoCurrent = FMath::Min(AmmoData.AmmoCurrent, AmmoData.AmmoMax);
}

void AWeaponBase::DecreaseAmmo(const int32 Amount)
{
	if (Amount <= 0 || AmmoData.AmmoCurrent <= 0) return;

	AmmoData.AmmoCurrent -= Amount;
	AmmoData.AmmoCurrent = FMath::Max(AmmoData.AmmoCurrent, 0);
}
