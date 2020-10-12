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
#include "Misc/AssetRegistryInterface.h"
#include "Net/UnrealNetwork.h"
#include "Serialization/JsonTypes.h"
#include "SwordFly/GamePlay/PlayerState/SwordFlyPlayerState.h"
#include "SwordFly/Component/SwordFlyInformationrComponent.h"
#include "SwordFly/Component/SwordFlyInformationrComponent.h"
#include "SwordFly/Itme/Weapons/Sword.h"
#include "SwordFly/Itme/Weapons/Bow.h"
class ASwordFlyBaseWeapon;
// Sets default values
ASwordFlyCharacter::ASwordFlyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp=CreateDefaultSubobject<USpringArmComponent>(FName("springcomp"));
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->TargetArmLength = 700.f;
	SpringArmComp->SetIsReplicated(true);
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetIsReplicated(true);
	
	TiredCamera=CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	TiredCamera->FieldOfView = 110.f;
	TiredCamera->SetIsReplicated(true);
	TiredCamera->SetupAttachment(SpringArmComp);
	
	bAlwaysRelevant = true;
	bReplayRewindable=true;
	SetReplicates(true);
    ACharacter::SetReplicateMovement(true);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetMesh()->SetIsReplicated(true);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->MaxWalkSpeed= 300.f;
	
	//GetCharacterMovement()->SetIsReplicated(true);
	//GetCharacterMovement()->movem
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_EngineTraceChannel1,ECR_Overlap);
   
	isRuning=false;

	PlayerWeaponArray= TArray<ASwordFlyBaseWeapon*>();
	PlayerItemArray=TArray<ABaseItem*>();
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

	InputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ASwordFlyCharacter::Run);
	InputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ASwordFlyCharacter::Run);
	
}

void ASwordFlyCharacter::MoveForward(float amount)
{
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());
	
	if (!PC/* && PC->bPauseMenuDisplayed*/) return;

	if (PC->IsLocalController() && amount) {
		
		AddMovementInput(TiredCamera->GetForwardVector(),amount);
	}
	
}

void ASwordFlyCharacter::MoveRight(float amount)
{
	

	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (!PC/* && PC->bPauseMenuDisplayed*/) return;
	
	if (PC->IsLocalController() && amount) {
		
		AddMovementInput(TiredCamera->GetRightVector(), amount);
	}
	
}

void ASwordFlyCharacter::RotateCamera(float amount)
{
	
	
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (!PC/* && PC->bPauseMenuDisplayed*/) return;

	
	if (PC->IsLocalController() && amount) {
		FVector rot = SpringArmComp->GetComponentRotation().Euler();
		rot += FVector(0, 0, amount);
		SpringArmComp->SetWorldRotation(FQuat::MakeFromEuler(rot));
	}
}

void ASwordFlyCharacter::ChangeCameraHeight(float amount)
{
	
	
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (!PC/* && PC->bPauseMenuDisplayed*/) return;

	
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

ASwordFlyBaseWeapon* ASwordFlyCharacter::GetCurrentWeapon()
{
	if (PlayerWeaponArray.IsValidIndex(0))
	{
		return PlayerWeaponArray[0];
	}
	return nullptr;
}

void ASwordFlyCharacter::SetCurrentWeapon(ASwordFlyBaseWeapon* Weapon)
{
	if (Weapon==nullptr)
	{
		SetCharacterState(ECharacterState::ENone);
		return;
	}
	ASwordFlyPlayerState* PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	
	ASwordFlyBaseWeapon* thisWeapon=Cast<ASwordFlyBaseWeapon>(Weapon);

	
	if (!thisWeapon||!PS)return;
	
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
		default:
			SetCharacterState(ECharacterState::ENone);
			break;;
	}
}

void ASwordFlyCharacter::PackUp(ABaseItem* Itme)
{
	if (!Itme)return;

	switch (Itme->GetItemType())
	{
		case EItmeType::EWeapon:
			{
				ASwordFlyBaseWeapon *thisWeapon=Cast<ASwordFlyBaseWeapon>(Itme);
				this->Equipment(thisWeapon);
				break;
			}
		case EItmeType::EOther:
			break;
	default:
		break;
	}
}

void ASwordFlyCharacter::Equipment(ASwordFlyBaseWeapon* Itme)
{
	EquipmentServer(Itme);
}

