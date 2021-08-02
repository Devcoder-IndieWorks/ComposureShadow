#include "SimpleComposureHelper.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

USimpleComposureHelper::USimpleComposureHelper( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
}

void USimpleComposureHelper::SetSelfShadowOnly( const AActor* InActor, bool InEnableSelfShadowOnly )
{
    if ( InActor != nullptr ) {
        auto primitiveComp = Cast<UPrimitiveComponent>( InActor->GetComponentByClass( UPrimitiveComponent::StaticClass() ) );
        if ( primitiveComp != nullptr ) {
            primitiveComp->bSelfShadowOnly = InEnableSelfShadowOnly;
            primitiveComp->SetCastInsetShadow(InEnableSelfShadowOnly);
        }
    }
}
