// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NormalIconUI.h"

void UNormalIconUI::NativeConstruct()
{
	Super::NativeConstruct();

	//���⼭ �ش� �ڵ带 �ۼ��ϸ�, ������ �𸣰����� �������� �ٷιٷ� ��µ�������.
	/*SubQuestWidget->SetRenderOpacity(0.f);
	MainQuestWidget->SetRenderOpacity(0.f);
	SubQuestingWidget->SetRenderOpacity(0.f);
	MainQuestingWidget->SetRenderOpacity(0.f);
	SubSucceedWidget->SetRenderOpacity(0.f);
	MainSucceedWidget->SetRenderOpacity(0.f);*/
}


void UNormalIconUI::SetRenderIcon(EQuestState State)
{
	switch (State)
	{
	case EQuestState::SubQuest:
		if (SubQuestWidget->GetRenderOpacity() != 1.f)
		{
			if (MainQuestWidget->GetRenderOpacity() == 1.f)
				MainQuestWidget->SetRenderOpacity(0.f);

			if (SubQuestingWidget->GetRenderOpacity() == 1.f)
				SubQuestingWidget->SetRenderOpacity(0.f);

			if (MainQuestingWidget->GetRenderOpacity() == 1.f)
				MainQuestingWidget->SetRenderOpacity(0.f);

			if (SubSucceedWidget->GetRenderOpacity() == 1.f)
				SubSucceedWidget->SetRenderOpacity(0.f);

			if (MainSucceedWidget->GetRenderOpacity() == 1.f)
				MainSucceedWidget->SetRenderOpacity(0.f);

			SubQuestWidget->SetRenderOpacity(1.f);
		}
		break;
	case EQuestState::MainQuest:
		if (MainQuestWidget->GetRenderOpacity() != 1.f)
		{
			if (SubQuestWidget->GetRenderOpacity() == 1.f)
				SubQuestWidget->SetRenderOpacity(0.f);

			if (SubQuestingWidget->GetRenderOpacity() == 1.f)
				SubQuestingWidget->SetRenderOpacity(0.f);

			if (MainQuestingWidget->GetRenderOpacity() == 1.f)
				MainQuestingWidget->SetRenderOpacity(0.f);

			if (SubSucceedWidget->GetRenderOpacity() == 1.f)
				SubSucceedWidget->SetRenderOpacity(0.f);

			if (MainSucceedWidget->GetRenderOpacity() == 1.f)
				MainSucceedWidget->SetRenderOpacity(0.f);

			MainQuestWidget->SetRenderOpacity(1.f);
		}
		break;
	case EQuestState::SubQuesting:
		if (SubQuestingWidget->GetRenderOpacity() != 1.f)
		{
			if (SubQuestWidget->GetRenderOpacity() == 1.f)
				SubQuestWidget->SetRenderOpacity(0.f);

			if (MainQuestWidget->GetRenderOpacity() == 1.f)
				MainQuestWidget->SetRenderOpacity(0.f);

			if (MainQuestingWidget->GetRenderOpacity() == 1.f)
				MainQuestingWidget->SetRenderOpacity(0.f);

			if (SubSucceedWidget->GetRenderOpacity() == 1.f)
				SubSucceedWidget->SetRenderOpacity(0.f);

			if (MainSucceedWidget->GetRenderOpacity() == 1.f)
				MainSucceedWidget->SetRenderOpacity(0.f);

			SubQuestingWidget->SetRenderOpacity(1.f);
		}
		break;
	case EQuestState::MainQuesting:
		if (MainQuestingWidget->GetRenderOpacity() != 1.f)
		{
			if (SubQuestWidget->GetRenderOpacity() == 1.f)
				SubQuestWidget->SetRenderOpacity(0.f);

			if (MainQuestWidget->GetRenderOpacity() == 1.f)
				MainQuestWidget->SetRenderOpacity(0.f);

			if (SubQuestingWidget->GetRenderOpacity() == 1.f)
				SubQuestingWidget->SetRenderOpacity(0.f);

			if (SubSucceedWidget->GetRenderOpacity() == 1.f)
				SubSucceedWidget->SetRenderOpacity(0.f);

			if (MainSucceedWidget->GetRenderOpacity() == 1.f)
				MainSucceedWidget->SetRenderOpacity(0.f);

			MainQuestingWidget->SetRenderOpacity(1.f);
		}
		break;
	case EQuestState::SubSucceedQuest:
		if (SubSucceedWidget->GetRenderOpacity() != 1.f)
		{
			if (SubQuestWidget->GetRenderOpacity() == 1.f)
				SubQuestWidget->SetRenderOpacity(0.f);

			if (MainQuestWidget->GetRenderOpacity() == 1.f)
				MainQuestWidget->SetRenderOpacity(0.f);

			if (SubQuestingWidget->GetRenderOpacity() == 1.f)
				SubQuestingWidget->SetRenderOpacity(0.f);

			if (MainQuestingWidget->GetRenderOpacity() == 1.f)
				MainQuestingWidget->SetRenderOpacity(0.f);

			if (MainSucceedWidget->GetRenderOpacity() == 1.f)
				MainSucceedWidget->SetRenderOpacity(0.f);

			SubSucceedWidget->SetRenderOpacity(1.f);
		}
		break;
	case EQuestState::MainSucceedQuest:
		if (MainSucceedWidget->GetRenderOpacity() != 1.f)
		{
			if (SubQuestWidget->GetRenderOpacity() == 1.f)
				SubQuestWidget->SetRenderOpacity(0.f);

			if (MainQuestWidget->GetRenderOpacity() == 1.f)
				MainQuestWidget->SetRenderOpacity(0.f);

			if (SubQuestingWidget->GetRenderOpacity() == 1.f)
				SubQuestingWidget->SetRenderOpacity(0.f);

			if (MainQuestingWidget->GetRenderOpacity() == 1.f)
				MainQuestingWidget->SetRenderOpacity(0.f);

			if (SubSucceedWidget->GetRenderOpacity() == 1.f)
				SubSucceedWidget->SetRenderOpacity(0.f);

			MainSucceedWidget->SetRenderOpacity(1.f);
		}
		break;
	}
}