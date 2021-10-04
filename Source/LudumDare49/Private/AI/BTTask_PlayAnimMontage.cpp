// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/BTTask_PlayAnimMontage.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

UBTTask_PlayAnimMontage::UBTTask_PlayAnimMontage()
{
	NodeName = "Play Animation Montage";
	// instantiating to be able to use Timers
	bCreateNodeInstance = true;

	TimerDelegate = FTimerDelegate::CreateUObject(this, &UBTTask_PlayAnimMontage::OnAnimationTimerDone);
}

EBTNodeResult::Type UBTTask_PlayAnimMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const float DiceRoll = FMath::FRand();

	if (DiceRoll > Chance) return EBTNodeResult::Succeeded;

	AAIController* const MyController = OwnerComp.GetAIOwner();

	// reset timer handle
	TimerHandle.Invalidate();
	MyOwnerComp = &OwnerComp;

	if (!MontageToPlay && !MyController && !MyController->GetPawn()) return EBTNodeResult::Failed;

	ACharacter* const MyCharacter = Cast<ACharacter>(MyController->GetPawn());
	PlayRate += FMath::FRandRange(-RandomDeviation, RandomDeviation);

	int32 SectionIndex = 0;

	if (bPlayRandomSection)
	{
		SectionIndex = FMath::RandRange(0, SectionsNumber - 1);
		StartSectionName = MontageToPlay->GetSectionName(SectionIndex);
	}
	else
	{
		SectionIndex = StartSectionName == NAME_None ? 0 : MontageToPlay->GetSectionIndex(StartSectionName);
	}

	MyCharacter->PlayAnimMontage(MontageToPlay, PlayRate, StartSectionName);
	const float FinishDelay = MontageToPlay->GetSectionLength(SectionIndex) / (PlayRate <= 0.f ? 1.f : PlayRate);

	if (FinishDelay > 0)
	{
		MyController->GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		                                                     TimerDelegate,
		                                                     FinishDelay,
		                                                     /*bLoop=*/
		                                                     false);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_PlayAnimMontage::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const MyController = OwnerComp.GetAIOwner();

	if (MontageToPlay && MyController && TimerHandle.IsValid())
	{
		MyController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	TimerHandle.Invalidate();

	return EBTNodeResult::Aborted;
}

void UBTTask_PlayAnimMontage::OnAnimationTimerDone()
{
	if (MyOwnerComp)
	{
		FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
	}
}
