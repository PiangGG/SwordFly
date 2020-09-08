// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
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
  
    static ConstructorHelpers::FObjectFinder<UStaticMesh> objFinder(TEXT("StaticMesh'/Game/Mesh/Arrow01SM.Arrow01SM'"));
    Mesh->SetStaticMesh(objFinder.Object);
    Mesh->SetRelativeRotation(FRotator(0,180,0));
    
    DamageVar=20.f;
    ActorID=4;
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
    ASwordFlyCharacter* Charactered=Cast<ASwordFlyCharacter>(OtherActor);
    if (Charactered&&GetWorld()->GetFirstPlayerController()->GetCharacter()!=Charactered)
    {
        Charactered->ReceiveDamage(DamageVar);
        this->Destroy();
    }
}
void AArrow::BeginPlay()
{
    Super::BeginPlay();
    Collision_Attack->OnComponentHit.AddDynamic(this,&AArrow::OnHitActor);

}

