// Description for test project for Joy Way


#include "JWGameModeBase.h"
#include "JWBaseVRPawn.h"

AJWGameModeBase::AJWGameModeBase()
{
	DefaultPawnClass = AJWBaseVRPawn::StaticClass();
}
