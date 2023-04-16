// Description for test project for Joy Way


#include "Components/JWHealthComponent.h"

UJWHealthComponent::UJWHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UJWHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UJWHealthComponent::OnTakeAnyDamage);
	}
}

void UJWHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld())
		return;

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UJWHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}

void UJWHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if (IsHealthFull() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UJWHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

bool UJWHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull())
		return false;

	SetHealth(Health + HealthAmount);
	return true;
}

bool UJWHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void UJWHealthComponent::Reset()
{
	SetHealth(MaxHealth);
}
