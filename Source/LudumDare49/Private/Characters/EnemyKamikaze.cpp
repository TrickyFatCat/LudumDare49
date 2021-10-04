// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Characters/EnemyKamikaze.h"

#include "Components/DamageControllerComponent.h"
#include "Weapons/WeaponFxComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyKamikaze::AEnemyKamikaze()
{
	WeaponFxComponent = CreateDefaultSubobject<UWeaponFxComponent>("WeaponFxComponent");
}

void AEnemyKamikaze::StartAttack()
{
	Super::StartAttack();

	SpawnExplosion();

	if (DamageController->GetIsDead()) return;
	
	Destroy();
}

void AEnemyKamikaze::FinishAttack()
{
	Super::FinishAttack();
}

void AEnemyKamikaze::SpawnExplosion()
{
	if (!GetWorld()) return;
	
	UGameplayStatics::ApplyRadialDamage(GetWorld(),
	                                    AttackDamage,
	                                    GetActorLocation(),
	                                    ExplosionRadius,
	                                    nullptr,
	                                    {this},
	                                    this,
	                                    GetController(),
	                                    true,
	                                    ECC_GameTraceChannel1);

	FHitResult HitResult;
	HitResult.ImpactPoint = GetActorLocation();
	HitResult.ImpactNormal = GetActorUpVector();
	WeaponFxComponent->PlayImpactFX(HitResult);

	if (DamageController->GetIsDead())
	{
		SetLifeSpan(0.1f);
	}
}
