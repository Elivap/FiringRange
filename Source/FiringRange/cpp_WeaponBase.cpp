// Fill out your copyright notice in the Description page of Project Settings.

#include "cpp_WeaponBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FiringRangeCharacter.h"
#include "Engine/Texture2D.h"


// Sets default values
Acpp_WeaponBase::Acpp_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	SetRootComponent(GunMesh);

    PickupTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("PickupTrigger"));
    PickupTrigger->SetupAttachment(RootComponent);
    PickupTrigger->SetSphereRadius(100.f);
    PickupTrigger->SetCollisionProfileName(TEXT("Trigger"));
    PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &Acpp_WeaponBase::OnOverlapBegin);

}







void Acpp_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	


}

void Acpp_WeaponBase::SetPlayerPointer(ACharacter* Playerptr)
{
    if (!Playerptr)
    {
        UE_LOG(LogTemp, Error, TEXT("SetPlayerPointer failed: Playerptr is null."));
        return;
    }

    player = Playerptr;
    UE_LOG(LogTemp, Warning, TEXT("SetPlayerPointer succeeded: player set to %s"), *player->GetName());
}


void Acpp_WeaponBase::Shoot()
{
    if (!player)
    {
        UE_LOG(LogTemp, Error, TEXT("Shoot aborted: player is null"));
        return;
    }

    if (FireSound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, player->GetActorLocation());
    }

   
 
    FVector ViewLocation;
    FRotator ViewRotation;
    player->GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
    FVector ViewDirection = ViewRotation.Vector();

   
    FVector VisualFireOrigin = GunMesh->GetComponentLocation()
        + GunMesh->GetForwardVector()
        + GunMesh->GetRightVector() * 30.f
        + GunMesh->GetUpVector() * 15.f;

    FVector TraceEnd = ViewLocation + (ViewDirection * 3000.f);
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(player);
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        ViewLocation,
        TraceEnd,
        ECC_Visibility,
        Params
    );

    FVector ImpactPoint = bHit ? HitResult.ImpactPoint : TraceEnd;
    FColor LineColor = bHit ? FColor::Green : FColor::Red;


    DrawDebugLine(GetWorld(), VisualFireOrigin, ImpactPoint, LineColor, false, 0.0f, 0, 0.0f);

    if (MuzzleFlash)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            MuzzleFlash,
            VisualFireOrigin,
            GunMesh->GetComponentRotation()
        );
    }

    if (bHit && ImpactDecal)
    {
        FVector DecalSize = FVector(10.f, 10.f, 10.f);
        FRotator DecalRotation = HitResult.ImpactNormal.Rotation(); 

       UGameplayStatics::SpawnDecalAttached(
           ImpactDecal,
            FVector(10.0f),  // decal size
            HitResult.GetComponent(),
            NAME_None,
            HitResult.ImpactPoint,
            HitResult.ImpactNormal.Rotation(),
            EAttachLocation::KeepWorldPosition,
            5.0f // lifespan
        );
    }



    if (GEngine) {
        FString FireMsg = FString::Printf(TEXT("Fired"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, *FireMsg);
    }


}

// Called every frame
void Acpp_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Acpp_WeaponBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AFiringRangeCharacter* Character = Cast<AFiringRangeCharacter>(OtherActor);
    if (!Character) return;

    if (!Character->PrimaryWeapon)
    {
        Character->PrimaryWeapon = this;
        Character->ActiveWeapon = this;

        AttachToComponent(Character->GetMesh1P(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GripPoint"));
        SetOwner(Character);
        SetPlayerPointer(Character);
        UE_LOG(LogTemp, Warning, TEXT("Equipped as PRIMARY weapon."));
    }
    else if (!Character->SecondaryWeapon)
    {
        Character->SecondaryWeapon = this;

        AttachToComponent(Character->GetMesh1P(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("StowPoint"));
        SetOwner(Character);
        SetPlayerPointer(Character);
        SetActorHiddenInGame(true); 
        UE_LOG(LogTemp, Warning, TEXT("Equipped as SECONDARY weapon."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot equip weapon: both slots full."));
        return;
    }

    PickupTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

