// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/BTDecorator_IsNearPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsNearPlayer::UBTDecorator_IsNearPlayer()
{
	NodeName = "Is Near Player";
}

bool UBTDecorator_IsNearPlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Blackboard) return false;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!TargetActor) return false;

	const float CurrentDistance = Blackboard->GetValueAsFloat(DistanceToActorKey.SelectedKeyName);
	return CurrentDistance <= TargetDistance;
}
