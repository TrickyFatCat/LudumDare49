// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckSight.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UBTService_CheckSight : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckSight();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector CanSeePlayerKey;
};
