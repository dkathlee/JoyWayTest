// Description for test project for Joy Way


#include "JWMultiShotWeapon.h"
#include "DrawDebugHelpers.h"

void AJWMultiShotWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &AJWMultiShotWeapon::MakeShoot, TimeBetweenShoots, true);
	MakeShoot();
}

void AJWMultiShotWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

void AJWMultiShotWeapon::MakeShoot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	const FVector TraceStart = MeshComponent->GetSocketLocation(MuzzleSocketName);
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(MeshComponent->GetSocketRotation(MuzzleSocketName).Vector(), HalfRad);
	const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	DrawDebugLine(GetWorld(), TraceStart, HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	MakeDamage(HitResult);

	DecreaseAmmo();
}

void AJWMultiShotWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor)
		return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}