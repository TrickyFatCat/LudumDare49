// Made by Title Goose Team during LudumDare 49. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CounterUserWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UCounterUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCounter(const int32 Value) const;

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Counter = nullptr;
};
