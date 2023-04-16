// Description for test project for Joy Way

#pragma once

#include "CoreMinimal.h"
#include "JWBaseWeapon.h"
#include "JWSingleShotWeapon.generated.h"

class AJWProjectile;

UCLASS()
class JOYWAYTEST_API AJWSingleShotWeapon : public AJWBaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AJWProjectile> ProjectileClass;

	virtual void MakeShoot() override;
};
