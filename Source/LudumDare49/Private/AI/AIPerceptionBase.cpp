// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/AIPerceptionBase.h"
#include "AIController.h"
#include "Characters/PlayerCharacter.h"
#include "Components/DamageControllerComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

AActor* UAIPerceptionBase::GetPlayer() const
{
	TArray<AActor*> PerceivedActors;
	GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PerceivedActors);

	if (PerceivedActors.Num() == 0)
	{
		GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

		if (PerceivedActors.Num() == 0) return nullptr;
	}

	AAIController* AIController = Cast<AAIController>(GetOwner());

	if (!AIController) return nullptr;

	APawn* Pawn = AIController->GetPawn();

	if (!Pawn) return nullptr;

	AActor* Player = nullptr;

	for (AActor* PerceivedActor : PerceivedActors)
	{
		if (!PerceivedActor->IsA(APlayerCharacter::StaticClass())) continue;

		UDamageControllerComponent* DamageController = PerceivedActor->FindComponentByClass<
			UDamageControllerComponent>();

		if (DamageController && !DamageController->GetIsDead())
		{
			Player = PerceivedActor;
			break;
		}
	}
	return Player;
}

bool UAIPerceptionBase::CanSeePlayer() const
{
	bool Result = false;
	TArray<AActor*> PerceivedActors;

	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

	if (PerceivedActors.Num() == 0) return Result;

	AAIController* AIController = Cast<AAIController>(GetOwner());

	if (!AIController) return Result;

	APawn* Pawn = AIController->GetPawn();

	if (!Pawn) return nullptr;

	for (AActor* PerceivedActor : PerceivedActors)
	{
		if (!PerceivedActor->IsA(APlayerCharacter::StaticClass())) continue;

		UDamageControllerComponent* DamageController = PerceivedActor->FindComponentByClass<
			UDamageControllerComponent>();

		if (DamageController && !DamageController->GetIsDead())
		{
			Result = true;
			break;
		}
	}
	return Result;
}
