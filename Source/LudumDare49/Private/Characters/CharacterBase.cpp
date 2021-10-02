// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Characters/CharacterBase.h"
#include "Components/DamageControllerComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageController = CreateDefaultSubobject<UDamageControllerComponent>("DamageController");
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

