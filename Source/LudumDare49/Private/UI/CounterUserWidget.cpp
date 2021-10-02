// Made by Title Goose Team during LudumDare 49. All rights reserved.


#include "UI/CounterUserWidget.h"

#include "Components/TextBlock.h"

void UCounterUserWidget::SetCounter(const int32 Value) const
{
	const FString String = FString::Printf(TEXT("%3d"), Value);
	Text_Counter->SetText(FText::AsCultureInvariant(String));
}
