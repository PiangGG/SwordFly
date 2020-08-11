// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	thisOwner=nullptr;
	//thisItmeType=NULL;
	Collision_Pack=CreateDefaultSubobject<USphereComponent>(FName("Collision_Pack"));

	Collision_Pack->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision_Pack->SetCollisionResponseToChannels(ECR_Ignore);
	
	Collision_Pack->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	
	Mesh=CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));

	RootComponent=Mesh;
	Collision_Pack->SetupAttachment(RootComponent);
	Collision_Pack->OnComponentBeginOverlap.AddDynamic(this,&ABaseItem::Collision_Pack_BeginOverlap);
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
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
	ASwordFlyCharacter* Player=Cast<ASwordFlyCharacter>(OtherActor);
	if (Player)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("拾取initem"));
		Collision_Pack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Collision_Pack->SetCollisionResponseToChannels(ECR_Ignore);
		thisOwner=Player;
		Player->PackUp(this);
	}
}

