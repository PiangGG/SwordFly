// Fill out your copyright notice in the Description page of Project Settings.


#include "SFCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"

// Sets default values
ASFCharacter::ASFCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed=300.f;
	bIsRun=false;
	SpringArmComp=CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength=300.f;
	SpringArmComp->bUsePawnControlRotation=true;
	
	TiredCamera=CreateDefaultSubobject<UCameraComponent>(FName("TiredCamera"));
	TiredCamera->SetupAttachment(SpringArmComp);
	TiredCamera->FieldOfView=110.f;

	bReplicates=true;
	SetReplicates(true);
    ACharacter::SetReplicateMovement(true);
	bAlwaysRelevant=true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->SetIsReplicated(true);

}

// Called when the game starts or when spawned
void ASFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled())
	{
		//UE_LOG(LogTemp,Warning,TEXT("RunStart %f,%f"),GetCharacterMovement()->MaxWalkSpeed,RootComponent->GetComponentVelocity().Size());

	}
}

// Called to bind functionality to input
void ASFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&ASFCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASFCharacter::MoveRight);
	PlayerInputComponent->BindAxis("RotateCamera",this,&ASFCharacter::RotateCamera);
	PlayerInputComponent->BindAxis("ChangeCameraHeight",this,&ASFCharacter::ChangeCameraHeight);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ASFCharacter::Jump);
	PlayerInputComponent->BindAction("Run",IE_Pressed,this,&ASFCharacter::RunStart);
	PlayerInputComponent->BindAction("Run",IE_Released,this,&ASFCharacter::RunEnd);
}

void ASFCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASFCharacter,bIsRun);
}

void ASFCharacter::MoveForward(float amount)
{
	if (amount)
	{
		this->AddMovementInput(GetActorForwardVector(),amount);
	}
}

void ASFCharacter::MoveRight(float amount)
{
	if (amount)
	{
		this->AddMovementInput(GetActorRightVector(),amount);
	}
}

void ASFCharacter::RotateCamera(float amount)
{
	if (amount)
	{
		AddControllerYawInput(amount);
	}
	
}

void ASFCharacter::ChangeCameraHeight(float amount)
{
	if (amount)
	{
		AddControllerPitchInput(amount);
	}
}

void ASFCharacter::RunEnd()
{
	if (GetLocalRole()<ROLE_Authority)
	{
		RunEndServer();
	}
	GetCharacterMovement()->MaxWalkSpeed=300.f;
}

void ASFCharacter::RunEndServer_Implementation()
{
	RunEnd();
}

void ASFCharacter::RunStart()
{
	if (GetLocalRole()<ROLE_Authority)
	{
		RunStartServer();
	}
	GetCharacterMovement()->MaxWalkSpeed=600.f;
}

void ASFCharacter::RunStartServer_Implementation()
{
	RunStart();
}

void ASFCharacter::OnRep_bIsRun()
{
	if (bIsRun)
	{
		GetCharacterMovement()->MaxWalkSpeed=600.f;
	}else
	{
		GetCharacterMovement()->MaxWalkSpeed=300.f;
	}
	UE_LOG(LogTemp,Warning,TEXT("OnRep_bIsRun %f ,%f"),GetCharacterMovement()->MaxWalkSpeed,RootComponent->GetComponentVelocity().Size());
}

void ASFCharacter::Jump()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		this->ACharacter::Jump();
		Crouch();
	}
}


