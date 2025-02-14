#include "Character/EclipsedCavernsPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/PlayerAnim.h"
#include "Character/Bullet.h"

// Sets default values
AEclipsedCavernsPlayer::AEclipsedCavernsPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Skins/PolarStrike/Meshes/Belica_PolarStrike.Belica_PolarStrike'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;

	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->SetupAttachment(springArmComp);
	CamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	JumpMaxCount = 2;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Weapon/GrenadeGun/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}
	
}

// Called when the game starts or when spawned
void AEclipsedCavernsPlayer::BeginPlay()
{
	Super::BeginPlay();

	anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AEclipsedCavernsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
	
}

// Called to bind functionality to input
void AEclipsedCavernsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AEclipsedCavernsPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AEclipsedCavernsPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AEclipsedCavernsPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AEclipsedCavernsPlayer::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AEclipsedCavernsPlayer::InputJump);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AEclipsedCavernsPlayer::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AEclipsedCavernsPlayer::StopSprinting);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AEclipsedCavernsPlayer::InputFire);


}

void AEclipsedCavernsPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void AEclipsedCavernsPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void AEclipsedCavernsPlayer::InputHorizontal(float value)
{

	direction.Y = value;

}


void AEclipsedCavernsPlayer::InputVertical(float value)
{
	direction.X = value;

}

void AEclipsedCavernsPlayer::InputJump()
{
	Jump();
}

void AEclipsedCavernsPlayer::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
}

void AEclipsedCavernsPlayer::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
}


void AEclipsedCavernsPlayer::Move()
{
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;

	
}

void AEclipsedCavernsPlayer::InputFire()
{
	//총알 발사 처리 
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);

	/*if (anim)
	{
		anim->PlayBasicAttackAnim();
		UE_LOG(LogTemp, Warning, TEXT("basic attack"));
	}*/
}

