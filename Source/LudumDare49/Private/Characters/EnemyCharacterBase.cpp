// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Characters/EnemyCharacterBase.h"

#include "Components/DamageControllerComponent.h"
#include "Perception/AISense_Damage.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
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

void AEnemyCharacterBase::AttackPlayer()
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
