// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/BTService_GetDistanceToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_GetDistanceToPlayer::UBTService_GetDistanceToPlayer()
{
	NodeName = "Get Distance To Player";
}

void UBTService_GetDistanceToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Owner = OwnerComp.GetOwner();

	if (Blackboard && Owner)
	{
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

		if (TargetActor)
		{
			const float Distance = (Owner->GetActorLocation() - TargetActor->GetActorLocation()).Size();
			Blackboard->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
