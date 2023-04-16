// Description for test project for Joy Way


#include "JWBaseVRPawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/JWGrabComponent.h"
#include "Components/JWHealthComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AJWBaseVRPawn::AJWBaseVRPawn(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	// SetRootComponent(DefaultSceneRoot);
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, CapsuleComponent->GetScaledCapsuleHalfHeight()));
	CapsuleComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(RootComponent);
	
	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>("MotionControllerLeft");
	MotionControllerLeft->MotionSource = "Left";
	MotionControllerLeft->SetupAttachment(RootComponent);
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>("MotionControllerRight");
	MotionControllerRight->MotionSource = "Right";
	MotionControllerRight->SetupAttachment(RootComponent);
	MotionControllerLeftAim = CreateDefaultSubobject<UMotionControllerComponent>("MotionControllerLeftAim");
	MotionControllerLeftAim->MotionSource = "LeftAim";
	MotionControllerLeftAim->SetupAttachment(RootComponent);
	MotionControllerRightAim = CreateDefaultSubobject<UMotionControllerComponent>("MotionControllerRightAim");
	MotionControllerLeftAim->MotionSource = "RightAim";
	MotionControllerRightAim->SetupAttachment(RootComponent);
	
	WidgetInteractionLeft = CreateDefaultSubobject<UWidgetInteractionComponent>("WidgetInteractionLeft");
	WidgetInteractionLeft->SetupAttachment(MotionControllerLeftAim);
	WidgetInteractionLeft->TraceChannel = ECC_GameTraceChannel1;
	WidgetInteractionRight = CreateDefaultSubobject<UWidgetInteractionComponent>("WidgetInteractionRight");
	WidgetInteractionRight->SetupAttachment(MotionControllerRightAim);
	WidgetInteractionRight->TraceChannel = ECC_GameTraceChannel1;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	HealthComponent = CreateDefaultSubobject<UJWHealthComponent>("HealthComponent");
}

void AJWBaseVRPawn::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	// check(GetCharacterMovement());

	HealthComponent->OnDeath.AddUObject(this, &AJWBaseVRPawn::OnDeath);

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

void AJWBaseVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MovementAxisLeft_X", this, &AJWBaseVRPawn::MovementAxisLeftX);
	PlayerInputComponent->BindAxis("MovementAxisRight_X", this, &AJWBaseVRPawn::MovementAxisRightX);
	PlayerInputComponent->BindAxis("MovementAxisRight_Y", this, &AJWBaseVRPawn::MovementAxisRightY);
	PlayerInputComponent->BindAction("GrabLeft", IE_Pressed, this, &AJWBaseVRPawn::GrabLeft);
	PlayerInputComponent->BindAction("GrabLeft", IE_Released, this, &AJWBaseVRPawn::ReleaseLeft);
	PlayerInputComponent->BindAction("GrabRight", IE_Pressed, this, &AJWBaseVRPawn::GrabRight);
	PlayerInputComponent->BindAction("GrabRight", IE_Released, this, &AJWBaseVRPawn::ReleaseRight);
	PlayerInputComponent->BindAction("TriggerLeft", IE_Pressed, this, &AJWBaseVRPawn::TriggerPressedLeft);
	PlayerInputComponent->BindAction("TriggerLeft", IE_Released, this, &AJWBaseVRPawn::TriggerReleasedLeft);
	PlayerInputComponent->BindAction("TriggerRight", IE_Pressed, this, &AJWBaseVRPawn::TriggerPressedRight);
	PlayerInputComponent->BindAction("TriggerRight", IE_Released, this, &AJWBaseVRPawn::TriggerReleasedRight);
}

void AJWBaseVRPawn::OnDeath()
{
	if (!GetWorld())
		return;
	
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	if (PlayerStarts.Num() == 0)
		return;

	SetActorLocationAndRotation(PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation());
	HealthComponent->Reset();
}

