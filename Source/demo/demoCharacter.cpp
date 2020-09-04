// Copyright Epic Games, Inc. All Rights Reserved.

#include "demoCharacter.h"
#include "demoProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "DrawDebugHelpers.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AdemoCharacter

AdemoCharacter::AdemoCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component·
	FP_Gun = CreateDefaultSubobject<AAcotr>(TEXT("FP_Gun"));
	FP_GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_GunMesh"));
	FP_GunMesh->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_GunMesh->bCastDynamicShadow = false;
	FP_GunMesh->CastShadow = false;
	FP_GunMesh->SetupAttachment(FP_Gun->RootComponent);
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_GunMesh);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	DemageValue = 50;

	//用于vr手柄控制
	/*
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	*/

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.


	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	//vr游戏用的枪
	/*
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.
	*/
	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AdemoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	int32 test = GetController()->GetLinkerIndex();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::FromInt(test), true);
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	//判断是否有vr控制器
	/*if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}*/
}

//////////////////////////////////////////////////////////////////////////
// Input

void AdemoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AdemoCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AdemoCharacter::StopFire);
	
	//Bind throw event
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AdemoCharacter::ThrowGun);

	// Enable touchscreen input
	//EnableTouchscreenMovement(PlayerInputComponent);

	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AdemoCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AdemoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AdemoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AdemoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AdemoCharacter::LookUpAtRate);
}

void AdemoCharacter::ThrowGun()
{

	FP_Gun->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	FP_Gun->SetSimulatePhysics(true);
	FP_Gun->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	FP_Gun = nullptr;
}

void AdemoCharacter::StartFire()
{
	if (FP_Gun == nullptr)
		return;
	OnFire();
	GetWorldTimerManager().SetTimer(MyGunShoot, this, &AdemoCharacter::OnFire, Shootspeed, true);
}


void AdemoCharacter::StopFire()
{
	GetWorldTimerManager().ClearTimer(MyGunShoot);
}

void AdemoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AdemoCharacter::OnFire()
{
	// try and fire a projectile
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		//用于vr游戏适配
		//if (bUsingMotionControllers)
		//{
		//	const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
		//	const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
		//	//World->SpawnActor<AdemoProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		//}
		//else
		//{
		//	//const FRotator SpawnRotation = GetControlRotation();
		//	//// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		//	//const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		//	////Set Spawn Collision Handling Override
		//	//FActorSpawnParameters ActorSpawnParams;
		//	//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		//	//// spawn the projectile at the muzzle
		//	//World->SpawnActor<AdemoProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);	
		//}
		FVector StartPoint = FirstPersonCameraComponent->GetComponentLocation();
		FVector EndPoint = StartPoint + FirstPersonCameraComponent->GetForwardVector() * 20000.f;
		FCollisionQueryParams* TraceParam = new FCollisionQueryParams();
		FHitResult* MyHitResualt = new FHitResult();

		int32 test = 100;

		if (World->LineTraceSingleByChannel(*MyHitResualt, StartPoint, EndPoint, ECC_Visibility, *TraceParam))
		{
			DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Green, false, 4.f);
			AEnemy* Target = Cast<AEnemy>(MyHitResualt->GetActor());


			if (Target != nullptr && !Target->IsPendingKill())
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Target->GetActorLocation().ToString(), false);
				Target->GetDemage(DemageValue);
			}
		}
	}
	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

//void AdemoCharacter::OnResetVR()
//{
//	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
//}

//void AdemoCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if (TouchItem.bIsPressed == true)
//	{
//		return;
//	}
//	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
//	{
//		OnFire();
//	}
//	TouchItem.bIsPressed = true;
//	TouchItem.FingerIndex = FingerIndex;
//	TouchItem.Location = Location;
//	TouchItem.bMoved = false;
//}
//
//void AdemoCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if (TouchItem.bIsPressed == false)
//	{
//		return;
//	}
//	TouchItem.bIsPressed = false;
//}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AdemoCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AdemoCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AdemoCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AdemoCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AdemoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//bool AdemoCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
//{
//	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
//	{
//		//PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AdemoCharacter::BeginTouch);
//		//PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AdemoCharacter::EndTouch);
//
//		//Commenting this out to be more consistent with FPS BP template.
//		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AdemoCharacter::TouchUpdate);
//		return true;
//	}
//	
//	return false;
//}
