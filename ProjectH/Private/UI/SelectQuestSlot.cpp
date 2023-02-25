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
		//�� ���⼭ ���������� �ƴ��� �Ǵ��ؼ� ���̾�α׵� ���Ӱ� �ָ�ɵ��ϴ�.

		// DialougeState�� ���� ����Ʈ�� ���������� ���� �ٸ� ���̾�α׸� ������ �ش�.
		Dial->SelectText = GI->GetDialData(DialougeState, QuestNumber);
		Dial->OwnerQuestNum = Num;
		Dial->SelectBoxHidden();
		Dial->SetSelectDial(0);
	}
}
