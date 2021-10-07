// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractiveActorBase.h"
#include "Components/KeyRingComponent.h"
#include "Components/TextRenderComponent.h"
#include "DoorBase.generated.h"

class UBaseBoxTriggerComponent;

/**
 * A base door class
 */

UCLASS()
class TRICKYPROTOTYPING_API ADoorBase: public AInteractiveActorBase
{
	GENERATED_BODY()

public:
	ADoorBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBaseBoxTriggerComponent* DoorTrigger = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TArray<UTextRenderComponent*> Messages;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bIsTriggerEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(EditCondition="bIsTriggerEnabled"))
	bool bRequireKey = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(EditCondition="bRequireKey"))
	EKey RequiredKey = EKey::Blue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Door")
	TMap<EKey, FColor> MessageColors {{EKey::Blue, FColor::Blue}, {EKey::Green, FColor::Green}, {EKey::Yellow, FColor::Yellow}};
	
	virtual void Disable() override;

	virtual void Enable() override;
	
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                         AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp,
	                         int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void SetMessagesTextAndColor(const FText Text, const FColor Color);

	UFUNCTION(BlueprintCallable)
	void SetMessagesHiddenInGame(const bool bIsHiddenInGame);

	UFUNCTION(BlueprintCallable)
	void SetKeyMessage();
	
};
