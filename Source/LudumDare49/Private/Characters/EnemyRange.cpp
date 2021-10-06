// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Characters/EnemyRange.h"
#include "Components/ArrowComponent.h"
#include "Weapons/ProjectileBase.h"
#include "Weapons/WeaponCoreTypes.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AEnemyRange::AEnemyRange()
{
	ProjectileSpawn = CreateDefaultSubobject<UArrowComponent>("ProjectileSpawn");
	ProjectileSpawn->SetupAttachment(GetMesh());
}

void AEnemyRange::StartAttack()
{
	Super::StartAttack();
	
	SpawnProjectile();
}

void AEnemyRange::FinishAttack()
{
	Super::FinishAttack();
}

bool AEnemyRange::GetTraceData(FVector& TraceStart, FVector& TraceEnd, const bool bCalculateSpread)
{
	TraceStart = ProjectileSpawn->GetComponentLocation();
	const FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	FVector TraceDirection = (PlayerLocation - GetActorLocation()).GetSafeNormal();

	if (WeaponData.Spread > 0.f && bCalculateSpread)
	{
		const float SpreadAngleRad = FMath::DegreesToRadians(WeaponData.Spread / 2);
		TraceDirection = FMath::VRandCone(TraceDirection, SpreadAngleRad);
	}

	TraceEnd = TraceStart + TraceDirection * WeaponData.HitScanDistance;
	return true;
}

void AEnemyRange::GetHitScanData(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult,
	                                     TraceStart,
	                                     TraceEnd,
	                                     ECC_GameTraceChannel2,
	                                     CollisionQueryParams);
}

void AEnemyRange::SpawnProjectile()
{
	FVector TraceStart, TraceEnd;
	FHitResult HitResult;

	for (int32 i = 1; i <= WeaponData.BulletsInShot; ++i)
	{
		if (!GetTraceData(TraceStart, TraceEnd, i != 1 || WeaponData.BulletsInShot == 1))
		{
			FinishAttack();
			continue;
		}

		GetHitScanData(HitResult, TraceStart, TraceEnd);
		// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 5, 0, 3);
		const FVector MuzzleLocation = ProjectileSpawn->GetComponentLocation();
		const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
		const FVector Direction = (EndPoint - MuzzleLocation).GetSafeNormal();

		const FTransform SpawnTransform(FRotator::ZeroRotator, MuzzleLocation);
		AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(
			WeaponData.ProjectileClass,
			SpawnTransform);

		if (Projectile)
		{
			Projectile->SetDirectionAndDamage(Direction, WeaponData.DamageType, CalculateDamage());
			Projectile->SetOwner(GetOwner());
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
