// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionMenu.h"
#include "PlayerControllerBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "GameMode/ProjectHGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


/* 게임 인스턴스 안에있는 사용가능한 해상도를 컨트롤러에서 받아온다.*/
void UOptionMenu::Init()
{
	GI = Cast<UProjectHGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		TArray<FString>& arr = GI->ResolutionArr;
		for (auto Res : arr)
			ResComboBox->AddOption(Res);

		AddComboBoxList();

	// 게임 인스턴스에서 마지막 셋팅들을 전부 받아온다. 그리고 콤보박스에 셋 셀렉트 옵션 한다.
		GI->GetDefaultGameSetting(Resolution, AASetting, ShadowSetting, TextureSetting, MouseSensitivity);
		SetComboBox();
		SetOtherOption();
	}
	
	ApplyButton->OnClicked.AddDynamic(this, &UOptionMenu::Apply);
}


void UOptionMenu::AddComboBoxList()
{
	AAComboBox->AddOption("NONE");
	AAComboBox->AddOption("x2");
	AAComboBox->AddOption("x4");
	AAComboBox->AddOption("x8");

	ShadowComboBox->AddOption(L"낮음");
	ShadowComboBox->AddOption(L"보통");
	ShadowComboBox->AddOption(L"높음");
	ShadowComboBox->AddOption("NONE");

	TextureComboBox->AddOption(L"낮음");
	TextureComboBox->AddOption(L"보통");
	TextureComboBox->AddOption(L"높음");
	TextureComboBox->AddOption("NONE");
}

void UOptionMenu::AddViewportSetupComboBox()
{
	AddToViewport();
	SetComboBox();
}

void UOptionMenu::SetComboBox()
{
	ResComboBox->SetSelectedOption(Resolution);

	_DEBUG("AA : %d", AASetting);
	_DEBUG("Shadow : %d", ShadowSetting);
	_DEBUG("Texture : %d", TextureSetting);

	switch (AASetting)
	{
	case 0:
		AAComboBox->SetSelectedOption("NONE");
		break;
	case 1:
	case 2:
		AAComboBox->SetSelectedOption("x2");
		break;

	case 3:
	case 4:
		AAComboBox->SetSelectedOption("x4");
		break;

	case 7:
	case 8:
		AAComboBox->SetSelectedOption("x8");
		break;
	default:
		AAComboBox->SetSelectedOption("NONE");
	}

	switch (ShadowSetting)
	{
	case 1:
		ShadowComboBox->SetSelectedOption(L"낮음");
		break;
	case 2:
		ShadowComboBox->SetSelectedOption(L"보통");
		break;
	case 3:
		ShadowComboBox->SetSelectedOption(L"높음");
		break;
	default:
		ShadowComboBox->SetSelectedOption("NONE");
		break;
	}

	switch (TextureSetting)
	{
	case 1:
		TextureComboBox->SetSelectedOption(L"낮음");
		break;
	case 2:
		TextureComboBox->SetSelectedOption(L"보통");
		break;
	case 3:
		TextureComboBox->SetSelectedOption(L"높음");
		break;
	default:
		TextureComboBox->SetSelectedOption("NONE");
		break;
	}
}


void UOptionMenu::SetOtherOption()
{
	MouseSensitivity = MouseSensitivity / MaxMouseSensitivity;
}


void UOptionMenu::SetINI()
{
	FString path = FPaths::SourceConfigDir(); // 소스파일이있는 곳의 config파일.
	path += "DefaultMainGameSetting.ini";

	/* 현재 보여질 값으로 저장. */
	if (bRes)
	{
		Resolution = SelectResolution;
		//GConfig->SetString(TEXT("/Script/GameSetting.MainGameSetting"), TEXT("Resolution"), *SelectResolution, path);
		SET_OPTION_S("Resolution", path, *Resolution);
		bRes = false;
	}
	if (bAA)
	{
		AASetting = SelectAASetting;
		//GConfig->SetInt(TEXT("/Script/GameSetting.MainGameSetting"), TEXT("AASetting"), SelectAASetting, path);
		SET_OPTION("AASetting", path, AASetting);
		bAA = false;
	}
	if (bShadow)
	{
		ShadowSetting = SelectShadowSetting;
		//GConfig->SetInt(TEXT("/Script/GameSetting.MainGameSetting"), TEXT("ShadowQ"), SelectShadowSetting, path);
		SET_OPTION("ShadowQ", path, ShadowSetting);
		bShadow = false;
	}
	if (bTex)
	{
		TextureSetting = SelectTextureSetting;
		//GConfig->SetInt(TEXT("/Script/GameSetting.MainGameSetting"), TEXT("TextureQ"), SelectTextureSetting, path);
		SET_OPTION("TextureQ", path, TextureSetting);
		bTex = false;
	}
	if (bMS)
	{
		MouseSensitivity = SelectMouseSensitivity;
		float ControllerMS = UKismetMathLibrary::FClamp(MouseSensitivity * MaxMouseSensitivity, 1, MaxMouseSensitivity);
		// 슬라이더에선 0~ 1값이 필요하지만 컨트롤러에선 1~ 의 값이 필요하다.
		//GConfig->SetInt(TEXT("/Script/GameSetting.MainGameSetting"), TEXT("MouseSensitivity"), SelectTextureSetting, path);
		SET_OPTION("MouseSensitivity", path, ControllerMS);
		bMS = false;

		APlayerControllerBase* Controller = Cast<APlayerControllerBase>(OwnerController);
		if (Controller)
		{
			Controller->MouseSensitivity = ControllerMS;
			Controller->SetNewMouseSensitivity();
		}
	}


	if (GI)
		GI->GISetGameSetting(Resolution, AASetting, ShadowSetting, TextureSetting, MouseSensitivity);

}


/* 블루 프린트에서 콤보 박스에서 선택한 것을 가져와서 커맨드 실행.*/
void UOptionMenu::SetResCommand(FString str)
{
	SelectResolution = str;
	bRes = true;
}

void UOptionMenu::SetAACommand(int32 str)
{
	SelectAASetting = str;
	bAA = true;
}

void UOptionMenu::SetShadowCommand(int32 str)
{
	SelectShadowSetting = str;
	bShadow = true;
}

void UOptionMenu::SetTextureCommand(int32 str)
{
	SelectTextureSetting = str;
	bTex = true;
}

void UOptionMenu::SetMouseSensitivity(float NewMS)
{
	SelectMouseSensitivity = NewMS;
	bMS = true;
}

void UOptionMenu::Apply()
{
	SetINI();
}


// true는 끄기, false는 켜기
void UOptionMenu::OptionAnimation(bool IsOpened)
{
	PlayAnimation(OptionFade, 0.f, 1, IsOpened ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward);
	if (IsOpened)
	{
		bRes = false;
		bAA = false;
		bShadow = false;
		bTex = false;
		bMS = false;

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &UOptionMenu::OptionMenuRemove, OptionFade->GetEndTime(), false);
	}
}


void UOptionMenu::OptionMenuRemove()
{
	RemoveFromParent();
}