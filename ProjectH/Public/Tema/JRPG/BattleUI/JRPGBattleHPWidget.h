// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectH.h"
#include "Blueprint/UserWidget.h"
#include "JRPGBattleHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UJRPGBattleHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* ULT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* CharLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UWrapBox* BuffIconWrapBox;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UJRPGBuffWidget> BP_BuffWidget;
	UPROPERTY()
		TArray<class UJRPGBuffWidget*> BuffWidgets;

	UPROPERTY()
		class AJRPGUnit* OwnerUnit;

public:
	virtual void NativeConstruct() override;

	void Init(class AJRPGUnit* Unit);
	void SetBuffIcon();

	UFUNCTION(BlueprintCallable)
		float RetHP();
	UFUNCTION(BlueprintCallable)
		float RetMP();
	UFUNCTION(BlueprintCallable)
		float RetULT();

};
