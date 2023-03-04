// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuestingFunctionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ActorComponent/QuestComponent/QuestingFunction.h"


void UQuestingFunctionWidget::Init(UQuestingFunction* QuestingFunction, int32 FlagIndex)
{
	Index = FlagIndex;
	switch (FlagIndex)
	{
	case 1:
		QuestingFlagText->SetText(FText::FromString(FlagText));
		SlotButton->OnClicked.AddDynamic(QuestingFunction, &UQuestingFunction::Flag1Function);
		break;
	case 2:
		QuestingFlagText->SetText(FText::FromString(FlagText));
		SlotButton->OnClicked.AddDynamic(QuestingFunction, &UQuestingFunction::Flag2Function);
		break;
	case 3:
		QuestingFlagText->SetText(FText::FromString(FlagText));
		SlotButton->OnClicked.AddDynamic(QuestingFunction, &UQuestingFunction::Flag3Function);
		break;
	}
}


