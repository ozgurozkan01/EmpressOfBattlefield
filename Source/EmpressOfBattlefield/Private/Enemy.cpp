// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/CapsuleComponent.h"
#include "EmpressOfBattlefield/DebugMacros.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic); // Weapon does not overlap with Pawns
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // Weapon trace channel
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // prevent breakdown of player's camera when collide with enemy
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // same
	GetMesh()->SetGenerateOverlapEvents(true); // Activate overlap event generation
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAW_DEBUG_SPHERE(ImpactPoint);
}

