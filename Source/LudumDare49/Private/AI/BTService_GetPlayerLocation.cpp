// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/BTService_GetPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_GetPlayerLocation::UBTService_GetPlayerLocation()
{
	NodeName = "Get Player Location";
}

void UBTService_GetPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (Blackboard)
	{
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

		if (TargetActor)
		{
			Blackboard->SetValueAsVector(TargetActorLocationKey.SelectedKeyName, TargetActor->GetActorLocation());
		}
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
