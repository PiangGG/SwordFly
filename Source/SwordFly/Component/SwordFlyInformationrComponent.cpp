// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlyInformationrComponent.h"
#include "SwordFly/Itme/Weapons/Sword.h"
#include "SwordFly/Itme/Weapons/SwordFlyBaseWeapon.h"

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

void USwordFlyInformationrComponent::Equipment(ABaseItem* Item)
{
	EquipmentServer(Item);
}

void USwordFlyInformationrComponent::UnEquipmen(ABaseItem* Item, int32 var)
{
	UnEquipmenServer(Item,var);
}

void USwordFlyInformationrComponent::UnEquipmenServer_Implementation(ABaseItem* Item, int32 var)
{
	UnEquipmenNetMulticast(Item,var);
}

bool USwordFlyInformationrComponent::UnEquipmenServer_Validate(ABaseItem* Item, int32 var)
{
	return true;
}

void USwordFlyInformationrComponent::UnEquipmenNetMulticast_Implementation(ABaseItem* Item, int32 var)
{
	if (GetOwnerRole()!=ROLE_Authority)return;
	if (!Item)return;
	if (var<=0)return;
	ASwordFlyBaseWeapon* thisItem=Cast<ASwordFlyBaseWeapon>(Item);
	if (!thisItem)return;
	switch (thisItem->GetWeaponType())
	{
		case EWeaponType::ESword:
		{
			if (CurrentWeaponArray.IsValidIndex(0))
			{
				CurrentWeaponArray.RemoveAt(0);
			}
			break;
		}
		case EWeaponType::EBow:
			{
				if (CurrentWeaponArray.IsValidIndex(1))
				{
					CurrentWeaponArray.RemoveAt(1);
				}
				break;
			}
	default:
		break;
	}
	//thisItem->UnEquipment(Cast<ASwordFlyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()));
}

void USwordFlyInformationrComponent::Putbackpack(ABaseItem* Item, int32 var)
{
	PutbackpackServer(Item,var);
}

void USwordFlyInformationrComponent::PutbackpackNetMulticast_Implementation(ABaseItem* Item, int32 var)
{
	if (GetOwnerRole()!=ROLE_Authority)return;
	if (!Item)return;
	if (var<=0)return;
	switch (Item->GetItemType())
	{
case EItmeType::EWeapon:
	{
		ASwordFlyBaseWeapon* thisItem=Cast<ASwordFlyBaseWeapon>(Item);
		if (!thisItem)return;
	
		switch (thisItem->GetWeaponType())
		{
		case EWeaponType::ESword:
			{
				if (CurrentWeaponArray.IsValidIndex(0))
				{
					//CurrentWeaponArray.Insert(*thisItem,0);
					Equipment(thisItem);
				}else
				{
					thisItem->SetActorHiddenInGame(true);
					for (auto& thisItmeArray:PackItmeArray)
					{
						if (thisItmeArray.thisItem->ItemName==thisItem->ItemName)
						{
							thisItmeArray.thisItemnumber=thisItmeArray.thisItemnumber+var;
							return;
						}
		
					}
					FPackItme newItme;
					newItme.thisItem=thisItem;
					newItme.thisItemnumber=var;
					PackItmeArray.Add(newItme);
					
				}
				break;
			}
		case EWeaponType::EBow:
			{
				if (CurrentWeaponArray.IsValidIndex(1))
				{
					//CurrentWeaponArray.Insert(*thisItem,0);
					Equipment(thisItem);
				}else
				{
					thisItem->SetActorHiddenInGame(true);
					for (auto& thisItmeArray:PackItmeArray)
					{
						if (thisItmeArray.thisItem->ItemName==thisItem->ItemName)
						{
							thisItmeArray.thisItemnumber=thisItmeArray.thisItemnumber+var;
							return;
						}
		
					}
					FPackItme newItme;
					newItme.thisItem=thisItem;
					newItme.thisItemnumber=var;
					PackItmeArray.Add(newItme);
				}
					
			}
		case EWeaponType::EOther:
			{
				if (!thisItem)return;
				{
					
				}
				thisItem->SetActorHiddenInGame(true);
				for (auto& thisItmeArray:PackItmeArray)
				{
					if (thisItmeArray.thisItem->ItemName==thisItem->ItemName)
					{
						thisItmeArray.thisItemnumber=thisItmeArray.thisItemnumber+var;
						return;
					}
		
				}
				FPackItme newItme;
				newItme.thisItem=thisItem;
				newItme.thisItemnumber=var;
				PackItmeArray.Add(newItme);
				break;
			}
		default:
			UE_LOG(LogTemp, Warning, TEXT("无法获取default"));
			break;
			;
		}
		break;;
	}
		default: break;
	}
	
	
}

void USwordFlyInformationrComponent::PutbackpackServer_Implementation(ABaseItem* Item, int32 var)
{
	PutbackpackNetMulticast(Item,var);
}

bool USwordFlyInformationrComponent::PutbackpackServer_Validate(ABaseItem* Item, int32 var)
{
	return true;
}

void USwordFlyInformationrComponent::EquipmentServerNetMulticast_Implementation(ABaseItem* Item)
{
	if (GetOwnerRole()!=ROLE_Authority)return;
	if (!Item)return;
	ASwordFlyBaseWeapon* thisItem=Cast<ASwordFlyBaseWeapon>(Item);
	if (!thisItem)return;
//	thisItem->Equipment(Cast<ASwordFlyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()));
}

void USwordFlyInformationrComponent::EquipmentServer_Implementation(ABaseItem* Item)
{
	EquipmentServerNetMulticast(Item);
}

bool USwordFlyInformationrComponent::EquipmentServer_Validate(ABaseItem* Item)
{
	return  true;
}

void USwordFlyInformationrComponent::OutPutbackpack(ABaseItem* Item, int32 var)
{
	OutPutbackpackServer(Item,var);
}

void USwordFlyInformationrComponent::OutPutbackpackNetMulticast_Implementation(ABaseItem* Item, int32 var)
{
	if (GetOwnerRole()!=ROLE_Authority)return;
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
