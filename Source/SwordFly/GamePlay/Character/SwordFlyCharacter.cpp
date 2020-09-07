// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "SwordFly/GamePlay/PlayerController/SwordFlyPlayerController.h"
#include "SwordFly/Itme/BaseItem.h"
#include "SwordFly/Itme/Weapons/SwordFlyBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Serialization/JsonTypes.h"
#include "SwordFly/GamePlay/PlayerState/SwordFlyPlayerState.h"
class ASwordFlyBaseWeapon;
// Sets default values
ASwordFlyCharacter::ASwordFlyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp=CreateDefaultSubobject<USpringArmComponent>(FName("springcomp"));
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->TargetArmLength = 700.f;
	SpringArmComp->SetupAttachment(RootComponent);
	
	TiredCamera=CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	TiredCamera->FieldOfView = 110.f;
	TiredCamera->SetupAttachment(SpringArmComp);

	bAlwaysRelevant = true;
	bReplayRewindable=true;
	SetReplicates(true);
    ACharacter::SetReplicateMovement(true);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->MaxWalkSpeed= 300.f;
	GetCharacterMovement()->SetIsReplicated(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_EngineTraceChannel1,ECR_Overlap);
   
}

void ASwordFlyCharacter::BeginPlay()
{
	Super::BeginPlay();
	ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	if (PS)
	{
		GetCharacterMovement()->MaxWalkSpeed = PS->PlayerSpeed;
	}
}


void ASwordFlyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwordFlyCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASwordFlyCharacter, CurrentCharacterState);
}


void ASwordFlyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&ASwordFlyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASwordFlyCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("RotateCamera",this,&ASwordFlyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("ChangeCameraHeight",this,&ASwordFlyCharacter::ChangeCameraHeight);
	PlayerInputComponent->BindAxis("ChangeCameraHeight",this,&ASwordFlyCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("UnEquipment",EInputEvent::IE_Pressed,this,&ASwordFlyCharacter::UnEquipment);
	PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed,this,&ACharacter::Jump);

	PlayerInputComponent->BindAction("Attack",EInputEvent::IE_Pressed,this,&ASwordFlyCharacter::Attack);
	PlayerInputComponent->BindAction("Attack",EInputEvent::IE_Released,this,&ASwordFlyCharacter::Attack);
	
}

void ASwordFlyCharacter::MoveForward(float amount)
{
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;

	if (amount) {
		
		AddMovementInput(TiredCamera->GetForwardVector(),amount);
	}
	
}

void ASwordFlyCharacter::MoveRight(float amount)
{
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;

	
	if (PC->IsLocalController() && amount) {
		
		AddMovementInput(TiredCamera->GetRightVector(), amount);
	}
	
}

void ASwordFlyCharacter::RotateCamera(float amount)
{
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;

	
	if (PC->IsLocalController() && amount) {
		FVector rot = SpringArmComp->GetComponentRotation().Euler();
		rot += FVector(0, 0, amount);
		SpringArmComp->SetWorldRotation(FQuat::MakeFromEuler(rot));
	}
}

void ASwordFlyCharacter::ChangeCameraHeight(float amount)
{
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;

	
	if (PC->IsLocalController() && amount) {
		FVector rot = SpringArmComp->GetComponentRotation().Euler();

		float originalHeight = rot.Y;
		originalHeight += amount;
		originalHeight = FMath::Clamp(originalHeight, -45.f, -5.f);

		rot = FVector(0, originalHeight, rot.Z);
		SpringArmComp->SetWorldRotation(FQuat::MakeFromEuler(rot));
	}
}

ECharacterState ASwordFlyCharacter::GetCharacterState()
{
	return CurrentCharacterState;
}

void ASwordFlyCharacter::SetCharacterState(ECharacterState newState)
{
	CurrentCharacterState=newState;
	switch (CurrentCharacterState)
	{
		case ECharacterState::ENone:{break;}
		case ECharacterState::ESword:{break;}
		case ECharacterState::EBow:{break;}
		case ECharacterState::EOther:{break;}
	default:
		break;
	}
}

ABaseItem* ASwordFlyCharacter::GetCurrentWeapon()
{
	if (GetLocalRole()!=ROLE_Authority) return nullptr;
	ASwordFlyPlayerState* PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	if (PS)
	{
		return PS->GetCurrentWeapon();
	}
	return nullptr;
}

void ASwordFlyCharacter::SetCurrentWeapon(ABaseItem* Weapon)
{
	if (GetLocalRole()!=ROLE_Authority)return;
	
	ASwordFlyPlayerState* PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	
	ASwordFlyBaseWeapon* thisWeapon=Cast<ASwordFlyBaseWeapon>(PS->CurrentWeapon);
	
	switch (thisWeapon->GetWeaponType())
	{
		case EWeaponType::EBow:
			{
				SetCharacterState(ECharacterState::EBow);
				break;
			}
		case EWeaponType::ESword:
			{
				SetCharacterState(ECharacterState::ESword);
				break;
			}
		case EWeaponType::EOther:
			{
				SetCharacterState(ECharacterState::ENone);
				break;
			}
		default: break;;
	}
}

void ASwordFlyCharacter::PackUp(ABaseItem* Itme)
{
	if (GetLocalRole()!=ROLE_Authority)return;
	ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>( GetPlayerState());
	if (PS)
	{
		PS->PackUp(Itme);
	}
}

void ASwordFlyCharacter::Equipment(ABaseItem* Itme)
{
	if (GetLocalRole()!=ROLE_Authority)return;
	ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>( GetPlayerState());
	if (PS)
	{
		PS->Equipment(Itme);
	}
}

void ASwordFlyCharacter::UnEquipment()
{
	if (GetLocalRole()!=ROLE_Authority)return;
	ASwordFlyPlayerState* PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	if (PS)
	{
		PS->UnEquipment();
	}
	
}

void ASwordFlyCharacter::SweapWeapon(ABaseItem* newWeapon)
{
	if (GetLocalRole()!=ROLE_Authority)return;
	ASwordFlyPlayerState* PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	if (PS)
	{
		
		PS->SweapWeapon(newWeapon);
	}
	
}


void ASwordFlyCharacter::Attack()
{
	if (GetLocalRole()!=ROLE_Authority)return;
	ASwordFlyPlayerState* PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	if (PS&&PS->CurrentWeapon)
	{
		PS->CurrentWeapon->Attack();
	}
}

void ASwordFlyCharacter::ReceiveDamage(float var)
{
	if (GetLocalRole()!=ROLE_Authority)return;
	ASwordFlyPlayerState* PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	if (PS)
	{
		PS->ReceiveDamage(var);
	}
}