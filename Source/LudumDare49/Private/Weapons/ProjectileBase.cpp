// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Weapons/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerCharacter.h"
#include "Weapons/WeaponFxComponent.h"


AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>("ProjectileCollision");
	SetRootComponent(ProjectileCollision);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileCollision->bReturnMaterialOnMove = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(GetRootComponent());

	ProjectileFX = CreateDefaultSubobject<UWeaponFxComponent>("ProjectileFX");
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement->Velocity = ShotDirection * ProjectileMovement->InitialSpeed;
	ProjectileCollision->IgnoreActorWhenMoving(GetOwner(), true);
	ProjectileCollision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnProjectileHit);
	SetLifeSpan(DefaultLifeSpan);

	if (ProjectileData.bIsExplosive && !ProjectileData.bDamageOwner)
	{
		IgnoredActors.Add(GetOwner());
	}

	if (ProjectileData.bIsBouncing)
	{
		ProjectileMovement->bShouldBounce = ProjectileData.bIsBouncing;
	}
}

void AProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason != EEndPlayReason::Destroyed) return;

	if (GetLifeSpan() <= 0.f && ProjectileData.bIsExplosive)
	{
		DealRadialDamage();
		FHitResult HitResult;
		HitResult.ImpactPoint = GetActorLocation();
		ProjectileFX->PlayImpactFX(HitResult);
	}
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::GetProjectileData(FProjectileData& Data) const
{
	Data = ProjectileData;
}

void AProjectileBase::SetDirectionAndDamage(const FVector& Direction,
                                            const TSubclassOf<UDamageType> DamageType,
                                            const int32 Damage)
{
	ShotDirection = Direction;

	if (Damage <= 0) return;

	ProjectileData.DamageType = DamageType;
	ProjectileData.Damage = Damage;
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent,
                                      AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      FVector NormalImpulse,
                                      const FHitResult& Hit)
{
	UWorld* World = GetWorld();

	if (!World) return;

	if (ProjectileData.bIsBouncing && !OtherActor->IsA(APawn::StaticClass())) return;

	ProjectileMovement->StopMovementImmediately();

	if (ProjectileData.bIsExplosive)
	{
		DealRadialDamage();
	}
	else
	{
		if (OtherActor->IsA(GetOwner()->GetClass())) return;
		UGameplayStatics::ApplyPointDamage(OtherActor,
		                                   ProjectileData.Damage,
		                                   Hit.Location,
		                                   Hit,
		                                   GetOwnerController(),
		                                   this,
		                                   GetDamageType());
	}

	ProjectileFX->PlayImpactFX(Hit);
	Destroy();
}

void AProjectileBase::DealRadialDamage()
{
	if (!GetWorld()) return;

	UGameplayStatics::ApplyRadialDamage(GetWorld(),
	                                    ProjectileData.Damage,
	                                    GetActorLocation(),
	                                    ProjectileData.ExplosionRadius,
	                                    GetDamageType(),
	                                    IgnoredActors,
	                                    this,
	                                    GetOwnerController(),
	                                    ProjectileData.bDealFullDamage,
	                                    ECC_GameTraceChannel1);
}

TSubclassOf<UDamageType> AProjectileBase::GetDamageType() const
{
	return nullptr;
}

AController* AProjectileBase::GetOwnerController() const
{
	APawn* Pawn = Cast<APawn>(GetOwner());

	return Pawn ? Pawn->GetController() : nullptr;
}
