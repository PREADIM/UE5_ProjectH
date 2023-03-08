// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectQuestSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameMode/ProjectHGameInstance.h"

void USelectQuestSlot::Init()
{
	QuestNameText->SetText(FText::FromString(QuestName));
	SlotButton->OnClicked.AddDynamic(this, &USelectQuestSlot::BindQuestStartButton);
	SetupQuestStateImg();
}

void USelectQuestSlot::SetupQuestStateImg()
{
	FSlateBrush Brush;

	switch (DialougeState)
	{
	case EDialougeState::CanSucceed:
		Brush.SetResourceObject(SucceedTex);		
		break;
	case EDialougeState::Questing:
		Brush.SetResourceObject(QuestingTex);
		break;
	case EDialougeState::CanAccept:
		Brush.SetResourceObject(CanAcceptTex);
		break;
	}

	Brush.SetImageSize(FVector2D(64.f, 64.f));
	if (QuestType == EQuestType::Main)
		Brush.TintColor = FSlateColor(MainQuestColor);
	else
	{
		Brush.TintColor = FSlateColor(SubQuestColor);
	}

	QuestStateImage->SetBrush(Brush);
}


void USelectQuestSlot::BindQuestStartButton()
{
	UProjectHGameInstance* GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		// DialougeState로 현재 퀘스트의 진행정도에 따라 다른 다이얼로그를 가져다 준다.
		Dial->SelectText = GI->GetDialData(DialougeState, QuestNumber);
		Dial->OwnerQuestNum = Num;
		Dial->SelectBoxHidden();
		Dial->SetSelectDial(0);
	}
}


