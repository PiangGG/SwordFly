// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow2.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GamePlay/PlayerState/SwordFlyPlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AArrow2::AArrow2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackComp=CreateDefaultSubobject<USphereComponent>(FName("AttackComp"));

	RootComponent=AttackComp;
	AttackComp->SetSimulatePhysics(true);
	AttackComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AttackComp->SetCollisionResponseToChannels(ECR_Overlap);
	Mesh1=CreateDefaultSubobject<UStaticMeshComponent>(FName("Name"));
	Mesh1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh1->SetCollisionResponseToChannels(ECR_Ignore);
	Mesh1->SetSimulatePhysics(false);
	AttackComp->OnComponentHit.AddDynamic(this,&AArrow2::OnHitOther);

	TimeLiva=10.f;
	
}

// Called when the game starts or when spawned
void AArrow2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimeLiva=TimeLiva-DeltaTime;
	if (TimeLiva<=0)
	{
		this->Destroy();
	}
}
void AArrow2::OnHitOther_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (GetLocalRole()!=ROLE_Authority)return;
	UE_LOG(LogTemp, Warning, TEXT("OnHitActorServer_Implementation"));
	ASwordFlyCharacter* Charactered=Cast<ASwordFlyCharacter>(OtherActor);
	if (Charactered)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnHitActorServer_Implementation1"));
		Charactered->ReceiveDamage(20.f);
		this->Destroy();
	}
}

bool AArrow2::OnHitOther_Validate(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	return true;
}
