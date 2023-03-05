#pragma once
#include "ProjectH.h"
#include "Engine/DataTable.h"
#include "QuestStep.generated.h"



USTRUCT(BlueprintType)
struct FQuestStep : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ATriggerEventBase> BP_Trigger;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ATriggerEventBase* Trigger = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector TriggerLocation = FVector();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUseProgress = false; //������� ����ϴ� ����Ʈ�ΰ�?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentProgress = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxProgress = 0.f;
	/* �� �ᱹ�� ������� ǥ�� �Ѵٴ°��� %�� ��Ÿ���ų� Ƚ���� ��Ÿ���� �ȴ�. �ؽ�Ʈ�� ��Ÿ���� ����
	 Ʈ���ſ��� Ƚ���� ����ɶ����� ���ε� ���ָ� �ȴ�.
	 Ʈ���ſ��� ����Ʈ ������Ʈ�� ActiveQuest�� �����ؼ� CurrentProgress�� ������Ű��,
	 �ش� ������ ���� bSuccess�� true�� �ٲ㼭 Ʈ������ QuestClear�� �����ϸ� �ȴ�.*/

};