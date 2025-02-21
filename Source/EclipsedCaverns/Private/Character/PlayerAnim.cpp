// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnim.h"
#include "Character/EclipsedCavernsPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<AEclipsedCavernsPlayer>(ownerPawn);

	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();
		//speed에 내적 값 할당
		speed = FVector::DotProduct(forwardVector, velocity);

		//좌우 속도 할당
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}

void UPlayerAnim::PlayAttackAnim()
{
	//Montage_Play(attackAnimMontage);
	/*if (IsAnyMontagePlaying())
	{
		UE_LOG(LogTemp, Warning, TEXT("fire attack"));
	}*/

	if (attackAnimMontage && attackAnimMontage->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("Montage 유효! 실행 시도"));
		Montage_Play(attackAnimMontage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Montage가 유효하지 않습니다!"));
	}
	

}

