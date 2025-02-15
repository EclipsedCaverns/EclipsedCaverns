#include "Character/EclipsedCavernsPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/PlayerAnim.h"
#include "Character/Bullet.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEclipsedCavernsPlayer::AEclipsedCavernsPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/ParagonLtBelica/Characters/Heroes/Belica/Meshes/Belica.Belica'"));
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

	sniperGunComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/SniperGun/sniper11.sniper11'"));
	if (TempSniperMesh.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}
}

void AEclipsedCavernsPlayer::BeginPlay()
{
	Super::BeginPlay();

	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	//±âº»À¸·Î À¯ÅºÃÑ »ç¿ë
	ChangeToGrenadeGun();


	anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
}

void AEclipsedCavernsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
	
}

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
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &AEclipsedCavernsPlayer::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &AEclipsedCavernsPlayer::ChangeToSniperGun);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &AEclipsedCavernsPlayer::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &AEclipsedCavernsPlayer::SniperAim);


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
	//ÃÑ¾Ë ¹ß»ç Ã³¸®

	if (bUsingGrenadeGun)
	{
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}

	else
	{
		FVector startPos = CamComp->GetComponentLocation();
		FVector endPos = CamComp->GetComponentLocation() + CamComp->GetForwardVector() * 5000;
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		if (bHit)
		{
			FTransform bulletTrans;
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);
		}
	}

	/*if (anim)
	{
		anim->PlayBasicAttackAnim();
		UE_LOG(LogTemp, Warning, TEXT("basic attack"));
	}*/
}

void AEclipsedCavernsPlayer::ChangeToGrenadeGun()
{
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void AEclipsedCavernsPlayer::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void AEclipsedCavernsPlayer::SniperAim()
{
	if (bUsingGrenadeGun)
	{
		return;
	}
	if (bSniperAim==false)
	{
		bSniperAim = true;
		_sniperUI->AddToViewport();
		CamComp->SetFieldOfView(45.0f);
	}
	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent();
		CamComp->SetFieldOfView(90.0f);
	}

}

