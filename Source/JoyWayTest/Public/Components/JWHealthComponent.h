// Description for test project for Joy Way

#pragma once

#include "CoreMinimal.h"
#include "JWCoreTypes.h"
#include "Components/ActorComponent.h"
#include "JWHealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JOYWAYTEST_API UJWHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UJWHealthComponent();

	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	float GetHealth() const { return Health; }
	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;
	void Reset();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Healht", meta = (ClampMin = "25.0", ClampMax = "1000.0"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal", ClampMin = "0.01", ClampMax = "10.0"))
	float HealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal", ClampMin = "0.0", ClampMax = "10.0"))
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal", ClampMin = "0.0", ClampMax = "100.0"))
	float HealModifier = 5.0f;

	virtual void BeginPlay() override;

private:
	float Health = 0.0f;
	FTimerHandle HealTimerHandle;

	UFUNCTION()
	void OnTakeAnyDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, class AController *InstigatedBy, AActor *DamageCauser);

	void HealUpdate();
	void SetHealth(float NewHealth);
};
