// Description for test project for Joy Way


#include "Dev/JWDamageableTarget.h"

#include "Components/JWHealthComponent.h"
#include "Components/TextRenderComponent.h"

AJWDamageableTarget::AJWDamageableTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	SetRootComponent(StaticMesh);

	HealthComponent = CreateDefaultSubobject<UJWHealthComponent>("Health Component");
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("Health Text");
	HealthTextComponent->SetupAttachment(StaticMesh);
}

void AJWDamageableTarget::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	check(HealthTextComponent);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &AJWDamageableTarget::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AJWDamageableTarget::OnHealthChanged);
}

void AJWDamageableTarget::Respawn()
{
	SetActorHiddenInGame(false);
	HealthComponent->Reset();
}

void AJWDamageableTarget::OnDeath()
{
	SetActorHiddenInGame(true);
	
	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AJWDamageableTarget::Respawn, RespawnTime);
}

void AJWDamageableTarget::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}
