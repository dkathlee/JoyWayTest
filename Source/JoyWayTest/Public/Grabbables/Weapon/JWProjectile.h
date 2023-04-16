﻿// Description for test project for Joy Way

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JWProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class JOYWAYTEST_API AJWProjectile : public AActor
{
	GENERATED_BODY()

public:
	AJWProjectile();

	void SetShootDirection(const FVector& Direction) { ShootDirection = Direction; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool DoFullDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float LifeSeconds = 5.0f;

	virtual void BeginPlay() override;

private:
	FVector ShootDirection;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	AController* GetController() const;
};
