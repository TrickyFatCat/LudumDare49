// Made by Title Goose Team during LudumDare 49. All rights reserved.


#pragma once

#include "Core/ProjectUtils.h"
#include "GameFramework/Character.h"

bool FProjectUtils::GetPlayerViewPoint(AActor* CharacterActor, FVector& ViewLocation, FRotator& ViewRotation)
{
	ACharacter* Character = Cast<ACharacter>(CharacterActor);

	if (!Character) return false;

	if (Character->IsPlayerControlled())
	{
		const APlayerController* Controller = Character->GetController<APlayerController>();

		if (!Controller) return false;

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	return true;
}
