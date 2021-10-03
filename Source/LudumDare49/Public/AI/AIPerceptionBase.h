// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIPerceptionBase.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UAIPerceptionBase : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
	AActor* GetPlayer() const;
};
