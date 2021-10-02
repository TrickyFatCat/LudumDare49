// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponCoreTypes.h"
#include "WeaponFxComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUDUMDARE49_API UWeaponFxComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponFxComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void PlayImpactFX(const FHitResult& HitResult);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VFX")
	FHitScanImpactData DefaultImpactData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VFX")
	TMap<TEnumAsByte<EPhysicalSurface>, FHitScanImpactData> ImpactDataMap;
};
