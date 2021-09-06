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
    UFUNCTION(BlueprintCallable, Category="SimpleComposureHelperFunctions", meta=(WorldContext="InWorldContextObject"))
    static void SetSelfShadowOnlys( const UObject* InWorldContextObject, const TArray<class AActor*> InActorList, bool InSelfShadowOnly, bool InExcluded, bool InClearAllActors );

private:
    static TArray<class AActor*> AllActors;
};
