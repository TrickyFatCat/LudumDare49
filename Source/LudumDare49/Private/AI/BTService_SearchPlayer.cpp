// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/BTService_SearchPlayer.h"
#include "AIController.h"
#include "AI/AIPerceptionBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SearchPlayer::UBTService_SearchPlayer()
{
	NodeName = "Search Player";
}

void UBTService_SearchPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (Blackboard)
	{
		const AAIController* AIController = OwnerComp.GetAIOwner();
		UAIPerceptionBase* PerceptionComponent = AIController->FindComponentByClass<UAIPerceptionBase>();

		if (PerceptionComponent)
		{
			AActor* Player = PerceptionComponent->GetPlayer();
			Blackboard->SetValueAsObject(TargetActorKey.SelectedKeyName, Player);
		}
	}
}
