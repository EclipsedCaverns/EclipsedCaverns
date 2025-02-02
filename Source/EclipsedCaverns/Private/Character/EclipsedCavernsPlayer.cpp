#include "Character/EclipsedCavernsPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// 생성자
AEclipsedCavernsPlayer::AEclipsedCavernsPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    // 기본 이동 속도 저장
    DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

    // 스켈레탈 메시 설정 (에셋 로딩)
    ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Skins/FrozenHearth/Meshes/Aurora_FrozenHearth.Aurora_FrozenHearth'"));
    if (TempMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(TempMesh.Object);
        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
    }

    // SpringArm 설정
    springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    springArmComp->SetupAttachment(RootComponent);
    springArmComp->SetRelativeLocation(FVector(0, 70, 90));
    springArmComp->TargetArmLength = 400;
    springArmComp->bUsePawnControlRotation = true;  // 카메라 회전 가능

    // 카메라 설정
    CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
    CamComp->SetupAttachment(springArmComp);
    CamComp->bUsePawnControlRotation = false; // 카메라 회전 X

    // 캐릭터 회전 설정
    bUseControllerRotationYaw = true;

    // 점프 설정
    JumpMaxCount = 1;

    // 초기 회전값
    NewRotation = GetActorRotation();
}

// 게임 시작 시 실행
void AEclipsedCavernsPlayer::BeginPlay()
{
    Super::BeginPlay();
}

// 매 프레임 실행
void AEclipsedCavernsPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 회전 값 적용
    SetActorRotation(NewRotation);
}

// 입력 바인딩
void AEclipsedCavernsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 회전
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AEclipsedCavernsPlayer::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AEclipsedCavernsPlayer::LookUp);

    // 이동
    PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AEclipsedCavernsPlayer::InputHorizontal);
    PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AEclipsedCavernsPlayer::InputVertical);

    // 점프 & 스프린트
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AEclipsedCavernsPlayer::InputJump);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AEclipsedCavernsPlayer::Sprint);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AEclipsedCavernsPlayer::StopSprinting);
}

// 카메라 회전 (Yaw)
void AEclipsedCavernsPlayer::Turn(float Value)
{
    AddControllerYawInput(Value);
}

// 카메라 회전 (Pitch)
void AEclipsedCavernsPlayer::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

// 좌우 회전 (카메라 고정)
void AEclipsedCavernsPlayer::InputHorizontal(float Value)
{
    if (Value != 0.0f)
    {
        // 현재 캐릭터 회전 값 가져오기
        NewRotation = GetActorRotation();

        // Yaw(좌우 회전) 값 변경 (카메라는 그대로)
        NewRotation.Yaw += Value * 100.0f * GetWorld()->GetDeltaSeconds();
    }
}

// 앞뒤 이동 (현재 방향 기준)
void AEclipsedCavernsPlayer::InputVertical(float Value)
{
    if (Value != 0.0f)
    {
        // 현재 캐릭터가 바라보는 방향 가져오기
        FVector ForwardDirection = GetActorForwardVector();

        // 현재 이동 속도 가져오기
        float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;

        // 새로운 위치 계산
        FVector NewLocation = GetActorLocation() + (ForwardDirection * Value * CurrentSpeed * GetWorld()->GetDeltaSeconds());

        // 위치 적용
        SetActorLocation(NewLocation, true);
    }
}

// 점프
void AEclipsedCavernsPlayer::InputJump()
{
    Jump();
}

// 스프린트 (속도 증가)
void AEclipsedCavernsPlayer::Sprint()
{
    GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * SprintSpeedMultiplier;
}

// 스프린트 중지 (기본 속도로 복구)
void AEclipsedCavernsPlayer::StopSprinting()
{
    GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

// 이동 처리 (현재 사용 안 함)
void AEclipsedCavernsPlayer::Move()
{
    direction = FTransform(GetControlRotation()).TransformVector(direction);
    AddMovementInput(direction);
    direction = FVector::ZeroVector;

    // 회전 값 적용
    SetActorRotation(NewRotation);
}
