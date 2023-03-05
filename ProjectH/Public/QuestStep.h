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
		bool bUseProgress = false; //진행률을 사용하는 퀘스트인가?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentProgress = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxProgress = 0.f;
	/* ★ 결국엔 진행률을 표기 한다는것은 %로 나타내거나 횟수로 나타내면 된다. 텍스트를 나타내는 것은
	 트리거에서 횟수가 변경될때마다 바인딩 해주면 된다.
	 트리거에서 퀘스트 컴포넌트의 ActiveQuest에 접근해서 CurrentProgress를 변동시키고,
	 해당 조건이 차면 bSuccess를 true로 바꿔서 트리거의 QuestClear를 실행하면 된다.*/

};