// Description for test project for Joy Way

#pragma once

#include "CoreMinimal.h"
#include "JWCoreTypes.h"
#include "JWGrabComponent.generated.h"

class UMotionControllerComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JOYWAYTEST_API UJWGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UJWGrabComponent();

	FOnGrabbedSignature OnGrabbed;
	FOnReleasedSignature OnReleased;
	FOnInteractionStartedSignature OnInteractionStarted;
	FOnInteractionFinishedSignature OnInteractionFinished;

	bool Grab(UMotionControllerComponent* MotionController) const;
	void Release() const;
	void BeginInteraction() const;
	void EndInteraction() const;

protected:
	UPROPERTY()
	UPrimitiveComponent* ParentComponent;
	
	virtual void BeginPlay() override;

private:
	bool bSimulatePhysics;
};
