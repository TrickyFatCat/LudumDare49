// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/BTService_FindPointNearPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTService_FindPointNearPlayer::UBTService_FindPointNearPlayer()
{
	NodeName = "Get Point Near Player";
}

void UBTService_FindPointNearPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Owner = OwnerComp.GetOwner();

	if (Blackboard && Owner)
	{
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Owner);
		if (TargetActor && NavSystem)
		{
			FNavLocation TargetNavLocation;
			bool bLocationFound = false;

			do
			{
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
				bLocationFound = NavSystem->GetRandomReachablePointInRadius(Point, SearchRadius, TargetNavLocation);
			}
			while (!bLocationFound);

			Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetNavLocation);
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
