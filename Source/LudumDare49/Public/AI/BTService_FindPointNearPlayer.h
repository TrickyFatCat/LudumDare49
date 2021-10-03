// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindPointNearPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UBTService_FindPointNearPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_FindPointNearPlayer();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	FBlackboardKeySelector TargetLocationKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float DistanceMin = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float DistanceMax = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float SearchRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node")
	float SearchConeHalfAngle = 90.f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
