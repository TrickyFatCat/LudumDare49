// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SearchPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UBTService_SearchPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_SearchPlayer();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
