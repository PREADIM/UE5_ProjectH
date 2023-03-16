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
#include "UI/QuestingFunctionWidget.h"
#include "ActorComponent/QuestComponent/QuestingFunction.h"


void UDialogueWidget::NPCDialogue()
{
	if (OwnerNPC->NormalDialogue.Dialogue.Num())
	{
		NormalDialogue = OwnerNPC->NormalDialogue.Dialogue;
		SetNormalDialText(0);
	}
	else
	{
		// �� ���� 23 02 22
		DialText->SetText(FText::FromString("..."));
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
	SelectBox->ClearChildren();
	//int32 Cnt = 0; // ����Ʈ �ε���
	if (!OwnerNPC->NPCQuests.Quests.Num())
		return;

	for (int32 i = 0; i < OwnerNPC->NPCQuests.Quests.Num(); ++i)
	{
		if (OwnerNPC->NPCQuests.Quests[i].bCanAccepted == true)
		{
			if (BP_SelectQuestSlot)
			{
				USelectQuestSlot* SSlot = CreateWidget<USelectQuestSlot>(GetWorld(), BP_SelectQuestSlot);
				if (SSlot)
				{
					SSlot->QuestNumber = OwnerNPC->NPCQuests.Quests[i].QuestNumber;
					SSlot->QuestName = OwnerNPC->NPCQuests.Quests[i].QuestName;
					SSlot->QuestType = OwnerNPC->NPCQuests.Quests[i].QuestType;

					// ����Ʈ ���¿� ���� ���̾�α׸� �ٸ��� �ؾ��ϱ� ����.
					//FNPCQuest�� �׳� �Ѱ��ֱ⿣ �ʹ� ����ü�� ũ��.
					if (OwnerNPC->NPCQuests.Quests[i].CanSucceed)
						SSlot->DialougeState = EDialougeState::CanSucceed;
					else if (OwnerNPC->NPCQuests.Quests[i].Questing)
						SSlot->DialougeState = EDialougeState::Questing;
					else
						SSlot->DialougeState = EDialougeState::CanAccept;

					SSlot->Dial = this;
					SSlot->Num = i; // NPC�� ����Ʈ�� ���°���� �˾ƾ� �ϱ⶧��.
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

void UDialogueWidget::SetQuestingSelectList(UQuestingFunction* QuestingFunction, int32 FlagCnt)
{
	SelectBox->ClearChildren();

	for (int32 i = 1; i <= FlagCnt; ++i)
	{
		UQuestingFunctionWidget* QFW = CreateWidget<UQuestingFunctionWidget>(GetWorld(), BP_QuestingFunctionWidget);
		if (QFW)
		{
			QFW->FlagText = QuestingFunction->Flag1Text;
			QFW->Dial = this;
			QFW->OwnerMainWidget = OwnerMainWidget;
			QFW->Init(QuestingFunction, i);
			QFW->SetPadding(FMargin(0.0f, 5.0f, 0.0f, 0.0f));
			SelectBox->AddChild(QFW);
		}
	}
	SelectBox->SetRenderOpacity(1.0f);
	SelectBox->SetVisibility(ESlateVisibility::Visible);
}


// ����Ʈ�� Ŭ���ϸ� �ش� ����Ʈ�� ���̾�αװ� ���.
// ���̾�α� �Ŀ� ����Ʈ�� Ŭ���ϴ� �Լ�.
void UDialogueWidget::SetSelectDial(int32 Index)
{
	SelectTextIndex = Index;
	if (SelectText.Num() == SelectTextIndex)
	{	
		//OwnerQuestNum�� ����Ʈ �ѹ����ƴ϶� �ش� NPC�� ����Ʈ ����Ʈ�߿� ���° �ε������� ��Ÿ���� ��.
		if (OwnerNPC->NPCQuests.Quests[OwnerQuestNum].CanSucceed)
		{
			OwnerMainWidget->CloseDialogue(); // �̰� �ڿ� �ؾ��Ѵ�. OwnerQuestNum�� 0���� ����Ƿ�.
			OwnerNPC->QuestSucceedInfoOpen(OwnerQuestNum, OwnerController);
		}
		else if (OwnerNPC->NPCQuests.Quests[OwnerQuestNum].Questing)
		{
			if (OwnerNPC->NPCQuests.Quests[OwnerQuestNum].QuestingFunction) // �����ϸ� �ִ°�
				SetQuestingSelectList(OwnerNPC->NPCQuests.Quests[OwnerQuestNum].QuestingFunction, OwnerNPC->NPCQuests.Quests[OwnerQuestNum].QuestingFunction->FlagCnt);
			else
			{
				// QuestingFunction�� ���״�� Questing�� �ٽ� ���ɾ����� ���� �����ؾ��ϴ� �Լ��ε�, �װԾ����� �׳� �ݱ�.
				OwnerMainWidget->CloseDialogue();
			}
		}
		else
		{
			OwnerMainWidget->CloseDialogue(); // �̰� �ڿ� �ؾ��Ѵ�. OwnerQuestNum�� 0���� ����Ƿ�.
			OwnerNPC->QuestInfoOpen(OwnerQuestNum, OwnerController);
		}
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
	
		//_DEBUG("Mouse Left");
		
	}

	return reply.NativeReply;
}