// Description for test project for Joy Way

#pragma once

#include "CoreMinimal.h"
#include "JWBaseVRGrabbable.h"
#include "JWCoreTypes.h"
#include "Components/TextRenderComponent.h"
#include "JWBaseWeapon.generated.h"

UCLASS()
class JOYWAYTEST_API AJWBaseWeapon : public AJWBaseVRGrabbable
{
	GENERATED_BODY()

public:
	AJWBaseWeapon();
	
	void ChangeClip();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Visual")
	USkeletalMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Visual")
	UTextRenderComponent* AmmoTextRenderComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance = 5000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoData DefaultAmmo{15, 10, false};

	virtual void BeginPlay() override;

	virtual void StartFire();
	virtual void StopFire();
	virtual void MakeShoot();

	UFUNCTION()
	void OnGrabbed();

	UFUNCTION()
	void OnReleased();
	
	UFUNCTION()
	void OnInteractionStarted();
	
	UFUNCTION()
	void OnInteractionFinished();

	APlayerController* GetPlayerController() const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	
	void DecreaseAmmo();
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	bool IsAmmoFull() const;

private:
	FAmmoData CurrentAmmo;
};
