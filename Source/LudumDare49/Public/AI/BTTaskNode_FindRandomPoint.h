// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FindRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UBTTaskNode_FindRandomPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_FindRandomPoint();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetLocationKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float Distance = 256.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float SearchRadius = 128.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float SearchConeHalfAngle = 90.f;
	
};
