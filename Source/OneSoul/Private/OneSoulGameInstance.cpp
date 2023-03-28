// Fill out your copyright notice in the Description page of Project Settings.


#include "OneSoulGameInstance.h"

UOneSoulGameInstance::UOneSoulGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempData(TEXT("DataTable'/Game/KDH/Recources/DB/DT_Status.DT_Status'"));
	if (tempData.Succeeded())
	{
		UDataTable* data = tempData.Object;
		//갯수 (행의 갯수)
		TArray<FName> rowNames = data->GetRowNames();
		for (int32 i = 0; i < rowNames.Num(); i++)
		{
			//각 행에 대한 데이터를 가져오자
			FPlayerStatus* status = data->FindRow<FPlayerStatus>(rowNames[i], TEXT(""));
			
			//우리가 만든 Array 넣어주자
			statusData.Add(*status);
		}
	}
}

// void UOneSoulGameInstance::Init()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("curr level : %d"), statusData[0].level);
// }
