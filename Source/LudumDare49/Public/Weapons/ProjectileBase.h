// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponCoreTypes.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UWeaponFxComponent;

UCLASS()
class LUDUMDARE49_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category="Projectile")
	void GetProjectileData(FProjectileData& Data) const;

	void SetDirectionAndDamage(const FVector& Direction, const TSubclassOf<UDamageType> DamageType, const int32 Damage);


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USphereComponent* ProjectileCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* ProjectileMesh = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UWeaponFxComponent* ProjectileFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	FProjectileData ProjectileData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile", meta=(AllowPrivateAccess="true"))
	float DefaultLifeSpan = 5.f;

	FVector ShotDirection = FVector::ZeroVector;

	UPROPERTY()
	TArray<AActor*> IgnoredActors;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent,
	                     AActor* OtherActor,
	                     UPrimitiveComponent* OtherComp,
	                     FVector NormalImpulse,
	                     const FHitResult& Hit);

	void DealRadialDamage();

	TSubclassOf<UDamageType> GetDamageType() const;

	AController* GetOwnerController() const;
};
