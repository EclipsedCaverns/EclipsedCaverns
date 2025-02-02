// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EclipsedCavernsPlayer.generated.h"

UCLASS()
class ECLIPSEDCAVERNS_API AEclipsedCavernsPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEclipsedCavernsPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere,Category=Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere,Category=Camera)
	class UCameraComponent* CamComp;

	//플레이어 마우스 조작 
	void Turn(float value);
	void LookUp(float value);

	UPROPERTY(EditAnywhere,Category=PlayerSetting)
	float walkSpeed = 600;

	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
	float SprintSpeedMultiplier = 2.0f;

	FVector direction;
	FRotator NewRotation;

	//플레이어 키 맵핑
	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();
	void Sprint();
	void StopSprinting();

	// 플레이어 이동
	void Move();

	float DefaultWalkSpeed;

};
