// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AIFunctionLibrary.generated.h"

class AEnemyCharacterBase;
/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UAIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Enemy")
	static void AggroEnemies(TArray<AEnemyCharacterBase*> Enemies);
};
