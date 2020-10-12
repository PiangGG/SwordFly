// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SwordFly/Component/SwordFlyInformationrComponent.h"
#include "SwordFly/GamePlay/PlayerState/SwordFlyPlayerState.h"

// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	thisOwner = nullptr;
	//thisItmeType=NULL;

	Collision_Pack = CreateDefaultSubobject<USphereComponent>(FName("Collision_Pack"));
	Collision_Pack->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision_Pack->SetCollisionResponseToChannels(ECR_Ignore);
	Collision_Pack->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Overlap);
	Collision_Pack->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Mesh->SetCollisionResponseToChannels(ECR_Block);
	RootComponent = Mesh;
	Collision_Pack->SetupAttachment(RootComponent);
	

	bReplicates = true;
	bReplayRewindable = true;
	bAlwaysRelevant = true;
	AActor::SetReplicateMovement(true);

	ActorID=0;
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	Collision_Pack->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::Collision_Pack_BeginOverlap);
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EItmeType ABaseItem::GetItemType()
{
	return thisItmeType;
}

void ABaseItem::SetItmeType(EItmeType Type)
{
	thisItmeType=Type;
}

void ABaseItem::Collision_Pack_BeginOverlap(UPrimitiveComponent* Component,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner() != nullptr)return;
	
	ASwordFlyCharacter* Player = Cast<ASwordFlyCharacter>(OtherActor);
	
	if (Player)
	{
		Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Collision_Pack->SetCollisionResponseToChannels(ECR_Ignore);
		
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetOwner(Player);
		Pack(Player);
	}
}

void ABaseItem::AfterThroud(class ASwordFlyCharacter* theOwner)
{
	AfterThroudServer(theOwner);
}

void ABaseItem::Pack(ASwordFlyCharacter* theOwner)
{
	PackServer(theOwner);
}

void ABaseItem::PackServer_Implementation(ASwordFlyCharacter* theOwner)
{
	if (GetLocalRole()!=ROLE_Authority)return;
	theOwner->PackUp(this);
}

bool ABaseItem::PackServer_Validate(ASwordFlyCharacter* theOwner)
{
	return true;
}

void ABaseItem::AfterThroudServer_Implementation(ASwordFlyCharacter* theOwner)
{
	if (GetLocalRole()!=ROLE_Authority)return;
	AfterThroudNetMulticast(theOwner);
	
}
bool ABaseItem::AfterThroudServer_Validate(ASwordFlyCharacter* theOwner)
{
	//AfterThroudNetMulticast(theOwner);
	return true;
}

void ABaseItem::AfterThroudNetMulticast_Implementation(ASwordFlyCharacter* theOwner)
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ABaseItem::ReSetPackupFTimerHndle, 1.0f, true, 5.0f);
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Mesh->SetCollisionResponseToChannels(ECR_Block);
	
	Collision_Pack->SetCollisionResponseToChannels(ECR_Ignore);
	Collision_Pack->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Overlap);
	Collision_Pack->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	this->SetActorHiddenInGame(false);
	this->SetOwner(nullptr);
	//UE_LOG(LogTemp, Warning, TEXT("UnEquipment9"));
	
}

void ABaseItem::ReSetPackupFTimerHndle()
{
	Collision_Pack->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

