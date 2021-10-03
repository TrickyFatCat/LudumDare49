// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Actors/JumpPad.h"

#include "Characters/PlayerCharacter.h"
#include "Components/TriggerComponents/BaseSphereTriggerComponent.h"

AJumpPad::AJumpPad()
{
	PrimaryActorTick.bCanEverTick = true;

	JumpTrigger = CreateDefaultSubobject<UBaseSphereTriggerComponent>("JumpTrigger");
	SetRootComponent(JumpTrigger);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(JumpTrigger);

	JumpDirection = CreateDefaultSubobject<UArrowComponent>("JumpDirection");
	JumpDirection->SetupAttachment(Mesh);
}

void AJumpPad::BeginPlay()
{
	Super::BeginPlay();

	JumpTrigger->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::OnTriggerBeginOverlap);
}

void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJumpPad::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                     AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex,
                                     bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!PlayerCharacter) return;

	PlayerCharacter->LaunchCharacter(JumpDirection->GetRelativeRotation().Vector() * JumpForce, false, true);
}
