// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Actors/ExplosiveBarrel.h"
#include "Weapons/WeaponFxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AExplosiveBarrel::AExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = true;
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
	SetRootComponent(BarrelMesh);

	ExplosionFXComponent = CreateDefaultSubobject<UWeaponFxComponent>("ExplosionFXComponent");
}

void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &AExplosiveBarrel::OnTakeDamage);
}

void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExplosiveBarrel::OnTakeDamage(AActor* DamagedActor,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (bIsExploded) return;

	ExplosionDelay = ExplosionDelay <= 0.f ? GetWorld()->GetTimeSeconds() : ExplosionDelay;
	GetWorldTimerManager().SetTimer(ExplosionDelayHandle,
	                                this,
	                                &AExplosiveBarrel::ProcessExplosion,
	                                ExplosionDelay,
	                                false);
	
	bIsExploded = true;
}

void AExplosiveBarrel::ProcessExplosion()
{
	UGameplayStatics::ApplyRadialDamage(GetWorld(),
	                                    ExplosionDamage,
	                                    GetActorLocation(),
	                                    ExplosionRadius,
	                                    DamageTypeClass,
	                                    {},
	                                    this,
	                                    nullptr,
	                                    bDoFullDamage,
	                                    ECC_GameTraceChannel1);

	FHitResult HitResult;
	HitResult.ImpactPoint = GetActorLocation();
	HitResult.ImpactNormal = GetActorUpVector();
	ExplosionFXComponent->PlayImpactFX(HitResult);
	Destroy();
}

