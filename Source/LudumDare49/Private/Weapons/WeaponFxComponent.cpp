// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "Weapons/WeaponFxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/WeaponCoreTypes.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"
#include "Components/DecalComponent.h"

UWeaponFxComponent::UWeaponFxComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UWeaponFxComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UWeaponFxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponFxComponent::PlayImpactFX(const FHitResult& HitResult)
{UWorld* World = GetWorld();
 
 	if (!World) return;
 
 	FHitScanImpactData ImpactData = DefaultImpactData;
 
 	if (HitResult.PhysMaterial.IsValid())
 	{
 		const EPhysicalSurface SurfaceType = HitResult.PhysMaterial->SurfaceType;
 
 		if (ImpactDataMap.Contains(SurfaceType))
 		{
 			ImpactData = ImpactDataMap[SurfaceType];
 		}
 	}
 
 	UGameplayStatics::SpawnEmitterAtLocation(World,
 	                                         ImpactData.ImpactEffect,
 	                                         HitResult.ImpactPoint,
 	                                         HitResult.ImpactNormal.Rotation());
 
 	UGameplayStatics::PlaySoundAtLocation(World, ImpactData.Sound, HitResult.ImpactPoint);
 
 	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(World,
 	                                                                ImpactData.DecalData.Material,
 	                                                                ImpactData.DecalData.Size,
 	                                                                HitResult.ImpactPoint,
 	                                                                HitResult.ImpactNormal.Rotation());
 
 	if (Decal)
 	{
 		Decal->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutDuration);
 	}
}

