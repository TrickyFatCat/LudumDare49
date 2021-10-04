// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsNearPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UBTDecorator_IsNearPlayer : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsNearPlayer();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category="Node")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category="Node")
	FBlackboardKeySelector DistanceToActorKey;

	UPROPERTY(EditAnywhere, Category="Node")
	float TargetDistance = 200.f;
};
