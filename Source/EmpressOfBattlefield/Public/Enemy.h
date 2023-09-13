// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class EMPRESSOFBATTLEFIELD_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit(const FVector& ImpactPoint) override;	
	
protected:
	virtual void BeginPlay() override;
	
	void PlayHitReactionMontage(const FName& SectionName);
private:

	UPROPERTY(EditDefaultsOnly, Category="Animation | Montage")
	UAnimMontage* HitReactionMontage;
};
