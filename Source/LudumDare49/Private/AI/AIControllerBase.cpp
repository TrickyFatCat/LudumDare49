// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/AIControllerBase.h"
#include "AI/AIPerceptionBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EnemyCharacterBase.h"

AAIControllerBase::AAIControllerBase()
{
	Perception = CreateDefaultSubobject<UAIPerceptionBase>("PerceptionComponent");
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacterBase* AICharacter = Cast<AEnemyCharacterBase>(InPawn);

	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->GetBehaviorTree());
	}

}

AActor* AAIControllerBase::GetTargetActor() const
{
	if (!GetBlackboardComponent()) return nullptr;

	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetActorKeyName));
}

void AAIControllerBase::SetTargetActor(AActor* Value)
{
	if (!GetBlackboardComponent()) return;

	GetBlackboardComponent()->SetValueAsObject(TargetActorKeyName, Value);	
}