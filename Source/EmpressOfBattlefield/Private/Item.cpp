#include "Item.h"

#include "SlashCharacter.h"
#include "Components/SphereComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(GetRootComponent());
	
	RunningTime = 0;
	SinResult = 0;
	OffsetMultiplier = 0.2f;
	OffsetSpeed = 1.5f;

	IsItemTaken = false;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AItem::OverlappingBegin);
	SphereCollider->OnComponentEndOverlap.AddDynamic(this, &AItem::OverlappingEnd);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsItemTaken)
	{
		SinusoidalMovement();
	}
}

void AItem::SinusoidalMovement()
{
	if (GetWorld())
	{
		RunningTime += GetWorld()->GetDeltaSeconds();
		SinResult = FMath::Sin(RunningTime * OffsetSpeed);
		AddActorLocalOffset(FVector(0.f, 0.f, SinResult * OffsetMultiplier));
		AddActorLocalRotation(FRotator(0.f, 1.5f, 0.f));
	}
}

void AItem::OverlappingBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* MainPlayer = Cast<ASlashCharacter>(OtherActor);

	if (MainPlayer)
	{
		MainPlayer->SetOverlappingItem(this);
	}
}

void AItem::OverlappingEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASlashCharacter* MainPlayer = Cast<ASlashCharacter>(OtherActor);

	if (MainPlayer)
	{
		MainPlayer->SetOverlappingItem(nullptr);
	}
}
