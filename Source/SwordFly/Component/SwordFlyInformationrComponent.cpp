// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyInformationrComponent.h"
#include "SwordFly/Itme/Weapons/Sword.h"
#include "SwordFly/Itme/Weapons/SwordFlyBaseWeapon.h"
#include "SwordFly/GamePlay/Character/SwordFlyCharacter.h"
// Sets default values for this component's properties
USwordFlyInformationrComponent::USwordFlyInformationrComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USwordFlyInformationrComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USwordFlyInformationrComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USwordFlyInformationrComponent::Equipment(ASwordFlyBaseWeapon* Item)
{
	EquipmentServer(Item);
}

void USwordFlyInformationrComponent::UnEquipmen(ASwordFlyBaseWeapon* Item, int32 var)
{
	UnEquipmenServer(Item,var);
}

void USwordFlyInformationrComponent::UnEquipmenServer_Implementation(ASwordFlyBaseWeapon* Item, int32 var)
{
	UnEquipmenNetMulticast(Item,var);
}

bool USwordFlyInformationrComponent::UnEquipmenServer_Validate(ASwordFlyBaseWeapon* Item, int32 var)
{
	return true;
}

void USwordFlyInformationrComponent::UnEquipmenNetMulticast_Implementation(ASwordFlyBaseWeapon* Item, int32 var)
{
	
}

void USwordFlyInformationrComponent::Putbackpack(ABaseItem* Item, int32 var)
{
	PutbackpackServer(Item,var);
}

void USwordFlyInformationrComponent::PutbackpackNetMulticast_Implementation(ABaseItem* Item, int32 var)
{
	
}

void USwordFlyInformationrComponent::PutbackpackServer_Implementation(ABaseItem* Item, int32 var)
{
	PutbackpackNetMulticast(Item,var);
}

bool USwordFlyInformationrComponent::PutbackpackServer_Validate(ABaseItem* Item, int32 var)
{
	return true;
}

void USwordFlyInformationrComponent::EquipmentServerNetMulticast_Implementation(ASwordFlyBaseWeapon* Item)
{
	
	
}

void USwordFlyInformationrComponent::EquipmentServer_Implementation(ASwordFlyBaseWeapon* Item)
{
	EquipmentServerNetMulticast(Item);
}

bool USwordFlyInformationrComponent::EquipmentServer_Validate(ASwordFlyBaseWeapon* Item)
{
	return  true;
}

void USwordFlyInformationrComponent::OutPutbackpack(ABaseItem* Item, int32 var)
{
	OutPutbackpackServer(Item,var);
}

void USwordFlyInformationrComponent::OutPutbackpackNetMulticast_Implementation(ABaseItem* Item, int32 var)
{
	//if (GetOwnerRole()!=ROLE_Authority)return;
	if (!Item)return;
	if (var<=0)return;
/*	switch (Item->GetItemType())
	{

	default: break;;
	}*/
	for (auto& thisItem:PackItmeArray)
	{
		if (thisItem.thisItem->ActorID==Item->ActorID)
		{
			if (thisItem.thisItemnumber>0)
			{
				if (thisItem.thisItemnumber-var<=0)
				{
					//thisItem.thisItemnumber=0;
					//PackItmeArray.Remove(thisItem);
				}else
				{
					thisItem.thisItemnumber=thisItem.thisItemnumber-var;
				}
			}
		}
	}
}

void USwordFlyInformationrComponent::OutPutbackpackServer_Implementation(ABaseItem* Item, int32 var)
{
	OutPutbackpackNetMulticast(Item,var);
}

bool USwordFlyInformationrComponent::OutPutbackpackServer_Validate(ABaseItem* Item, int32 var)
{
	return true;
}
