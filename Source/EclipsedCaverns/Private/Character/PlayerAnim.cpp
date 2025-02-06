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
		//speed�� ���� �� �Ҵ�
		speed = FVector::DotProduct(forwardVector, velocity);

		//�¿� �ӵ� �Ҵ�
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}

void UPlayerAnim::PlayBasicAttackAnim()
{
	Montage_Play(basicAttackAnimMontage);

}

void UPlayerAnim::PlayMeleeAttackAnim()
{
	Montage_Play(meleeAttackAnimMontage);
}
