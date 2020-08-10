// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "Components/ShapeComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Owner=nullptr;
	//thisItmeTtpe=NULL;
	Collision_Pack=CreateDefaultSubobject<UShapeComponent>(FName("Collision_Pack"));

	//Collision_Pack->OnComponentBeginOverlap.AddDynamic(this,&ABaseItem::Collision_Pack_BeginOverlap);

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
	return thisItmeTtpe;
}

void ABaseItem::SetItmeType(EItmeType Type)
{
	thisItmeTtpe=Type;
}

void ABaseItem::Collision_Pack_BeginOverlap(UPrimitiveComponent* Component,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
}

