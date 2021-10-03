// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetDistanceToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UBTService_GetDistanceToPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_GetDistanceToPlayer();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector DistanceKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
