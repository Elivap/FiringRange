#pragma once

#include "CoreMinimal.h"
#include "SkillNode.generated.h"

USTRUCT(BlueprintType)
struct FSkillNode
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString SkillName;

    // IDs of required skills
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FName> Prerequisites;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    bool bUnlocked = false;

    FSkillNode() {}

    FSkillNode(FName InID, FString InName, TArray<FName> InPrerequisites)
        : SkillID(InID), SkillName(InName), Prerequisites(InPrerequisites), bUnlocked(false)
    {}
};
