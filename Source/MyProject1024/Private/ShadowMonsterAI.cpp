// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowMonsterAI.h"

AShadowMonsterAI::AShadowMonsterAI()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//make sure the function order is right
	ActionFunctions.Add(std::bind(&AShadowMonsterAI::Pray, this));

	EndActionFunctions.Add(std::bind(&AShadowMonsterAI::EndPray, this));

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}

void AShadowMonsterAI::Chase()
{
	EndAttack();
	IsChasing = true;
}

void AShadowMonsterAI::EndChase()
{
	IsChasing = false;
}

void AShadowMonsterAI::Attack()
{
	EndChase();
	IsAttacking = true;
}

void AShadowMonsterAI::EndAttack()
{
	IsAttacking = false;
}

void AShadowMonsterAI::Pray()
{
	IsPraying = true;
}

void AShadowMonsterAI::EndPray()
{
	IsPraying = false;
}