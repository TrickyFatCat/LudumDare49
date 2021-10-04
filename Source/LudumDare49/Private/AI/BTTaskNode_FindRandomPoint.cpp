// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/BTTaskNode_FindRandomPoint.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTTaskNode_FindRandomPoint::UBTTaskNode_FindRandomPoint()
{
}

EBTNodeResult::Type UBTTaskNode_FindRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComponent) return EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();

	if (!Pawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Pawn);

	if (!NavSystem) return EBTNodeResult::Failed;

	FNavLocation TargetLocation;

	// FNavLocation TargetNavLocation;
	bool bLocationFound = false;

	const FRotator SearchCone = FRotator(0.f,
	                                     FMath::FRandRange(-SearchConeHalfAngle, SearchConeHalfAngle),
	                                     0.f);
	const FVector Direction = SearchCone.RotateVector(Pawn->GetActorRightVector() * (FMath::RandBool() ? 1 : -1));
	const FVector Point = Pawn->GetActorLocation() + Direction * Distance;
	bLocationFound = NavSystem->GetRandomPointInNavigableRadius(Point, SearchRadius, TargetLocation);

	// DrawDebugSphere(GetWorld(), Point, SearchRadius, 32, FColor::Red, false, 5, 0, 3);
	if (!bLocationFound) return EBTNodeResult::Failed;

	BlackboardComponent->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);
	return EBTNodeResult::Succeeded;
}
