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
#include "Net/UnrealNetwork.h"
#include "Serialization/JsonTypes.h"
class ASwordFlyBaseWeapon;
// Sets default values
ASwordFlyCharacter::ASwordFlyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp=CreateDefaultSubobject<USpringArmComponent>(FName("springcomp"));
	
	SpringArmComp->bUsePawnControlRotation = false;
	//SpringArmComp->SetAbsolute(true);
	SpringArmComp->SetUsingAbsoluteScale(true);
	SpringArmComp->TargetArmLength = 700.f;
	SpringArmComp->SetupAttachment(RootComponent);
	//RootComponent=SpringArmComp;
	TiredCamera=CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	TiredCamera->FieldOfView = 110.f;
	TiredCamera->SetupAttachment(SpringArmComp);

	bReplicates = true;
	bAlwaysRelevant = true;
	bReplayRewindable=true;
	//bReplicateMovement=true;
	SetReplicatingMovement(true);
	//GetMesh()->SetRelativeTransform();
	//bReplicateMovement=true;
	//this->SetReplicatedMovement();
	//OnRep_ReplicatedMovement();
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

	PlayerInputComponent->BindAxis("MoveForward",this,&ASwordFlyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASwordFlyCharacter::MoveRight);
	//PlayerInputComponent->BindAxis("RotateCamera",this,&ASwordFlyCharacter::RotateCamera);
	PlayerInputComponent->BindAxis("RotateCamera",this,&ASwordFlyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("ChangeCameraHeight",this,&ASwordFlyCharacter::ChangeCameraHeight);
	PlayerInputComponent->BindAxis("ChangeCameraHeight",this,&ASwordFlyCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("UnEquipment",EInputEvent::IE_Pressed,this,&ASwordFlyCharacter::UnEquipment);
	PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed,this,&ACharacter::Jump);
	
	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Pressed,this,&ASwordFlyCharacter::RunStart);
	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Released,this,&ASwordFlyCharacter::RunEnd);
	
	//PlayerInputComponent->BindAction("Inventory")
}

void ASwordFlyCharacter::MoveForward(float amount)
{
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	//if (PC && PC->bPauseMenuDisplayed) return;

	if (PC && amount) {
		//AddMovementInput(SpringArmComp->GetForwardVector(), amount);
		AddMovementInput(TiredCamera->GetForwardVector(),amount);
	}
	
}

void ASwordFlyCharacter::MoveRight(float amount)
{
	ASwordFlyPlayerController *PC = Cast<ASwordFlyPlayerController>(GetWorld()->GetFirstPlayerController());

	//if (PC && PC->bPauseMenuDisplayed) return;

	//add input in the camera's right direction
	if (PC && amount) {
		//AddMovementInput(SpringArmComp->GetRightVector(), amount);
		AddMovementInput(TiredCamera->GetRightVector(), amount);
	}
	//
}

void ASwordFlyCharacter::RotateCamera(float amount)
{
	APlayerController *PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	//if (PC && PC->bPauseMenuDisplayed) return;

	//add rotation on the spring arm's z axis
	if (PC && amount) {
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
	if (PC && amount) {
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

void ASwordFlyCharacter::PackUp_Implementation(ABaseItem* Itme)
{
	ABaseItem* thisItem=Cast<ABaseItem>(Itme);
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

bool ASwordFlyCharacter::PackUp_Validate(ABaseItem* Itme)
{
	return true;
}

/*void ASwordFlyCharacter::PackUp(ABaseItem* Itme)
{
	UE_LOG(LogTemp, Warning, TEXT("拾取inCharacter"));
	ABaseItem* thisItem=Cast<ABaseItem>(Itme);
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
}*/

void ASwordFlyCharacter::UnEquipment_Implementation()
{
	if (GetCurrentWeapon()==nullptr)return;
	SetCharacterState(ECharacterState::ENone);
	CurrentWeapon->Mesh->SetSimulatePhysics(true);
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->AfterThroud(this);
	//
	//SetCurrentWeapon();
	CurrentWeapon=nullptr;
}

bool ASwordFlyCharacter::UnEquipment_Validate()
{
	return true;
}

void ASwordFlyCharacter::Equipment_Implementation(ABaseItem* Itme)
{
	ASwordFlyBaseWeapon  *NewWeapon=Cast<ASwordFlyBaseWeapon>(Itme);
	
	if (CurrentWeapon==nullptr)
	{
		
		NewWeapon->Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NewWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,Socket_Right);
		SetCurrentWeapon(NewWeapon);
	}
}

bool ASwordFlyCharacter::Equipment_Validate(ABaseItem* Itme)
{
	return true;
}

/*void ASwordFlyCharacter::Equipment(ABaseItem* Itme)
{
	ASwordFlyBaseWeapon  *NewWeapon=Cast<ASwordFlyBaseWeapon>(Itme);
	
	if (CurrentWeapon==nullptr)
	{
		
		NewWeapon->Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NewWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,Socket_Right);
		SetCurrentWeapon(NewWeapon);
		//NewWeapon->GetWeaponType();
	}else
	{
		/*ASwordFlyBaseWeapon  *CurrentWeapon=Cast<ASwordFlyBaseWeapon>(GetCurrentWeapon());
		if (NewWeapon->GetWeaponType()!=CurrentWeapon->GetWeaponType())
		{
			
		}
	}
}
*/
/*void ASwordFlyCharacter::UnEquipment()
{
	if (GetCurrentWeapon()==nullptr)return;
	SetCharacterState(ECharacterState::ENone);
	CurrentWeapon->Mesh->SetSimulatePhysics(true);
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->AfterThroud(this);
	//
	//SetCurrentWeapon();
	CurrentWeapon=nullptr;
}*/

void ASwordFlyCharacter::RunEnd_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed=300.f;
}

bool ASwordFlyCharacter::RunEnd_Validate()
{
	return  true;
}

void ASwordFlyCharacter::RunStart_Implementation()
{
	//if (Role >= ROLE_Authority)return;
	
	GetCharacterMovement()->MaxWalkSpeed=600.f;
}

bool ASwordFlyCharacter::RunStart_Validate()
{
	return  true;
}

/*void ASwordFlyCharacter::RunStart()
{
	GetCharacterMovement()->MaxWalkSpeed=600.f;
}*/

/*void ASwordFlyCharacter::RunEnd()
{
	//UE_LOG(LogTemp, Warning, TEXT("测试。"));
	GetCharacterMovement()->MaxWalkSpeed=300.f;
}*/


