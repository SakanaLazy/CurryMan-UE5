#include "HitDetector_Prototype.h"
#include "Components/ShapeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "HealthSystem_Prototype.h"
#include "AudioManager_Prototype.h"

int32 UHitDetector_Prototype::SuccessfulHits = 0;
FOnHitLanded UHitDetector_Prototype::OnHitLanded;

UHitDetector_Prototype::UHitDetector_Prototype()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHitDetector_Prototype::BeginPlay()
{
	Super::BeginPlay();

	OwnerRoot = GetOwner();
	if (!OwnerRoot) return;

	Hitbox = OwnerRoot->FindComponentByClass<UShapeComponent>();
	if (Hitbox)
	{
		Hitbox->SetGenerateOverlapEvents(true);
		Hitbox->OnComponentBeginOverlap.AddDynamic(
			this,
			&UHitDetector_Prototype::OnOverlapBegin
		);
		Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	MeshRef = OwnerRoot->FindComponentByClass<USkeletalMeshComponent>();
	if (MeshRef)
	{
		AnimInstance = MeshRef->GetAnimInstance();
	}

	RebuildStateHashes();
}

void UHitDetector_Prototype::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Hitbox)
	{
		Hitbox->OnComponentBeginOverlap.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void UHitDetector_Prototype::RebuildStateHashes()
{
	AttackStateHashes.Empty();
	for (const FString& Name : AttackStateNames)
	{
		AttackStateHashes.Add(GetTypeHash(Name));
	}
}

void UHitDetector_Prototype::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!AnimInstance || !Hitbox) return;

	bool bShouldEnable = IsInAttackWindow();

	Hitbox->SetCollisionEnabled(
		bShouldEnable
		? ECollisionEnabled::QueryOnly
		: ECollisionEnabled::NoCollision
	);
}

bool UHitDetector_Prototype::IsInAttackWindow() const
{
	if (!AnimInstance) return false;

	const FAnimInstanceProxy& Proxy = AnimInstance->GetProxyOnAnyThread<FAnimInstanceProxy>();
	return MatchesAttackStateAndTime(Proxy);
}

bool UHitDetector_Prototype::MatchesAttackStateAndTime(const FAnimInstanceProxy& Proxy) const
{
	const FAnimMontageInstance* Montage = Proxy.GetActiveMontageInstance();
	if (!Montage) return false;

	const float NormalizedTime = FMath::Fmod(Montage->GetPosition() / Montage->Montage->GetPlayLength(), 1.f);

	if (ActiveStartNormalized <= ActiveEndNormalized)
	{
		return NormalizedTime >= ActiveStartNormalized &&
			NormalizedTime <= ActiveEndNormalized;
	}

	return true;
}

void UHitDetector_Prototype::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == OwnerRoot) return;

	bool bValidTarget =
		(bIsPlayerAttack && OtherActor->ActorHasTag("Enemy")) ||
		(!bIsPlayerAttack && OtherActor->ActorHasTag("Player"));

	if (!bValidTarget) return;

	UHealthSystem_Prototype* Health =
		OtherActor->FindComponentByClass<UHealthSystem_Prototype>();

	if (!Health || Health->bIsDead) return;

	Health->TakeDamage(Damage);

	if (UAudioManager_Prototype::Instance)
	{
		UAudioManager_Prototype::Instance->PlayHit();
	}

	if (bIsPlayerAttack)
	{
		SuccessfulHits++;
		OnHitLanded.Broadcast(SuccessfulHits);
	}
}
