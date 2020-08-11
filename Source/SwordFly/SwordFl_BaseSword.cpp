// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFl_BaseSword.h"
#include "Components/SphereComponent.h"

ASwordFl_BaseSword::ASwordFl_BaseSword()
{
    ASwordFl_BaseSword::SetItmeType(EItmeType::EWeapon);
    this->thisOwner=nullptr;

    Collision_Attack=CreateDefaultSubobject<USphereComponent>(FName("Collision_Attack"));
    Collision_Attack->SetupAttachment(RootComponent);
    Collision_Attack->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Collision_Attack->SetCollisionResponseToAllChannels(ECR_Ignore);
    Collision_Attack->SetCollisionResponseToChannel(ECC_EngineTraceChannel1,ECR_Overlap);
    
    Collision_Attack->OnComponentBeginOverlap.AddDynamic(this,&ASwordFl_BaseSword::Attack);

    //Collision_Pack
}

void ASwordFl_BaseSword::SetItmeType(EItmeType Type)
{
    thisItmeType=Type;
   
}

void ASwordFl_BaseSword::Attack(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Attack"));
}

void ASwordFl_BaseSword::Collision_Pack_BeginOverlap(UPrimitiveComponent* Component, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::Collision_Pack_BeginOverlap(Component, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    
}
