// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class ECLIPSEDCAVERNS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//발사체의 이동을 담당할 컴포넌트
	UPROPERTY(VisibleAnywhere, Category=Movement)
	class UProjectileMovementComponent* movementComp;
	//충돌 컴포넌트
	UPROPERTY(VisibleAnywhere,Category=Collision)
	class USphereComponent* collisionComp;
	//외관 컴포넌트
	UPROPERTY(VisibleAnywhere,Category=BodyMesh)
	class UStaticMeshComponent* bodyMeshComp;
};
