// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectQuestSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameMode/ProjectHGameInstance.h"

void USelectQuestSlot::Init()
{
	QuestNameText->SetText(FText::FromString(QuestName));
	SlotButton->OnClicked.AddDynamic(this, &USelectQuestSlot::BindQuestStartButton);
}


void USelectQuestSlot::BindQuestStartButton()
{
	UProjectHGameInstance* GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		//★ 여기서 진행중인지 아닌지 판단해서 다이얼로그도 새롭게 주면될듯하다.

		// DialougeState로 현재 퀘스트의 진행정도에 따라 다른 다이얼로그를 가져다 준다.
		Dial->SelectText = GI->GetDialData(DialougeState, QuestNumber);
		Dial->OwnerQuestNum = Num;
		Dial->SelectBoxHidden();
		Dial->SetSelectDial(0);
	}
}