void ASwordFlyCharacter::EquipmentServer_Implementation(ASwordFlyBaseWeapon* Itme)
{
	//if (!GetController()||!GetController()->IsLocalController())return;
	
	if (GetLocalRole()!=ROLE_Authority)return;
	ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	if (!PS)return;
	
	if (PlayerWeaponArray.Num()<2)
	{
		
		if (!PlayerWeaponArray.IsValidIndex(0)&&!PlayerWeaponArray.IsValidIndex(1))
		{
			Itme->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,Itme->AttachLocation);
			PlayerWeaponArray.Insert(Itme,0);
			SetCurrentWeapon(Itme);
			PS->Equipment(Itme);
		}else if (PlayerWeaponArray.IsValidIndex(0)&&!PlayerWeaponArray.IsValidIndex(1)&&PlayerWeaponArray[0]->GetWeaponType()!=Itme->GetWeaponType())
		{
			Itme->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,Itme->AttachBackLocation);
			PlayerWeaponArray.Insert(Itme,1);
			PS->Equipment(Itme);
		}else
		{
			PlayerItemArray.Add(Itme);
			Itme->SetActorHiddenInGame(true);
			PS->PackUp(Itme);
		}
	}else
	{
		PlayerItemArray.Add(Itme);
		Itme->SetActorHiddenInGame(true);
		PS->PackUp(Itme);
	}
	
}

bool ASwordFlyCharacter::EquipmentServer_Validate(ASwordFlyBaseWeapon* Itme)
{
	return true;
}



void ASwordFlyCharacter::UnEquipment()
{
	//if (GetLocalRole()!=ROLE_Authority)return;
	
	UnEquipmentServer();
}

void ASwordFlyCharacter::UnEquipmentServer_Implementation()
{
	if (GetLocalRole()!=ROLE_Authority)return;
	ASwordFlyPlayerState* PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	if (!PS||!PS->PlayerWeaponArray.IsValidIndex(0)||!PlayerWeaponArray.IsValidIndex(0))return;

	PlayerWeaponArray[0]->UnEquipment(this);
	PlayerWeaponArray.RemoveAt(0);
	SetCurrentWeapon(nullptr);
	PS->UnEquipment();
}

bool ASwordFlyCharacter::UnEquipmentServer_Validate()
{
	return true;
}

void ASwordFlyCharacter::SweapWeapon(ASwordFlyBaseWeapon* newWeapon)
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
	AttackServer();	
}

void ASwordFlyCharacter::AttackServer_Implementation()
{
	if (GetLocalRole()==ROLE_Authority)
	{
		AttackNetMulticast();
	}
	
}

bool ASwordFlyCharacter::AttackServer_Validate()
{
	return true;
}

void ASwordFlyCharacter::AttackNetMulticast_Implementation()
{
	if (!GetController())return;
	if (PlayerWeaponArray.IsValidIndex(0))
	{
		
		switch (PlayerWeaponArray[0]->GetWeaponType())
		{
			
			case EWeaponType::ESword:
				{
					ASword *thisWeapon=Cast<ASword>(PlayerWeaponArray[0]);
					if (thisWeapon)
					{
						thisWeapon->SwordAttack();
					}
					
					break;
				}
		case EWeaponType::EBow:
				{
					ABow *thisWeapon=Cast<ABow>(PlayerWeaponArray[0]);
					
					if (thisWeapon)
					{
						thisWeapon->BowAttack();
					}
					break;
				}
			default:
				break;
		}
		
	}
}


void ASwordFlyCharacter::ReceiveDamage(float var)
{
	
	ASwordFlyPlayerState* PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	if (PS)
	{
		PS->ReceiveDamage(var);
	}
}

void ASwordFlyCharacter::Death()
{
	DeathServer();
}

void ASwordFlyCharacter::Run()
{
	RunServer();
}

void ASwordFlyCharacter::RunServer_Implementation()
{
	RunNetMulticast();
}

bool ASwordFlyCharacter::RunServer_Validate()
{
	return true;
}

void ASwordFlyCharacter::RunNetMulticast_Implementation()
{
	//if (GetController())
	{
		if (isRuning) {
			isRuning = false;
			this->GetCharacterMovement()->MaxWalkSpeed = 300.f;
		}
		else
		{
			isRuning = true;
			this->GetCharacterMovement()->MaxWalkSpeed = 600.f;
		}
	}

}

void ASwordFlyCharacter::DeathNetMulticast_Implementation()
{
	if(GetLocalRole()!=ROLE_Authority)return;
	if (!GetController())return;
	if (!GetController()->IsLocalController())return;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(GetMesh()->GetBoneName(0),1,false);
	GetController()->UnPossess();
}

void ASwordFlyCharacter::DeathServer_Implementation()
{
	
		DeathNetMulticast();
	
	
}

bool ASwordFlyCharacter::DeathServer_Validate()
{
	return true;
}
