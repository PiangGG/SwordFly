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
    EOther         	UMETA(DisplayName="其他")
};
UCLASS()
class SWORDFLY_API ASwordFlyBaseWeapon : public ABaseItem
{
	GENERATED_BODY()
	
public:
	
	ASwordFlyBaseWeapon();

	//UFUNCTION(BlueprintCallable)
    virtual void SetItmeType(EItmeType Type) override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class USphereComponent* Collision_Attack;
	
	UFUNCTION()
    void Attack(class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    //UFUNCTION()
	virtual void Collision_Pack_BeginOverlap(class UPrimitiveComponent* Component,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EWeaponType thisWeaponType;
	UFUNCTION(BlueprintCallable)
	EWeaponType GetWeaponType();
	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponType(EWeaponType newType);
};