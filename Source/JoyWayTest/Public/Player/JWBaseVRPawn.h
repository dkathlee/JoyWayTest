// Description for test project for Joy Way

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "JWBaseVRPawn.generated.h"

class UCapsuleComponent;
class UJWHealthComponent;
class UFloatingPawnMovement;
class UWidgetInteractionComponent;
class UJWGrabComponent;
class UMotionControllerComponent;
class UCameraComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class JOYWAYTEST_API AJWBaseVRPawn : public APawn
{
	GENERATED_BODY()
	
public:
	AJWBaseVRPawn(const FObjectInitializer& ObjInit);

protected:
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	// USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UMotionControllerComponent* MotionControllerLeft;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UMotionControllerComponent* MotionControllerRight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UMotionControllerComponent* MotionControllerLeftAim;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UMotionControllerComponent* MotionControllerRightAim;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetInteractionComponent* WidgetInteractionRight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetInteractionComponent* WidgetInteractionLeft;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UFloatingPawnMovement* MovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UJWHealthComponent* HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grabbing", meta = (ClampMin = "0.0"))
	float GrabRadius = 5.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Locomotion", meta = (ClampMin = "5.0", ClampMax = "180.0"))
	float SnapTurnDegrees = 45.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Locomotion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SnapTurnDeadzone = 0.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Locomotion")
	float SpeedMultiplier = 1.0f;

	UPROPERTY()
	UJWGrabComponent* LeftHeldComponent;
	UPROPERTY()
	UJWGrabComponent* RightHeldComponent;
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void OnDeath();
	void TryGrab(bool Right);
	void TryRelease(bool Right);
	void GrabLeft();
	void GrabRight();
	void ReleaseLeft();
	void ReleaseRight();
	void TriggerPressedLeft();
	void TriggerPressedRight();
	void TriggerReleasedLeft();
	void TriggerReleasedRight();
	void MovementAxisLeftX(float AxisValue);
	void MovementAxisRightX(float AxisValue);
	void MovementAxisRightY(float AxisValue);
	void SnapTurn(bool Clockwise);

private:
	bool bCanSnapTurn = true;
};
