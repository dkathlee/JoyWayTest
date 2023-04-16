// Description for test project for Joy Way


#include "JWSingleShotWeapon.h"
#include "JWProjectile.h"

void AJWSingleShotWeapon::StartFire()
{
	MakeShoot();
}

void AJWSingleShotWeapon::MakeShoot()
{
	if (!GetWorld() || IsAmmoEmpty())
		return;

	const FTransform SpawnTransform(FRotator::ZeroRotator, MeshComponent->GetSocketLocation(MuzzleSocketName));
	AJWProjectile* Projectile = GetWorld()->SpawnActorDeferred<AJWProjectile>(ProjectileClass, SpawnTransform, this);
	if (Projectile)
	{
		Projectile->SetShootDirection(MeshComponent->GetSocketRotation(MuzzleSocketName).Vector());
		Projectile->FinishSpawning(SpawnTransform);
	}

	DecreaseAmmo();
}

