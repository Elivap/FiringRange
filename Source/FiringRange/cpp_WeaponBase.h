// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "cpp_WeaponBase.generated.h"

UCLASS()
class FIRINGRANGE_API Acpp_WeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Acpp_WeaponBase();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFiringRangeProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	virtual void Shoot();

	void SetPlayerPointer(ACharacter* Playerptr);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FireSound;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int MAGammo;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int TotalAmmo;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int CurrentAmmo;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int UsedAmmo;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UTexture2D* GunIcon;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	class USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* ImpactDecal;

	class ACharacter* player;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* PickupTrigger;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
