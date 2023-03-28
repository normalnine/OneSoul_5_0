// Fill out your copyright notice in the Description page of Project Settings.


#include "OneSoulGameInstance.h"

UOneSoulGameInstance::UOneSoulGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempData(TEXT("DataTable'/Game/KDH/Recources/DB/DT_Status.DT_Status'"));
	if (tempData.Succeeded())
	{
		UDataTable* data = tempData.Object;
		//���� (���� ����)
		TArray<FName> rowNames = data->GetRowNames();
		for (int32 i = 0; i < rowNames.Num(); i++)
		{
			//�� �࿡ ���� �����͸� ��������
			FPlayerStatus* status = data->FindRow<FPlayerStatus>(rowNames[i], TEXT(""));
			
			//�츮�� ���� Array �־�����
			statusData.Add(*status);
		}
	}
}

// void UOneSoulGameInstance::Init()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("curr level : %d"), statusData[0].level);
// }
