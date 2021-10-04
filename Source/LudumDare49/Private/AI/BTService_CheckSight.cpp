// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/BTService_CheckSight.h"
#include "AIController.h"
#include "AI/AIPerceptionBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckSight::UBTService_CheckSight()
{
	NodeName = "Check Sight";
}

void UBTService_CheckSight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (Blackboard)
	{
		const AAIController* AIController = OwnerComp.GetAIOwner();
		UAIPerceptionBase* PerceptionComponent = AIController->FindComponentByClass<UAIPerceptionBase>();

		if (PerceptionComponent)
		{
			Blackboard->SetValueAsBool(CanSeePlayerKey.SelectedKeyName, PerceptionComponent->CanSeePlayer());
		}
	}
}
