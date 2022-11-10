// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Components/ArrowComponent.h>
#include "Net/UnrealNetwork.h"
#include "MegaPlayerPawn.h"
#include "Projectile.h"
#include "PaperSpriteComponent.h"

#include "Components/AudioComponent.h"
// Sets default values
//Week 9 Lab: Audio Component is added to the weapon
AWeaponActor::AWeaponActor()
    :Super()
{
    FireRate = 10.0f;
    MaximumAmmo = 300;
    CurrentAmmo = 10;
    Muzzle = CreateDefaultSubobject<UArrowComponent>("Muzzle");
    Muzzle->SetupAttachment(RootComponent);


    //CREATE the AudioComponent
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
    //SET bAutoActivate on the AudioComponent to false
    AudioComponent->bAutoActivate = false;
    //CALL SetRelativeLocation() on AudioComponent passing in FVector(0.0f, 0.0f, 0.0f)
    AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    //ATTACH the AudioComponent to the RootComponent
    AudioComponent->SetupAttachment(RootComponent);
}

//Week 9 Lab: Equip the weapon by enabling and attaching it to the Characters Sprite. Look at the sprite asset for the Socket to attach to
void AWeaponActor::Equip(class AMegaPlayerPawn* Character)
{
    //ENABLE this Weapon
    this->Enable();

    //SET owner of this weapon to Character. There is a function in AActor that does this
    SetOwner(Character);

    //ATTACH weapon to the Character's PlayerSprite at WeaponSocket.
    //this->AttachToActor(Character, FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
}

//Week 9 Lab: UnEquip the weapon by disabling and detaching it from the Characters Sprite.
void AWeaponActor::UnEquip()
{
    //DISABLE this Weapon
    this->Disable();

    //UNSET owner of the weapon. Set the Owner to Null.
    SetOwner(nullptr);
    //DETACH weapon from the character's mesh.

}


FVector AWeaponActor::GetMuzzleLocation() const
{
    return Muzzle->GetComponentToWorld().GetLocation();
}

//Week 9 Lab:  Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
    Super::BeginPlay();

    //CALL SetSound() on AudioComponent passing in FireSound
    AudioComponent->SetSound(FireSound);
}


// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
//Week 9 Lab: Spawn the Projectile. Keep in mind CurrentAmmo count and sound
void AWeaponActor::Fire()
{
    //IF CurrentAmmo greater than 0
    if (CurrentAmmo > 0)
    {
        //IF the AudioComponent is Not Playing a Sound
        if (!AudioComponent->IsPlaying())
        {
            AudioComponent->Play();
            //CALL Play()

        }
        //ENDIF
        SpawnProjectile();
        //CALL SpawnProjectile()

    }
}

//Week 9 Lab: Spawn the projectile
void AWeaponActor::SpawnProjectile()
{
    // Spawn a projectile.

    //CREATE a variable of type FActorSpawnParameters called ActorSpawnParams
    FActorSpawnParameters ActorSpawnParams;
    //SET SpawnCollisionHandlingOverride on ActorSpawnParams passing in ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
    //SET Owner on ActorSpawnParams to this object
    ActorSpawnParams.Owner = this;
    //SET the Instigator on ActorSpawnParams to the Cast<>() of GetOwner()
    ActorSpawnParams.Instigator = Cast<APawn>(GetOwner());
    //DECLARE a variable called Transform of type FTransform and set it to the return value of Muzzle->GetComponentTransform()
    FTransform Transform = Muzzle->GetComponentTransform();
    //DECLARE a variable called projectile of type AProjectile* and assign it to the return value of GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Transform.GetLocation(), Transform.GetRotation().Rotator(), ActorSpawnParams)
    AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Transform.GetLocation(), Transform.GetRotation().Rotator(), ActorSpawnParams);
    //IF projectile is not null
    if(Projectile != nullptr)
    {
        //DECLARE a variable called direction of type FVector and assign it to the return value of FRotationMatrix(Transform.Rotator()).GetScaledAxis(EAxis::X)
        FVector Direction = FRotationMatrix(Transform.Rotator()).GetScaledAxis(EAxis::X);
        //CALL SetFireDirection() on projectile passing in direction
        Projectile->SetFireDirection(Direction);

    }
    //ENDIF
    CurrentAmmo--;
    //DECREMENT the ammo count.

}