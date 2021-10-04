// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Characters/EnemyCharacterBase.h"

#include "BrainComponent.h"
#include "AI/AIControllerBase.h"
#include "Components/DamageControllerComponent.h"
#include "Perception/AISense_Damage.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIControllerBase::StaticClass();
	bUseControllerRotationYaw = false;
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &AEnemyCharacterBase::OnAnyDamage);
}

void AEnemyCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyCharacterBase::ForceAttack() const
{
	
	AAIControllerBase* AIController = Cast<AAIControllerBase>(GetController());

	if (!AIController) return;

	AIController->SetTargetActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AEnemyCharacterBase::OnDeath(AController* DeathInstigator, AActor* DeathCauser, const UDamageType* Damage)
{
	FinishAttack();
	Super::OnDeath(DeathInstigator, DeathCauser, Damage);
	SetLifeSpan(DefaultLifeSpan);
	AAIController* AIController = Cast<AAIController>(Controller);

	if (AIController && AIController->BrainComponent)
	{
		AIController->BrainComponent->Cleanup();
		AIController->StopMovement();
	}
}

void AEnemyCharacterBase::StartAttack()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound, GetActorLocation(), GetActorRotation());
}

void AEnemyCharacterBase::FinishAttack()
{
}

void AEnemyCharacterBase::OnAnyDamage(AActor* DamageActor,
                                      float Damage,
                                      const UDamageType* DamageType,
                                      AController* InstigatedBy,
                                      AActor* DamageCauser)
{
	if (DamageController->GetIsDead()) return;

	ReportDamageEvent(Damage, InstigatedBy, DamageCauser);

	if (bIsDamaged) return;

	AggroNeighbours();

	bIsDamaged = true;
}

void AEnemyCharacterBase::ReportDamageEvent(const float Damage,
                                            const AController* InstigatedBy,
                                            const AActor* Causer) const
{
	AActor* InstigatorPawn = InstigatedBy == nullptr ? nullptr : InstigatedBy->GetPawn();
	const FVector EventLocation = InstigatedBy == nullptr
		                              ? Causer->GetActorLocation()
		                              : InstigatorPawn->GetActorLocation();
	UAISense_Damage::ReportDamageEvent(GetWorld(),
	                                   GetOwner(),
	                                   InstigatorPawn,
	                                   Damage,
	                                   EventLocation,
	                                   GetOwner()->GetActorLocation());
}

void AEnemyCharacterBase::AggroNeighbours()
{if (!GetWorld()) return;
 
 	TArray<FHitResult> HitResults;
 	AAIControllerBase* AIController = nullptr;
 
 	UKismetSystemLibrary::SphereTraceMulti(GetWorld(),
 	                                       GetActorLocation(),
 	                                       GetActorLocation(),
 	                                       AggroRadius,
 	                                       UEngineTypes::ConvertToTraceType(ECC_Visibility),
 	                                       false,
 	                                       {this},
 	                                       EDrawDebugTrace::None,
 	                                       HitResults,
 	                                       true);
 
 	for (auto HitResult : HitResults)
 	{
 		AEnemyCharacterBase* Character = Cast<AEnemyCharacterBase>(HitResult.GetActor());
 
 		if (!Character) continue;
 
 		AIController = Cast<AAIControllerBase>(Character->GetController());
 
 		if (!AIController) continue;
 
 		AIController->SetTargetActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
 	}
}
