#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillNode.h"
#include "SkillTreeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIRINGRANGE_API USkillTreeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USkillTreeComponent();

protected:
    virtual void BeginPlay() override;

public:
    // All skills in the tree
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    TArray<FSkillNode> Skills;

    /** Unlocks a skill if prerequisites are met */
    UFUNCTION(BlueprintCallable, Category = "Skill Tree")
    bool UnlockSkill(FName SkillID);

    /** Check if skill is unlocked */
    UFUNCTION(BlueprintCallable, Category = "Skill Tree")
    bool IsSkillUnlocked(FName SkillID) const;

    /** Blueprint-friendly read-only lookup (returns copy) */
    UFUNCTION(BlueprintCallable, Category = "Skill Tree")
    FSkillNode FindSkillBP(FName SkillID) const;

private:
    /** Internal C++ pointer version for modification (not exposed to Blueprint) */
    FSkillNode* FindSkill(FName SkillID);

    /** Check if prerequisites are unlocked */
    bool ArePrerequisitesMet(const FSkillNode& Skill) const;
};
