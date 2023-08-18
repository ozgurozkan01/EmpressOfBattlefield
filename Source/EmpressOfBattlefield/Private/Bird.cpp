#include "Bird.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimationAsset.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	BirdMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	BirdMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	
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

	//BirdMeshComponent->SetAnimation(BirdFlyAnimation);
	BirdMeshComponent->PlayAnimation(BirdFlyAnimation, true);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}
}

void ABird::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();

	if (Controller && DirectionValue != 0)
	{
		FVector ForwardVector = GetActorForwardVector();
		AddMovementInput(ForwardVector, DirectionValue);
	}
}

void ABird::Look(const FInputActionValue& Value)
{
	const FVector2d LookDirection = Value.Get<FVector2D>();

	if (Controller == nullptr)
	{
		return;
	}

	AddControllerYawInput(LookDirection.X);
	AddControllerPitchInput(LookDirection.Y);
}