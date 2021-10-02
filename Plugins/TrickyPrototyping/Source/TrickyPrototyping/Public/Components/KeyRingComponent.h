// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyRingComponent.generated.h"

UENUM(BlueprintType)
enum class EKey : uint8
{
	Green,
	Blue,
	Yellow
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUDUMDARE49_API UKeyRingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UKeyRingComponent();

protected:
	virtual void BeginPlay() override;

public:
	void AddKey(const EKey Key);

	bool HasKey(const EKey Key);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="KeyRing")
	TMap<EKey, bool> KeyRing{{EKey::Blue, false}, {EKey::Green, false}, {EKey::Yellow, false}};
};
