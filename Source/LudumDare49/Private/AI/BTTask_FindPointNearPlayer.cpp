// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/BTTask_FindPointNearPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPointNearPlayer::UBTTask_FindPointNearPlayer()
{
	NodeName = "Get Point Near Player";
}

EBTNodeResult::Type UBTTask_FindPointNearPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Owner = OwnerComp.GetOwner();

	if (!Blackboard && !Owner) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!TargetActor) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Owner);

	if (!NavSystem) return EBTNodeResult::Failed;

	FNavLocation TargetNavLocation;
	bool bLocationFound = false;

	const FRotator SearchCone = FRotator(0.f,
	                                     FMath::FRandRange(-SearchConeHalfAngle, SearchConeHalfAngle),
	                                     0.f);
	const FVector DirectionToOwner = SearchCone.RotateVector(
		                                           Owner->GetActorLocation() - TargetActor->
		                                           GetTargetLocation())
	                                           .GetSafeNormal();
	const FVector Point = TargetActor->GetActorLocation() + DirectionToOwner * FMath::FRandRange(
		DistanceMin,
		DistanceMax);
	bLocationFound = NavSystem->GetRandomPointInNavigableRadius(Point, SearchRadius, TargetNavLocation);

	if (!bLocationFound) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetNavLocation);
	return EBTNodeResult::Succeeded;
}
