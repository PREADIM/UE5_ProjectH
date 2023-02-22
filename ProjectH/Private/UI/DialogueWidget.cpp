// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Dialogue/TextNAnim.h"
#include "AI/QuestNPCBase.h"
#include "UI/SelectQuestSlot.h"
#include "UI/MainQuestUI.h"


void UDialogueWidget::NPCDialogue()
{
	//SetNameText(FText::FromString(OwnerNPC->NPCName));
	//if (OwnerNPC->NormalDialogue.Num())
	//{
	//	NormalDialogue = OwnerNPC->NormalDialogue[0].Dialogue;
	//	SetNormalDialText(0);
	//}
	//else
	//{
	//	// �� ���� 23 02 22
	//	//DialText->SetText(FText::FromString("..."));
	//	SetCanQuestList();
	//}

	if (OwnerNPC->NormalDialogue.Dialogue.Num())
	{
		NormalDialogue = OwnerNPC->NormalDialogue.Dialogue;
		SetNormalDialText(0);
	}
	else
	{
		// �� ���� 23 02 22
		//DialText->SetText(FText::FromString("..."));
		SetCanQuestList();
	}
}

void UDialogueWidget::SetNormalDialText(int32 index)
{
	DialIndex = index;
	if (NormalDialogue.Num() == DialIndex)
	{
		//������ ��
		SetCanQuestList();
		NormalDialogue.Empty();
		DialIndex = 0;
	}
	else if(NormalDialogue.Num() > DialIndex)
	{
		NameText->SetText(NormalDialogue[DialIndex].NPCName);

		DialText->SetText(NormalDialogue[DialIndex].Dialogue);
	}
	
}


/* ������ �� �ִ� ����Ʈ ����Ʈ�� ���� ��. */
void UDialogueWidget::SetCanQuestList()
{
	UProjectHGameInstance* GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		SelectBox->ClearChildren();
		int32 Cnt = 0; // ����Ʈ �ε���
		if (!OwnerNPC->NPCQuests.Quests.Num())
			return;

		for (FNPCQuest& Quests : OwnerNPC->NPCQuests.Quests)
		{	
			if (Quests.bCanAccepted == true)
			{
				if (BP_SelectQuestSlot)
				{
					USelectQuestSlot* SSlot = CreateWidget<USelectQuestSlot>(GetWorld(), BP_SelectQuestSlot);
					if (SSlot)
					{
						SSlot->QuestNumber = Quests.QuestNumber;
						SSlot->QuestName = Quests.QuestName;
						SSlot->Dial = this;
						SSlot->Num = Cnt++; // NPC�� ����Ʈ�� ���°���� �˾ƾ� �ϱ⶧��.
						SSlot->Init();
						SSlot->SetPadding(FMargin(0.0f, 5.0f, 0.0f, 0.0f));
						SelectBox->AddChild(SSlot);
					}
				}
			}
		}
		SelectBox->SetRenderOpacity(1.0f);
		SelectBox->SetVisibility(ESlateVisibility::Visible);
	}
}


// ����Ʈ�� Ŭ���ϸ� �ش� ����Ʈ�� ���̾�αװ� ���.

void UDialogueWidget::SetSelectDial(int32 Index)
{
	SelectTextIndex = Index;
	if (SelectText.Num() == SelectTextIndex)
	{
		OwnerMainWidget->CloseDialogue(); // �̰� �ڿ� �ؾ��Ѵ�. OwnerQuestNum�� 0���� ����Ƿ�.
		OwnerNPC->QuestInfoOpen(OwnerQuestNum, OwnerController);	
	}
	else if(SelectText.Num() > SelectTextIndex)
	{
		NameText->SetText(SelectText[SelectTextIndex].NPCName);
		DialText->SetText(SelectText[SelectTextIndex].Dialogue);
	}
}


void UDialogueWidget::SetNameText(FText Name)
{
	NameText->SetText(Name);
}

void UDialogueWidget::SetDialText(FText Dial)
{
	DialText->SetText(Dial);
}


void UDialogueWidget::SelectBoxHidden()
{
	SelectBox->SetRenderOpacity(0.0f);
	SelectBox->SetVisibility(ESlateVisibility::Hidden);
}

void UDialogueWidget::Clear()
{
	SelectBox->ClearChildren();
	SelectBoxHidden();

	NormalDialogue.Empty(); // �Ϲ� NPC�� ������ �ؽ�Ʈ
	DialIndex = 0;

	SelectText.Empty(); // ������ ����Ʈ�� �ؽ�Ʈ
	SelectTextIndex = 0;

}




FReply UDialogueWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;

	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		if (!NormalDialogue.IsEmpty() && NormalDialogue.Num() > DialIndex)
		{
			SetNormalDialText(DialIndex + 1);
		}
		else if (!SelectText.IsEmpty() && SelectText.Num() > SelectTextIndex)
		{
			SetSelectDial(SelectTextIndex + 1);
		}
		
		_DEBUG("Mouse Left");
		
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
	
		_DEBUG("Mouse Right");
	}

	return reply.NativeReply;
}