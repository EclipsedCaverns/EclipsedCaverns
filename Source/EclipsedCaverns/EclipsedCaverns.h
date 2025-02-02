// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 디버깅을 위한 매크로 구현

#define CALLINFO (FString(__FUNCTION__)+TEXT("(")+FString::FromInt(__LINE__)+TEXT(")"))

#define PRINT_CALLINFO() UE_LOG(LogTemp,Warning, TEXT("%s"),*CALLINFO)