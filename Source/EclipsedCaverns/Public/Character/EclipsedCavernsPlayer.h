// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerAnim.h"
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
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=Camera)
	class UCameraComponent* CamComp;

	//�÷��̾� ���콺 ���� 
	void Turn(float value);
	void LookUp(float value);

	UPROPERTY(EditAnywhere,Category=PlayerSetting)
	float walkSpeed = 600;

	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
	float SprintSpeedMultiplier = 2.0f;

	FVector direction;

	//�÷��̾� �̵� ����
	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();
	void Sprint();
	void StopSprinting();

	class UPlayerAnim* anim;

	//------------------------Move------------------------------
	void Move();

	//-----------------------Attack-----------------------------
	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(VisibleAnywhere,Category=GunMesh)
	class UStaticMeshComponent* sniperGunComp;

	UPROPERTY(EditDefaultsOnly,Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	void InputFire();

	//��ź�� ��� ����
	bool bUsingGrenadeGun = true;
	//��ź������ ����
	void ChangeToGrenadeGun();
	//�������۰����� ����
	void ChangeToSniperGun();
	//�������� ����
	void SniperAim();

	bool bSniperAim = false;

	UPROPERTY(EditDefaultsOnly,Category=SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	UPROPERTY()
	class UUserWidget* _sniperUI;

	UPROPERTY(EditAnywhere, Category=BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	UPROPERTY(EditDefaultsOnly,Category=SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	UPROPERTY()
	class UUserWidget* _crosshairUI;
	
};
