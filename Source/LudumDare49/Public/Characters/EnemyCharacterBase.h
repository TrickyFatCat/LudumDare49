// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "EnemyCharacterBase.generated.h"

class UBaseSphereTriggerComponent;
class UBehaviorTree;

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

protected:
	virtual void OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* DamageType) override;

	virtual void AttackPlayer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBaseSphereTriggerComponent* AggroRadius = nullptr;

private:
	const float DefaultLifeSpan = 5.f;

	UFUNCTION()
	void OnAnyDamage(AActor* DamageActor,
	                 float Damage,
	                 const UDamageType* DamageType,
	                 AController* InstigatedBy,
	                 AActor* DamageCauser);


	void ReportDamageEvent(const float Damage, const AController* InstigatedBy, const AActor* Causer) const;

	// Behaviour
public:
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	void SetState(const EEnemyState NewState);

	EEnemyState GetStateCurrent() const { return StateCurrent; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy|AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy", meta=(AllowPrivateAccess="true"))
	EEnemyState StateInitial = EEnemyState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy", meta=(AllowPrivateAccess="true"))
	EEnemyState StateCurrent = EEnemyState::Idle;

	
};
