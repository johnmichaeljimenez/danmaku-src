#pragma once
#include "main.h"

typedef struct Dialogue
{
	int CharacterID;
	const char* Message;
} Dialogue;

typedef struct DialogueSet
{
	const char* ID;
	Dialogue *Lines;
	int Count;
} DialogueSet;


#define DIALOGUE_COUNT 128
extern DialogueSet Dialogues[DIALOGUE_COUNT];
extern bool IsDialogueActive;

void DialogueShow(const char* dialogueID);
void DialogueUpdate(float dt);
void DialogueRender(float dt);