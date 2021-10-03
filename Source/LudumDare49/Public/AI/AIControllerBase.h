// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/AIPerceptionBase.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	// Behaviour tree
public:
	AActor* GetTargetActor() const;

	void SetTargetActor(AActor* Value);

	void SetIsAttacking(bool Value);
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BehaviorTree", meta=(AllowPrivateAccess="true"))
	FName TargetActorKeyName = "TargetActor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BehaviorTree", meta=(AllowPrivateAccess="true"))
	FName InitialLocationKeyName = "InitialLocation";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BehaviorTree", meta=(AllowPrivateAccess="true"))
	FName IsAttackingKeyName = "IsAttacking";

	// Perception
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UAIPerceptionBase* Perception = nullptr;
};
