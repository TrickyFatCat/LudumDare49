// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawn.generated.h"

class AEnemyCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawnedSignature, AEnemyCharacterBase*, SpawnedCharacter);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnStartedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnFinishedSignature);

USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	TSubclassOf<AEnemyCharacterBase> EnemyClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn", meta=(ClampMin="0"))
	int32 Number = 1;
};

UCLASS()
class LUDUMDARE49_API AEnemySpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category="Spawn")
    	FOnEnemySpawnedSignature OnEnemySpawned;
    
    	UPROPERTY(BlueprintAssignable, Category="Spawn")
    	FOnSpawnStartedSignature OnSpawnStarted;
    	
    	UPROPERTY(BlueprintAssignable, Category="Spawn")
    	FOnSpawnFinishedSignature OnSpawnFinished;
    	
    	UFUNCTION(BlueprintCallable, Category="Spawn")
    	void StartSpawn();
    
    	UFUNCTION(BlueprintCallable, Category="Spawn")
    	void StopSpawn();
    
    protected:
    	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Spawn")
    	TArray<FSpawnData> SpawnTable;
    
    	UPROPERTY(VisibleAnywhere, Category="Spawn")
    	TArray<FSpawnData> CachedSpawnTable;
    
    	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Spawn", meta=(ClampMin="0"))
    	float SpawnPause = 1.f;
    
    	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Spawn", meta=(ClampMin="1"))
    	int32 SpawnNumber = 1;
    
    	int32 CurrentEnemyID = 0;
    
    	int32 SpawnedEnemiesCount = 0;
    
    	int32 SpawnedEnemiesMax = 0;
    
    	FTimerHandle SpawnTimerHandle;
    
    	AEnemyCharacterBase* SpawnEnemy(const TSubclassOf<AEnemyCharacterBase> EnemyClass,
    	                            const FTransform& SpawnTransform);
    
    	UFUNCTION()
    	void ProcessSpawnQueue();
    
    	virtual FTransform CalculateSpawnTransform();
    
    	bool IsEnemyIdOutOfBounds();
};
