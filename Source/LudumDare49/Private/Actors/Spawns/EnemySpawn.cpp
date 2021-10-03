// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Actors/Spawns/EnemySpawn.h"

#include "Characters/EnemyCharacterBase.h"
#include "Math/TransformCalculus3D.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AEnemySpawn::AEnemySpawn()
{
	PrimaryActorTick.bCanEverTick = true;
	SpawnArea = CreateDefaultSubobject<UBoxComponent>("SpawnArea");
	SetRootComponent(SpawnArea);
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnArea->SetGenerateOverlapEvents(false);
}

void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawn::StartSpawn()
{
	if (SpawnTable.Num() <= 0 || SpawnedEnemiesMax <= 0) return;

	ProcessSpawnQueue();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawn::ProcessSpawnQueue, SpawnPause, true);
	OnSpawnStarted.Broadcast();
}

void AEnemySpawn::StopSpawn()
{
	if (SpawnTable.Num() <= 0 || !GetWorldTimerManager().IsTimerActive(SpawnTimerHandle)) return;

	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

AEnemyCharacterBase* AEnemySpawn::SpawnEnemy(const TSubclassOf<AEnemyCharacterBase> EnemyClass,
                                             const FTransform& SpawnTransform)
{
	AEnemyCharacterBase* EnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacterBase>(EnemyClass, SpawnTransform);

	if (!EnemyCharacter) return nullptr;

	OnEnemySpawned.Broadcast(EnemyCharacter);

	return EnemyCharacter;
}

void AEnemySpawn::ProcessSpawnQueue()
{
	if (CurrentEnemyID >= CachedSpawnTable.Num())
	{
		StopSpawn();
		return;
	}

	for (int32 i = 1; i <= SpawnNumber; ++i)
	{
		if (CachedSpawnTable[CurrentEnemyID].Number <= 0 || CachedSpawnTable[CurrentEnemyID].EnemyClass == nullptr)
		{
			CurrentEnemyID++;

			if (IsEnemyIdOutOfBounds()) break;

			continue;
		}

		AEnemyCharacterBase* SpawnedEnemy =
			SpawnEnemy(SpawnTable[CurrentEnemyID].EnemyClass, CalculateSpawnTransform());

		if (!SpawnedEnemy)
		{
			CurrentEnemyID++;

			if (IsEnemyIdOutOfBounds()) break;

			continue;
		}

		CachedSpawnTable[CurrentEnemyID].Number--;
		SpawnedEnemiesCount++;

		if (CachedSpawnTable[CurrentEnemyID].Number <= 0) CurrentEnemyID++;

		if (IsEnemyIdOutOfBounds()) break;
	}
}

FTransform AEnemySpawn::CalculateSpawnTransform()
{
	const FVector VolumeExtent = SpawnArea->GetScaledBoxExtent();
	const FVector VolumeOrigin = SpawnArea->GetComponentLocation();
	const FVector Location = UKismetMathLibrary::RandomPointInBoundingBox(VolumeOrigin, VolumeExtent);
	const FRotator Rotation = FRotator(0.f, GetActorRotation().Yaw, 0.f);
	return FTransform(Rotation, FVector(Location.X, Location.Y, GetActorLocation().Z), FVector::OneVector);
}

bool AEnemySpawn::IsEnemyIdOutOfBounds()
{
	if (CurrentEnemyID >= CachedSpawnTable.Num())
	{
		StopSpawn();
		OnSpawnFinished.Broadcast();
		return true;
	}

	return false;
}
