// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyFSM.h"
#include "Character/EclipsedCavernsPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyAnim.h"
#include "Runtime/AIModule/Classes/AIController.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AEclipsedCavernsPlayer::StaticClass());
	target = Cast<AEclipsedCavernsPlayer>(actor);
	me = Cast<AEnemy>(GetOwner());

	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (currentState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;

	}
}

void UEnemyFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime>idleDelayTime)
	{
		currentState = EEnemyState::Move;
		currentTime = 0;

		anim->animState = currentState;
	}
}

void UEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	//me->AddMovementInput(dir.GetSafeNormal());
	ai->MoveToLocation(destination);

	if (dir.Size()<attackRange)
	{
		currentState = EEnemyState::Attack;
		anim->animState = currentState;
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime>attackDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack!"));
		currentTime = 0;
		anim->bAttackPlay = true;
	}

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance>attackRange)
	{
		currentState = EEnemyState::Move;
		anim->animState = currentState;
	}
}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	//UE_LOG(LogTemp, Warning, TEXT("DamageState!"));

	if (currentTime>damageDelayTime)
	{
		currentState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = currentState;
	}
}

void UEnemyFSM::DieState()
{
	//아직 죽음 애니메이션 끝나지 않았다면
	if (anim->bDieDone==false)
	{
		return;
	}
	me->Destroy();

}

void UEnemyFSM::OnDamageProcess()
{
	hp--;

	if (hp>0)
	{
		currentState = EEnemyState::Damage;

		currentTime = 0;

		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	else
	{
		currentState = EEnemyState::Die;

		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		anim->PlayDamageAnim(TEXT("Die"));
	}
	anim->animState = currentState;

}

