// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	CapsuleCollider->SetCapsuleRadius(15.f);
	CapsuleCollider->SetCapsuleHalfHeight(20.f);
	SetRootComponent(CapsuleCollider);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &ABird::MoveForward);
}

void ABird::MoveForward(float ScaleValue)
{
	if ( (Controller != nullptr) && (ScaleValue != 0.f))
	{
		FVector ForwardVector = GetActorForwardVector(); 
		AddMovementInput(ForwardVector, ScaleValue);
	}
}
