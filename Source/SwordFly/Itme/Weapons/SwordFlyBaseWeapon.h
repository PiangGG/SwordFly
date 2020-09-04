#pragma once

#include "CoreMinimal.h"
#include "SwordFly/Itme/BaseItem.h"
#include "SwordFlyBaseWeapon.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponType:uint8
{
	ESword        	UMETA(DisplayName="剑"),
	EBow        	UMETA(DisplayName="弓"),
	EArrow       	UMETA(DisplayName="箭"),
    EOther         	UMETA(DisplayName="其他")
};
UCLASS()
class SWORDFLY_API ASwordFlyBaseWeapon : public ABaseItem
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaSeconds) override;
	
	ASwordFlyBaseWeapon();

	//UFUNCTION(BlueprintCallable)
    virtual void SetItmeType(EItmeType Type) override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class USphereComponent* Collision_Attack;
	
	UFUNCTION(BlueprintCallable)
    virtual void Attack(/*class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/);
	UFUNCTION(Server, WithValidation, Reliable)
	virtual void AttackServer(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/);
	UFUNCTION(NetMulticast, Reliable)
	virtual void AttackNetMulticast(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/);

    //UFUNCTION()
	virtual void Collision_Pack_BeginOverlap(class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EWeaponType thisWeaponType;
	UFUNCTION(BlueprintCallable)
	EWeaponType GetWeaponType();
	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponType(EWeaponType newType);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimationMontage")
	class UAnimMontage* AttackAnimMontage;
		
	virtual FRotator SetOwerRotation();

	bool isAttack;

	FName AttachLocation;
};