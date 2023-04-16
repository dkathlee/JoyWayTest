// Description for test project for Joy Way

#pragma once

#include "CoreMinimal.h"
#include "JWBaseWeapon.h"
#include "JWMultiShotWeapon.generated.h"

UCLASS()
class JOYWAYTEST_API AJWMultiShotWeapon : public AJWBaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShoots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float BulletSpread = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 10.0f;
	
	virtual void MakeShoot() override;

private:
	FTimerHandle ShootTimerHandle;
	void MakeDamage(const FHitResult& HitResult);
};
