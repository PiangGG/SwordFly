// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GamePlay/PlayerState/SwordFlyPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AArrow::AArrow()
{
    PrimaryActorTick.bCanEverTick = true;
    ASwordFlyBaseWeapon::SetItmeType(EItmeType::EWeapon);
    this->thisOwner=nullptr;
    ASwordFlyBaseWeapon::SetWeaponType(EWeaponType::EArrow);
   
    Collision_Attack->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Collision_Attack->SetCollisionResponseToChannels(ECR_Block);
    Collision_Attack->SetSimulatePhysics(true);
    //RootComponent=Collision_Attack;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> objFinder(TEXT("StaticMesh'/Game/Mesh/Arrow01SM.Arrow01SM'"));
    Mesh->SetStaticMesh(objFinder.Object);
    Mesh->SetRelativeRotation(FRotator(0,180,0));
    Mesh->SetSimulatePhysics(true);
    
    //Mesh->SetupAttachment(Collision_Attack);
    DamageVar=20.f;
    ActorID=4;
    TimeLive=10.f;
}

void AArrow::Collision_Pack_BeginOverlap(UPrimitiveComponent* Component, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //ASwordFlyCharacter *Character=Cast<ASwordFlyCharacter>(OtherActor);
    //ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>(Character->GetPlayerState());
    //this->Destroy();
}

void AArrow::OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    OnHitActorServer(HitComponent, OtherActor,OtherComp,
   NormalImpulse,Hit);
}

void AArrow::OnHitActorServer_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (GetLocalRole()!=ROLE_Authority)return;
    UE_LOG(LogTemp, Warning, TEXT("OnHitActorServer_Implementation"));
    ASwordFlyCharacter* Charactered=Cast<ASwordFlyCharacter>(OtherActor);
    if (Charactered)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnHitActorServer_Implementation1"));
        Charactered->ReceiveDamage(DamageVar);
        this->Destroy();
    }
}

bool AArrow::OnHitActorServer_Validate(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    return true;
}

void AArrow::BeginPlay()
{
    Super::BeginPlay();
    Collision_Attack->OnComponentHit.AddDynamic(this,&AArrow::OnHitActor);

}

void AArrow::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    TimeLive-=DeltaSeconds;
    if (TimeLive<=0.f)
    {
        this->Destroy();
    }
}

