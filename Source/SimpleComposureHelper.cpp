#include "SimpleComposureHelper.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"

//-----------------------------------------------------------------------------

TArray<AActor*> USimpleComposureHelper::AllActors;

//-----------------------------------------------------------------------------

USimpleComposureHelper::USimpleComposureHelper( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
}

//-----------------------------------------------------------------------------

static void __InternalSetSelfShadowOnly( const AActor* InActor, bool InSelfShadowOnly )
{
    if ( InActor != nullptr ) {
        auto primitiveComp = Cast<UPrimitiveComponent>( InActor->GetComponentByClass( UPrimitiveComponent::StaticClass() ) );
        if ( primitiveComp != nullptr ) {
            primitiveComp->bSelfShadowOnly = InSelfShadowOnly;
            primitiveComp->SetCastInsetShadow( InSelfShadowOnly );
        }
    }
}

void USimpleComposureHelper::SetSelfShadowOnlys( const UObject* InWorldContextObject, const TArray<AActor*> InActorList, bool InSelfShadowOnly, bool InExcluded, bool InClearAllActors )
{
    if ( InExcluded ) {
        if ( ensure( !InClearAllActors && AllActors.Num() == 0 ) )
            UGameplayStatics::GetAllActorsOfClass( InWorldContextObject, AActor::StaticClass(), AllActors );
        
        for ( auto actor : AllActors ) {
            if ( !InActorList.Contains( actor ) )
                __InternalSetSelfShadowOnly( actor, InSelfShadowOnly );
        }
        
        if ( InClearAllActors )
            AllActors.Empty();
    }
    else {
        for ( auto actor : InActorList )
            __InternalSetSelfShadowOnly( actor, InSelfShadowOnly );
    }
}
