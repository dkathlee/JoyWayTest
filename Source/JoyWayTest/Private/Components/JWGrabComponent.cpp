// Description for test project for Joy Way


#include "Components/JWGrabComponent.h"

#include "ComponentUtils.h"
#include "JWBaseVRGrabbable.h"
#include "MotionControllerComponent.h"


UJWGrabComponent::UJWGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UJWGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	ParentComponent = Cast<UPrimitiveComponent>(GetAttachParent());
	
	check(ParentComponent);

	bSimulatePhysics = ParentComponent->IsAnySimulatingPhysics();
}

bool UJWGrabComponent::Grab(UMotionControllerComponent* MotionController) const
{
	ParentComponent->SetSimulatePhysics(false);

	if (!ParentComponent->AttachToComponent(MotionController, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false)))
		return false;
	
	ParentComponent->SetRelativeRotation(GetRelativeRotation().GetInverse());
	ParentComponent->SetWorldLocation(MotionController->GetComponentLocation() + (GetComponentLocation() - ParentComponent->GetComponentLocation()) * -1);

	OnGrabbed.Broadcast();
	return true;
}

void UJWGrabComponent::Release() const
{
	ParentComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ParentComponent->SetSimulatePhysics(bSimulatePhysics);
	OnReleased.Broadcast();
}

void UJWGrabComponent::BeginInteraction() const
{
	OnInteractionStarted.Broadcast();
	// if (GetOwner()->IsA(AJWBaseVRGrabbable::StaticClass()))
	// 	Cast<AJWBaseVRGrabbable>(GetOwner())->BeginInteraction();
}

void UJWGrabComponent::EndInteraction() const
{
	OnInteractionFinished.Broadcast();
	// if (GetOwner()->IsA(AJWBaseVRGrabbable::StaticClass()))
	// 	Cast<AJWBaseVRGrabbable>(GetOwner())->EndInteraction();
}
