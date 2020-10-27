// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GamePlay/PlayerState/SwordFlyPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AArrow::AArrow()
{
    PrimaryActorTick.bCanEverTick = true;
    ASwordFlyBaseWeapon::SetItmeType(EItmeType::EWeapon);
    this->thisOwner=nullptr;
    ASwordFlyBaseWeapon::SetWeaponType(EWeaponType::EArrow);
   
    Collision_Attack->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Collision_Attack->SetCollisionResponseToChannels(ECR_Ignore);
    Collision_Attack->SetSimulatePhysics(false);
    //RootComponent=Collision_Attack;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> objFinder(TEXT("StaticMesh'/Game/Mesh/Arrow01SM.Arrow01SM'"));
    Mesh->SetStaticMesh(objFinder.Object);
    Mesh->SetRelativeRotation(FRotator(0,180,0));
    Mesh->SetCollisionResponseToChannels(ECR_Ignore);
    Mesh->SetSimulatePhysics(false);

    bIsShoot=false;
    
    //Mesh->SetupAttachment(Collision_Attack);
    DamageVar=20.f;
    ActorID=4;
    TimeLive=10.f;

    MovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(FName("MovementComponent"));
    MovementComponent->SetUpdatedComponent(RootComponent);
    
   
}

void AArrow::Collision_Pack_BeginOverlap(UPrimitiveComponent* Component, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //ASwordFlyCharacter *Character=Cast<ASwordFlyCharacter>(OtherActor);
    //ASwordFlyPlayerState *PS=Cast<ASwordFlyPlayerState>(Character->GetPlayerState());
    //this->Destroy();
}

void AArrow::OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    //if (GetLocalRole()==ROLE_Authority)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnHitActorServer_Implementation"));
        ASwordFlyCharacter* Charactered=Cast<ASwordFlyCharacter>(OtherActor);
        if (Charactered&&GetOwner()&&Charactered!=GetOwner())
        {
            UE_LOG(LogTemp, Warning, TEXT("OnHitActorServer_Implementation1"));
            Charactered->ReceiveDamage(DamageVar);
            this->Destroy();
        }
    }
}
void AArrow::BeginPlay()
{
    Super::BeginPlay();
    this->OnActorHit.AddDynamic(this,&AArrow::OnHitActor);

    if (GetOwner())
    {
        Mesh->MoveIgnoreActors.Add(GetOwner());
    }
    

}

void AArrow::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (bIsShoot==false)return;
    Mesh->SetCollisionResponseToChannels(ECR_Block);
    Mesh->SetSimulatePhysics(true);
    
    TimeLive-=DeltaSeconds;
    if (TimeLive<=0.f)
    {
        this->Destroy();
    }
}

