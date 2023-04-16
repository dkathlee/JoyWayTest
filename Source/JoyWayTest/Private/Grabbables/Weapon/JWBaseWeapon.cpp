// Description for test project for Joy Way


#include "JWBaseWeapon.h"

#include "Components/SlateWrapperTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

inline void AJWBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count coudn`t be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count coudn`t be less or equal zero"));

	CurrentAmmo = DefaultAmmo;

	AmmoTextRenderComponent->SetVisibility(false);
	AmmoTextRenderComponent->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo.Bullets, CurrentAmmo.Clips)));
	
	GrabComponent->OnGrabbed.AddUObject(this, &AJWBaseWeapon::OnGrabbed);
	GrabComponent->OnReleased.AddUObject(this, &AJWBaseWeapon::OnReleased);
	GrabComponent->OnInteractionStarted.AddUObject(this, &AJWBaseWeapon::OnInteractionStarted);
	GrabComponent->OnInteractionFinished.AddUObject(this, &AJWBaseWeapon::OnInteractionFinished);
}

AJWBaseWeapon::AJWBaseWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh Component");
	SetRootComponent(MeshComponent);
	
	AmmoTextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>("AmmoTextRenderComponent");
	AmmoTextRenderComponent->SetupAttachment(MeshComponent);

	GrabComponent->SetupAttachment(MeshComponent);
}

void AJWBaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.Infinite)
	{
		if (CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Display, TEXT("No more clips!"));
			return;
		}
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

void AJWBaseWeapon::StartFire() {}

void AJWBaseWeapon::StopFire() {}

void AJWBaseWeapon::MakeShoot() {}

void AJWBaseWeapon::OnGrabbed()
{
	AmmoTextRenderComponent->SetVisibility(true);
}

void AJWBaseWeapon::OnReleased()
{
	AmmoTextRenderComponent->SetVisibility(false);
}

void AJWBaseWeapon::OnInteractionStarted()
{
	StartFire();
}

void AJWBaseWeapon::OnInteractionFinished()
{
	StopFire();
}

APlayerController* AJWBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<APawn>(GetOwner());
	if (!Player)
		return nullptr;

	return Player->GetController<APlayerController>();
}

void AJWBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	if (!GetWorld())
		return;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void AJWBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Display, TEXT("Clip is empty!"));
		AmmoTextRenderComponent->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo.Bullets, CurrentAmmo.Clips)));
		return;
	}
	CurrentAmmo.Bullets--;

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		ChangeClip();
	}
	AmmoTextRenderComponent->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo.Bullets, CurrentAmmo.Clips)));
}

bool AJWBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool AJWBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

bool AJWBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}
