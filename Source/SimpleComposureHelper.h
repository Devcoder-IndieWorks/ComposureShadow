#pragma once
// Copright Vive-Studios, Inc.
#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleComposureHelper.generated.h"

UCLASS() 
class USimpleComposureHelper : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="SimpleComposureHelperFunctions")
    static void SetSelfShadowOnly( const class AActor* InActor, bool InEnableSelfShadowOnly );
};
