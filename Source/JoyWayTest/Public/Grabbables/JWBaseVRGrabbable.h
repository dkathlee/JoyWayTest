// Description for test project for Joy Way

#pragma once

#include "CoreMinimal.h"
#include "Components/JWGrabComponent.h"
#include "GameFramework/Actor.h"
#include "JWBaseVRGrabbable.generated.h"

class USphereComponent;

UCLASS()
class JOYWAYTEST_API AJWBaseVRGrabbable : public AActor
{
	GENERATED_BODY()

public:
	AJWBaseVRGrabbable();
	
	// virtual void BeginInteraction();
	// virtual void EndInteraction();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Grab")
	UJWGrabComponent* GrabComponent;
};
