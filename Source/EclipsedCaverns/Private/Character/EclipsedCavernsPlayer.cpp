#include "Character/EclipsedCavernsPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// ������
AEclipsedCavernsPlayer::AEclipsedCavernsPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    // �⺻ �̵� �ӵ� ����
    DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

    // ���̷�Ż �޽� ���� (���� �ε�)
    ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Skins/FrozenHearth/Meshes/Aurora_FrozenHearth.Aurora_FrozenHearth'"));
    if (TempMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(TempMesh.Object);
        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
    }

    // SpringArm ����
    springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    springArmComp->SetupAttachment(RootComponent);
    springArmComp->SetRelativeLocation(FVector(0, 70, 90));
    springArmComp->TargetArmLength = 400;
    springArmComp->bUsePawnControlRotation = true;  // ī�޶� ȸ�� ����

    // ī�޶� ����
    CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
    CamComp->SetupAttachment(springArmComp);
    CamComp->bUsePawnControlRotation = false; // ī�޶� ȸ�� X

    // ĳ���� ȸ�� ����
    bUseControllerRotationYaw = true;

    // ���� ����
    JumpMaxCount = 1;

    // �ʱ� ȸ����
    NewRotation = GetActorRotation();
}

// ���� ���� �� ����
void AEclipsedCavernsPlayer::BeginPlay()
{
    Super::BeginPlay();
}

// �� ������ ����
void AEclipsedCavernsPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ȸ�� �� ����
    SetActorRotation(NewRotation);
}

// �Է� ���ε�
void AEclipsedCavernsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // ȸ��
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AEclipsedCavernsPlayer::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AEclipsedCavernsPlayer::LookUp);

    // �̵�
    PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AEclipsedCavernsPlayer::InputHorizontal);
    PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AEclipsedCavernsPlayer::InputVertical);

    // ���� & ������Ʈ
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AEclipsedCavernsPlayer::InputJump);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AEclipsedCavernsPlayer::Sprint);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AEclipsedCavernsPlayer::StopSprinting);
}

// ī�޶� ȸ�� (Yaw)
void AEclipsedCavernsPlayer::Turn(float Value)
{
    AddControllerYawInput(Value);
}

// ī�޶� ȸ�� (Pitch)
void AEclipsedCavernsPlayer::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

// �¿� ȸ�� (ī�޶� ����)
void AEclipsedCavernsPlayer::InputHorizontal(float Value)
{
    if (Value != 0.0f)
    {
        // ���� ĳ���� ȸ�� �� ��������
        NewRotation = GetActorRotation();

        // Yaw(�¿� ȸ��) �� ���� (ī�޶�� �״��)
        NewRotation.Yaw += Value * 100.0f * GetWorld()->GetDeltaSeconds();
    }
}

// �յ� �̵� (���� ���� ����)
void AEclipsedCavernsPlayer::InputVertical(float Value)
{
    if (Value != 0.0f)
    {
        // ���� ĳ���Ͱ� �ٶ󺸴� ���� ��������
        FVector ForwardDirection = GetActorForwardVector();

        // ���� �̵� �ӵ� ��������
        float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;

        // ���ο� ��ġ ���
        FVector NewLocation = GetActorLocation() + (ForwardDirection * Value * CurrentSpeed * GetWorld()->GetDeltaSeconds());

        // ��ġ ����
        SetActorLocation(NewLocation, true);
    }
}

// ����
void AEclipsedCavernsPlayer::InputJump()
{
    Jump();
}

// ������Ʈ (�ӵ� ����)
void AEclipsedCavernsPlayer::Sprint()
{
    GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * SprintSpeedMultiplier;
}

// ������Ʈ ���� (�⺻ �ӵ��� ����)
void AEclipsedCavernsPlayer::StopSprinting()
{
    GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

// �̵� ó�� (���� ��� �� ��)
void AEclipsedCavernsPlayer::Move()
{
    direction = FTransform(GetControlRotation()).TransformVector(direction);
    AddMovementInput(direction);
    direction = FVector::ZeroVector;

    // ȸ�� �� ����
    SetActorRotation(NewRotation);
}
