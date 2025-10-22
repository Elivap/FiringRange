#include "SkillTreeComponent.h"
#include "Engine/Engine.h"

USkillTreeComponent::USkillTreeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USkillTreeComponent::BeginPlay()
{
    Super::BeginPlay();
}

//////////////////////////
// Internal C++ Functions

FSkillNode* USkillTreeComponent::FindSkill(FName SkillID)
{
    for (FSkillNode& Skill : Skills)
    {
        if (Skill.SkillID == SkillID)
            return &Skill;
    }
    return nullptr;
}

bool USkillTreeComponent::ArePrerequisitesMet(const FSkillNode& Skill) const
{
    if (Skill.Prerequisites.Num() == 0)
        return true;

    for (const FName& PreReqID : Skill.Prerequisites)
    {
        bool bFound = false;
        bool bUnlocked = false;

        for (const FSkillNode& S : Skills)
        {
            if (S.SkillID == PreReqID)
            {
                bFound = true;
                bUnlocked = S.bUnlocked;
                break;
            }
        }

        if (!bFound)
        {
            UE_LOG(LogTemp, Warning, TEXT("Prerequisite '%s' for skill '%s' not found."), *PreReqID.ToString(), *Skill.SkillID.ToString());
            return false;
        }
        if (!bUnlocked)
        {
            return false;
        }
    }
    return true;
}

//////////////////////////
// Public Functions

bool USkillTreeComponent::UnlockSkill(FName SkillID)
{
    if (SkillID.IsNone())
    {
        UE_LOG(LogTemp, Warning, TEXT("UnlockSkill called with empty SkillID."));
        return false;
    }

    FSkillNode* Skill = FindSkill(SkillID);
    if (!Skill)
    {
        UE_LOG(LogTemp, Warning, TEXT("UnlockSkill failed: Skill '%s' not found."), *SkillID.ToString());
        return false;
    }

    if (Skill->bUnlocked)
    {
        UE_LOG(LogTemp, Log, TEXT("Skill '%s' already unlocked."), *Skill->SkillName);
        return false;
    }

    if (!ArePrerequisitesMet(*Skill))
    {
        UE_LOG(LogTemp, Warning, TEXT("UnlockSkill failed: prerequisites not met for '%s'."), *Skill->SkillName);
        return false;
    }

    Skill->bUnlocked = true;
    UE_LOG(LogTemp, Log, TEXT("Unlocked Skill: '%s' (ID: %s)"), *Skill->SkillName, *Skill->SkillID.ToString());
    return true;
}

bool USkillTreeComponent::IsSkillUnlocked(FName SkillID) const
{
    if (SkillID.IsNone()) return false;

    for (const FSkillNode& Skill : Skills)
    {
        if (Skill.SkillID == SkillID)
            return Skill.bUnlocked;
    }

    return false;
}

FSkillNode USkillTreeComponent::FindSkillBP(FName SkillID) const
{
    for (const FSkillNode& Skill : Skills)
    {
        if (Skill.SkillID == SkillID)
            return Skill; // returns a copy, safe for Blueprint
    }

    return FSkillNode(); // empty fallback
}
