#include "Weapon.h"
#include "HitInterface.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Field/FieldSystemComponent.h"
#include "Particles/ParticleSystemComponent.h"

AWeapon::AWeapon()
{
	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Box"));
	DamageBox->SetupAttachment(GetRootComponent());
	DamageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	DamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Trace Start"));
	TraceStart->SetupAttachment(GetRootComponent());
	
	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Trace End"));
	TraceEnd->SetupAttachment(GetRootComponent());

	FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("Field System Component"));
	FieldSystemComponent->SetupAttachment(GetRootComponent());
	
	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("Radial Falloff"));
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("Radial Vector"));
	MetaDataFilter = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("Meta Data Filter"));
	
	FalloffMagnitude = 1500000000.f;
	VectorMagnitude = 50000000000.f;
	SphereRadius = 200.f;
	Damage = 20.f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));
	
	MetaDataFilter->ObjectType = EFieldObjectType::Field_Object_Destruction;
	MetaDataFilter->FilterType = EFieldFilterType::Field_Filter_Dynamic;
	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnDamageBoxOverlapBegin);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	IsItemTaken = true;
	AttachMeshToSocket(InParent, InSocketName);

	if (UnpickUpParticle)
	{
		UnpickUpParticle->Deactivate();
	}
	
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
	
	if (SphereCollider)
	{
		SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::CreateFields(const FVector& FieldLocation)
{
	// Min and Max Range is just a number which are multiplied by the Force.
	// So before the determine these values, watch out that.
	RadialFalloff->SetRadialFalloff(FalloffMagnitude,0.8f, 1.f, 0.f, SphereRadius, FieldLocation, EFieldFalloffType::Field_FallOff_None);	
	RadialVector->SetRadialVector(VectorMagnitude, FieldLocation);

	if (FieldSystemComponent)
	{
		if (RadialFalloff)
		{
			FieldSystemComponent->ApplyPhysicsField(true, Field_ExternalClusterStrain, nullptr, RadialFalloff);
		}

		if (RadialVector)
		{
			FieldSystemComponent->ApplyPhysicsField(true, Field_LinearForce, MetaDataFilter, RadialVector);
		}
	}
}

bool AWeapon::ActorHasSameTag(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag("Enemy") && OtherActor->ActorHasTag("Enemy");
}

void AWeapon::OnDamageBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorHasSameTag(OtherActor)) return;
	
	FHitResult BoxHit;
	BoxTrace(BoxHit);
	
	if (BoxHit.GetActor())
	{
		// When actor is hitting the different type object , the actor has same tag at nearby the actor which is hitting,
		// can effect. Because the collision on the weapon might not be deactivated.
		// Because of that we check the hit actor
		if (ActorHasSameTag(BoxHit.GetActor())) return; 
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		// if hit object inherites from IHitInterface, then call the methods,
		// But this method belongs to hit objects own. So we do not need to cast
		// specific classes like actor, character etc.
		ExecuteGetHit(BoxHit);
		CreateFields(BoxHit.ImpactPoint);
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceEnd->GetComponentLocation();
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(2.5f, 2.5f, 2.5f),
		TraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
		BoxHit,
		true
		);

	IgnoredActors.AddUnique(BoxHit.GetActor());
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AttachmentTransformRules, InSocketName);	
}
