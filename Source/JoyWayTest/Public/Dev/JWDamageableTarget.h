// Description for test project for Joy Way

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JWDamageableTarget.generated.h"

class UJWHealthComponent;
class UTextRenderComponent;

UCLASS()
class JOYWAYTEST_API AJWDamageableTarget : public AActor
{
	GENERATED_BODY()

public:
	AJWDamageableTarget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UJWHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damagable Target", meta = (ClampMin = "0.0"))
	float RespawnTime = 10.0f;

	virtual void BeginPlay() override;

private:
	void Respawn();
	
	void OnDeath();
	void OnHealthChanged(float Health);
};
