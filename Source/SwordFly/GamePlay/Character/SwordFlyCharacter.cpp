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
	//SpringArmComp->SetAbsolute(true);
	//SpringArmComp->SetUsingAbsoluteScale(true);
	SpringArmComp->TargetArmLength = 700.f;
	SpringArmComp->SetupAttachment(RootComponent);
	//RootComponent=SpringArmComp;
	TiredCamera=CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	TiredCamera->FieldOfView = 110.f;
	TiredCamera->SetupAttachment(SpringArmComp);

	/*bReplicates = true;
	bReplicateMovement=true;*/
	bAlwaysRelevant = true;
	bReplayRewindable=true;
	//bReplicateMovement=true;
	SetReplicates(true);
    SetReplicateMovement(true);
	
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
    // ReSharper disable once CppDeprecatedEntity
   // GetMesh()->MeshComponentUpdateFlag= EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	//GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

// Called when the game starts or when spawned
void ASwordFlyCharacter::BeginPlay()
{
	Super::BeginPlay();
	ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>(GetPlayerState());
	if (PS)
	{
		GetCharacterMovement()->MaxWalkSpeed = PS->PlayerSpeed;
	}
}

// Called every frame
void ASwordFlyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwordFlyCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASwordFlyCharacter, CurrentCharacterState);
}

// Called to bind functionality to input
void ASwordFlyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&ASwordFlyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASwordFlyCharacter::MoveRight);
	//PlayerInputComponent->BindAxis("RotateCamera",this,&ASwordFlyCharacter::RotateCamera);
	PlayerInputComponent->BindAxis("RotateCamera",this,&ASwordFlyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("ChangeCameraHeight",this,&ASwordFlyCharacter::ChangeCameraHeight);
	PlayerInputComponent->BindAxis("ChangeCameraHeight",this,&ASwordFlyCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("UnEquipment",EInputEvent::IE_Pressed,this,&ASwordFlyCharacter::UnEquipment);
	PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed,this,&ACharacter::Jump);

	PlayerInputComponent->BindAction("Attack",EInputEvent::IE_Pressed,this,&ASwordFlyCharacter::Attack);
	PlayerInputComponent->BindAction("Attack",EInputEvent::IE_Released,this,&ASwordFlyCharacter::Attack);
	/*
	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Pressed,this,&ASwordFlyCharacter::RunStartClient);
	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Released,this,&ASwordFlyCharacter::RunEndClient);*/
	
	//PlayerInputComponent->BindAction("Inventory")
}

void ASwordFlyCharacter::MoveForward(float amount)
{
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;

	if (amount) {
		//AddMovementInput(SpringArmComp->GetForwardVector(), amount);
		AddMovementInput(TiredCamera->GetForwardVector(),amount);
	}
	
}

void ASwordFlyCharacter::MoveRight(float amount)
{
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;

	//add input in the camera's right direction
	if (PC->IsLocalController() && amount) {
		//AddMovementInput(SpringArmComp->GetRightVector(), amount);
		AddMovementInput(TiredCamera->GetRightVector(), amount);
	}
	//
}

void ASwordFlyCharacter::RotateCamera(float amount)
{
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;

	//add rotation on the spring arm's z axis
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

	//add rotation on spring arm's y axis. Clamp between -45 and -5
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
	return CurrentWeapon;
}

void ASwordFlyCharacter::SetCurrentWeapon(ABaseItem* Weapon)
{
	CurrentWeapon = Weapon;
	ASwordFlyBaseWeapon* thisWeapon=Cast<ASwordFlyBaseWeapon>(CurrentWeapon);
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
	//PackUpServer(Itme);
	ABaseItem* thisItem = Cast<ABaseItem>(Itme);
	//EItmeType type= thisItem->GetItemType();
	switch (thisItem->GetItemType()) {
	case EItmeType::EWeapon:
		Equipment(thisItem);
		break;
	case EItmeType::EOther:
		thisItem->Destroy();
		break;
	default:
		thisItem->Destroy();
		break;
	}
}

void ASwordFlyCharacter::Equipment(ABaseItem* Itme)
{
	EquipmentServer(Itme);
}

void ASwordFlyCharacter::UnEquipment()
{
	UnEquipmentServer();
}

void ASwordFlyCharacter::Attack()
{
	AttackServer();
}

void ASwordFlyCharacter::AttackServer_Implementation()
{
	AttackNetMulticast();
}
bool ASwordFlyCharacter::AttackServer_Validate()
{
	return true;
}

void ASwordFlyCharacter::AttackNetMulticast_Implementation()
{
	if (CurrentWeapon == nullptr)return;
	
	ASwordFlyBaseWeapon* thisWeapon = Cast<ASwordFlyBaseWeapon>(CurrentWeapon);
	
	
	thisWeapon->Attack();
}

void ASwordFlyCharacter::UnEquipmentServer_Implementation()
{
	UnEquipmentNetMulticast();
}

bool ASwordFlyCharacter::UnEquipmentServer_Validate()
{
	return true;
}

void ASwordFlyCharacter::UnEquipmentNetMulticast_Implementation()
{
	if (CurrentCharacterState==ECharacterState::ENone)return;
	SetCharacterState(ECharacterState::ENone);
	CurrentWeapon->AfterThroud(this);
	CurrentWeapon = nullptr;
}

void ASwordFlyCharacter::EquipmentNetMulticast_Implementation(ABaseItem* Itme)
{
	ASwordFlyBaseWeapon* NewWeapon = Cast<ASwordFlyBaseWeapon>(Itme);

	if (CurrentWeapon == nullptr)
	{

		NewWeapon->Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket_Right);
		SetCurrentWeapon(NewWeapon);
	}
}

void ASwordFlyCharacter::EquipmentServer_Implementation(ABaseItem* Itme)
{
	EquipmentNetMulticast(Itme);
}
bool ASwordFlyCharacter::EquipmentServer_Validate(ABaseItem* Itme)
{
	return true;
}

void ASwordFlyCharacter::PackUpServer_Implementation(ABaseItem* Itme)
{
	PackUpNetMulticast(Itme);
}
bool ASwordFlyCharacter::PackUpServer_Validate(ABaseItem* Itme)
{
	return true;
}
void ASwordFlyCharacter::PackUpNetMulticast_Implementation(ABaseItem* Itme)
{
	/*ABaseItem* thisItem = Cast<ABaseItem>(Itme);
	//EItmeType type= thisItem->GetItemType();
	switch (thisItem->GetItemType()) {
	case EItmeType::EWeapon:
		Equipment(thisItem);
		break;
	case EItmeType::EOther:
		thisItem->Destroy();
		break;
	default:
		thisItem->Destroy();
		break;
	}*/
}