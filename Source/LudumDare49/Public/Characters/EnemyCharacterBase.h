// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "EnemyCharacterBase.generated.h"

class UBaseSphereTriggerComponent;
class UBehaviorTree;
class USoundCue;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Stun UMETA(Hidden),
	Attack,
	Death UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API AEnemyCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	void ForceAttack() const;

protected:
	virtual void OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy")
	int32 AttackDamage = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy")
	USoundCue* AttackSound = nullptr;
	
	UFUNCTION(BlueprintCallable)
	virtual void StartAttack();

	UFUNCTION(BlueprintCallable)
	virtual void FinishAttack();

private:
	const float DefaultLifeSpan = 5.f;

	UFUNCTION()
	void OnAnyDamage(AActor* DamageActor,
	                 float Damage,
	                 const UDamageType* DamageType,
	                 AController* InstigatedBy,
	                 AActor* DamageCauser);


	void ReportDamageEvent(const float Damage, const AController* InstigatedBy, const AActor* Causer) const;
	
	void AggroNeighbours();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy", meta=(AllowPrivateAccess="true", ClampMin="0"))
	float AggroRadius = 300.f;

	bool bIsDamaged = false;

	// Behaviour
public:
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree = nullptr;
};
