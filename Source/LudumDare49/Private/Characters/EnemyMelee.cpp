// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Characters/EnemyMelee.h"
#include "Components/CapsuleComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AEnemyMelee::AEnemyMelee()
{
	DamageTrigger = CreateDefaultSubobject<UCapsuleComponent>("DamageTrigger");
	DamageTrigger->SetupAttachment(GetMesh());
	DamageTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyMelee::BeginPlay()
{
	Super::BeginPlay();

	DamageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMelee::OnTriggerBeginOverlap);
}

void AEnemyMelee::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex,
                                        bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(APlayerCharacter::StaticClass())) return;

	UGameplayStatics::ApplyPointDamage(OtherActor,
	                                   AttackDamage,
	                                   GetActorForwardVector(),
	                                   SweepResult,
	                                   GetController(),
	                                   this,
	                                   UDamageType::StaticClass());
}

void AEnemyMelee::StartAttack()
{
	Super::StartAttack();

	DamageTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyMelee::FinishAttack()
{
	Super::FinishAttack();
	
	DamageTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
