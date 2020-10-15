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
	UFUNCTION(Server,WithValidation, Reliable,BlueprintCallable)
	virtual void AttackServer(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/);
	UFUNCTION(NetMulticast, Reliable,BlueprintCallable)
    virtual void AttackNetMulticast(/*class UPrimitiveComponent* Component, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult*/);
	
    //UFUNCTION()
	//virtual void Collision_Pack_BeginOverlap(class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EWeaponType thisWeaponType;
	UFUNCTION(BlueprintCallable)
	virtual EWeaponType GetWeaponType();
	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponType(EWeaponType newType);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimationMontage")
	class UAnimMontage* AttackAnimMontage;
		
	virtual FRotator SetOwerRotation();

	bool isAttack;

	FName AttachLocation;
	FName AttachBackLocation;
	UFUNCTION(BlueprintCallable)
	virtual void Equipment(class ASwordFlyCharacter* Player);
	UFUNCTION(Server,BlueprintCallable,WithValidation,Reliable)
	virtual void EquipmentServer(class ASwordFlyCharacter* Player);
	UFUNCTION(NetMulticast, Reliable)
	virtual void EquipmentNetMulticast(class ASwordFlyCharacter* Player);

	UFUNCTION(BlueprintCallable)
	virtual void UnEquipment(class ASwordFlyCharacter* Player);
	UFUNCTION(Server,BlueprintCallable,WithValidation,Reliable)
	virtual void UnEquipmentServer(class ASwordFlyCharacter* Player);
	UFUNCTION(NetMulticast, Reliable)
	virtual void UnEquipmentNetMulticast(class ASwordFlyCharacter* Player);

	virtual void PackServer_Implementation(ASwordFlyCharacter* theOwner) override;
};