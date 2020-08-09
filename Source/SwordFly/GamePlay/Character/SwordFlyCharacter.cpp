// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../PlayerController/SwordFlyPlayerController.h"

// Sets default values
ASwordFlyCharacter::ASwordFlyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp=CreateDefaultSubobject<USpringArmComponent>(FName("springcomp"));
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->SetAbsolute(true);
	SpringArmComp->TargetArmLength = 700.f;
	SpringArmComp->SetupAttachment(RootComponent);
	//RootComponent=SpringArmComp;
	TiredCamera=CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	TiredCamera->FieldOfView = 110.f;
	TiredCamera->SetupAttachment(RootComponent);

	bReplicates = true;
	bAlwaysRelevant = true;
	//bReplicateMovement = true;
	//this->SetReplicatedMovement();
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.8f;
}

// Called when the game starts or when spawned
void ASwordFlyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwordFlyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASwordFlyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForword",this,&ASwordFlyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASwordFlyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("RotateCamera",this,&ASwordFlyCharacter::RotateCamera);
	PlayerInputComponent->BindAxis("ChangeCameraHeight",this,&ASwordFlyCharacter::ChangeCameraHeight);

	PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed,this,&ACharacter::Jump);
}

void ASwordFlyCharacter::MoveForward(float amount)
{
	APlayerController *PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	//if (PC && PC->bPauseMenuDisplayed) return;

	if (Controller && amount) {
		AddMovementInput(SpringArmComp->GetForwardVector(), amount);
	}
}

void ASwordFlyCharacter::MoveRight(float amount)
{
	APlayerController *PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	//if (PC && PC->bPauseMenuDisplayed) return;

	//add input in the camera's right direction
	if (Controller && amount) {
		AddMovementInput(SpringArmComp->GetRightVector(), amount);
	}
}

void ASwordFlyCharacter::RotateCamera(float amount)
{
	APlayerController *PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	//if (PC && PC->bPauseMenuDisplayed) return;

	//add rotation on the spring arm's z axis
	if (Controller && amount) {
		FVector rot = SpringArmComp->GetComponentRotation().Euler();
		rot += FVector(0, 0, amount);
		SpringArmComp->SetWorldRotation(FQuat::MakeFromEuler(rot));
	}
}

void ASwordFlyCharacter::ChangeCameraHeight(float amount)
{
	APlayerController *PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	//if (PC && PC->bPauseMenuDisplayed) return;

	//add rotation on spring arm's y axis. Clamp between -45 and -5
	if (Controller && amount) {
		FVector rot = SpringArmComp->GetComponentRotation().Euler();

		float originalHeight = rot.Y;
		originalHeight += amount;
		originalHeight = FMath::Clamp(originalHeight, -45.f, -5.f);

		rot = FVector(0, originalHeight, rot.Z);
		SpringArmComp->SetWorldRotation(FQuat::MakeFromEuler(rot));
	}
}

