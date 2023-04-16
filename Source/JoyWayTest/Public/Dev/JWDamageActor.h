// Description for test project for Joy Way

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JWDamageActor.generated.h"

UCLASS()
class JOYWAYTEST_API AJWDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AJWDamageActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor SphereColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DoFullDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType;
	
	virtual void Tick(float DeltaTime) override;
};
