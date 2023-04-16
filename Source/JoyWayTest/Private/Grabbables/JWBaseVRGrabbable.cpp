// Description for test project for Joy Way

#include "JWBaseVRGrabbable.h"

AJWBaseVRGrabbable::AJWBaseVRGrabbable()
{
	// PrimaryActorTick.bCanEverTick = false;

	GrabComponent = CreateDefaultSubobject<UJWGrabComponent>("GrabComponent");
}

// void AJWBaseVRGrabbable::BeginInteraction() {}
//
// void AJWBaseVRGrabbable::EndInteraction() {}