void AJWBaseVRPawn::TryGrab(bool Right)
{
	const auto MotionController = Right ? MotionControllerRight : MotionControllerLeft;
	const auto MotionControllerLocation = MotionController->GetComponentLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody)
	};
	FHitResult HitResult;
	
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(this,
	                                                 MotionControllerLocation,
	                                                 MotionControllerLocation,
	                                                 GrabRadius,
	                                                 ObjectTypes,
	                                                 false,
	                                                 TArray<AActor*>{},
	                                                 EDrawDebugTrace::None,
	                                                 HitResult,
	                                                 true))
	{
		UJWGrabComponent* NearestGrabComponent = nullptr;
		float NearestDistance = TNumericLimits<float>::Max();

		TArray<UJWGrabComponent*> GrabComponents;
		HitResult.Actor->GetComponents<UJWGrabComponent>(GrabComponents);
		for (const auto GrabComponent : GrabComponents)
		{
			const float Distance = (GrabComponent->GetComponentLocation() - MotionControllerLocation).Size();
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestGrabComponent = GrabComponent;
			}
		}
		
		if (NearestGrabComponent != nullptr)
		{
			if (NearestGrabComponent->Grab(MotionController))
			{
				if (Right)
				{
					RightHeldComponent = NearestGrabComponent;
					if (LeftHeldComponent == RightHeldComponent)
						LeftHeldComponent = nullptr;
				}
				else
				{
					LeftHeldComponent = NearestGrabComponent;
					if (RightHeldComponent == LeftHeldComponent)
						RightHeldComponent = nullptr;
				}
			}
		}
	}
}

void AJWBaseVRPawn::TryRelease(bool Right)
{
	if (Right)
	{
		if (IsValid(RightHeldComponent))
		{
			RightHeldComponent->Release();
			RightHeldComponent = nullptr;
		}
	}
	else
	{
		if (IsValid(LeftHeldComponent))
		{
			LeftHeldComponent->Release();
			LeftHeldComponent = nullptr;
		}
	}
}

void AJWBaseVRPawn::GrabLeft()
{
	TryGrab(false);
}

void AJWBaseVRPawn::GrabRight()
{
	TryGrab(true);
}

void AJWBaseVRPawn::ReleaseLeft()
{
	TryRelease(false);
}

void AJWBaseVRPawn::ReleaseRight()
{
	TryRelease(true);
}

void AJWBaseVRPawn::TriggerPressedLeft()
{
	if (IsValid(LeftHeldComponent))
		LeftHeldComponent->BeginInteraction();
}

void AJWBaseVRPawn::TriggerPressedRight()
{
	if (IsValid(RightHeldComponent))
		RightHeldComponent->BeginInteraction();
}

void AJWBaseVRPawn::TriggerReleasedLeft()
{
	if (IsValid(LeftHeldComponent))
		LeftHeldComponent->EndInteraction();
}

void AJWBaseVRPawn::TriggerReleasedRight()
{
	if (IsValid(RightHeldComponent))
		RightHeldComponent->EndInteraction();
}

void AJWBaseVRPawn::MovementAxisLeftX(float AxisValue)
{
	if (FMath::Abs(AxisValue) > SnapTurnDeadzone && bCanSnapTurn)
	{
		SnapTurn(AxisValue > 0);
		bCanSnapTurn = false;
	}
	else if (FMath::Abs(AxisValue) < SnapTurnDeadzone && !bCanSnapTurn)
	{
		bCanSnapTurn = true;
	}
}

void AJWBaseVRPawn::MovementAxisRightX(float AxisValue)
{
	auto CameraRightVector = CameraComponent->GetRightVector();
	AddMovementInput(FVector(CameraRightVector.X, CameraRightVector.Y, 0), AxisValue * SpeedMultiplier);
}

void AJWBaseVRPawn::MovementAxisRightY(float AxisValue)
{
	auto CameraForwardVector = CameraComponent->GetForwardVector();
	AddMovementInput(FVector(CameraForwardVector.X, CameraForwardVector.Y, 0), AxisValue * SpeedMultiplier);
}

void AJWBaseVRPawn::SnapTurn(bool Clockwise)
{
	const auto CameraWorldLocation = CameraComponent->GetComponentLocation();
	const auto CameraRelativeTransform = CameraComponent->GetRelativeTransform();
	const auto SnapTurnDegreesSigned = FMath::Abs(SnapTurnDegrees) * (Clockwise ? 1 : -1);
	const auto NewTransform = FTransform(FRotator(FQuat(GetActorRotation())*FQuat(FRotator(0.0f, SnapTurnDegreesSigned, 0.0f))), GetActorLocation());

	AddActorWorldRotation(FRotator(0.0f, SnapTurnDegreesSigned, 0.0f));
	SetActorLocation(CameraWorldLocation - (CameraRelativeTransform * NewTransform).GetLocation() + GetActorLocation());
}
