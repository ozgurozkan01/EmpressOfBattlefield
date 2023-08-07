#include "Bird.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimationAsset.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	CapsuleCollider->SetCapsuleRadius(15.f);
	CapsuleCollider->SetCapsuleHalfHeight(20.f);
	SetRootComponent(CapsuleCollider);
	CapsuleCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleCollider->SetCollisionResponseToAllChannels(ECR_Block);
	
	
	BirdMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMeshComponent->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BirdMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimalVarietyPack/Crow/Meshes/SK_Crow.SK_Crow'"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> BirdAnimation(TEXT("/Script/Engine.AnimSequence'/Game/AnimalVarietyPack/Crow/Animations/ANIM_Crow_Fly.ANIM_Crow_Fly'"));

	BirdFlyAnimation = BirdAnimation.Object;
	
	if (BirdMesh.Succeeded() && BirdMeshComponent)
	{
		BirdMeshComponent->SetSkeletalMesh(BirdMesh.Object);
		BirdMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator::ZeroRotator);

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

		UE_LOG(LogTemp, Warning, TEXT("Animation loaded successfully"));
		//BirdMeshComponent->SetAnimation(BirdFlyAnimation);
		BirdMeshComponent->PlayAnimation(BirdFlyAnimation, true);
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Forward"), this, &ABird::MoveForward);
	PlayerInputComponent->BindAxis(FName("TurnRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &APawn::AddControllerPitchInput);
}

void ABird::MoveForward(float ScaleValue)
{
	if ( (Controller != nullptr) && (ScaleValue != 0.f))
	{
		FVector ForwardVector = GetActorForwardVector(); 
		AddMovementInput(ForwardVector, ScaleValue);
	}
	
}