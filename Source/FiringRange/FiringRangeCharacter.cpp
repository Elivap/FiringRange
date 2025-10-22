// Copyright Epic Games, Inc. All Rights Reserved.

#include "FiringRangeCharacter.h"
#include "cpp_WeaponBase.h"
#include "SkillTreeComponent.h"
#include "FiringRangeProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFiringRangeCharacter

AFiringRangeCharacter::AFiringRangeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	SkillTree = CreateDefaultSubobject<USkillTreeComponent>(TEXT("SkillTree"));


}

void AFiringRangeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	PrimaryWeapon = nullptr;
	SecondaryWeapon = nullptr;
	ActiveWeapon = nullptr;

}

//////////////////////////////////////////////////////////////////////////// Input

void AFiringRangeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFiringRangeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFiringRangeCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AFiringRangeCharacter::SwitchWeapon()
{
	if (SecondaryWeapon == nullptr || PrimaryWeapon == nullptr) return;

	if (ActiveWeapon == PrimaryWeapon)
	{
		PrimaryWeapon->SetActorHiddenInGame(true);
		SecondaryWeapon->SetActorHiddenInGame(false);
		ActiveWeapon = SecondaryWeapon;

		SecondaryWeapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GripPoint"));
		PrimaryWeapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("StowPoint"));
	}
	else
	{
		SecondaryWeapon->SetActorHiddenInGame(true);
		PrimaryWeapon->SetActorHiddenInGame(false);
		ActiveWeapon = PrimaryWeapon;

		PrimaryWeapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GripPoint"));
		SecondaryWeapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("StowPoint"));

	}
	SetUpAnim();
}

void AFiringRangeCharacter::ShootActiveWeapon()
{
	if (ActiveWeapon)
	{
		ActiveWeapon->Shoot();
	}
}




void AFiringRangeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFiringRangeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}