// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "AI/AIFunctionLibrary.h"
#include "Characters/EnemyCharacterBase.h"

void UAIFunctionLibrary::AggroEnemies(TArray<AEnemyCharacterBase*> Enemies)
{
	for (const auto& Enemy : Enemies)
	{
		if (!IsValid(Enemy)) continue;

		Enemy->ForceAttack();
	}
}
