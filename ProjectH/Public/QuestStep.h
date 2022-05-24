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
		class ATriggerEventBase* Trigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector TriggerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUseProgress; //������� ����ϴ� ����Ʈ�ΰ�?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentProgress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxProgress;
	/* �� �ᱹ�� ������� ǥ�� �Ѵٴ°��� %�� ��Ÿ���ų� Ƚ���� ��Ÿ���� �ȴ�. �ؽ�Ʈ�� ��Ÿ���� ����
	 Ʈ���ſ��� Ƚ���� ����ɶ����� ���ε� ���ָ� �ȴ�.
	 Ʈ���ſ��� ����Ʈ ������Ʈ�� ActiveQuest�� �����ؼ� CurrentProgress�� ������Ű��,
	 �ش� ������ ���� bSuccess�� true�� �ٲ㼭 Ʈ������ QuestClear�� �����ϸ� �ȴ�.*/

};