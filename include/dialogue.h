#pragma once
#include "main.h"

typedef struct Character
{
	int CharacterID;
	const char* Name;
	const char* ShortName;
} Character;

typedef struct Dialogue
{
	int CharacterID;
	const char *Message;
} Dialogue;

typedef struct DialogueSet
{
	const char *ID;
	Dialogue *Lines;
	int Count;
} DialogueSet;

#define DIALOGUE_COUNT 128
extern DialogueSet Dialogues[DIALOGUE_COUNT];

#define CHARACTER_COUNT 16
extern Character Characters[CHARACTER_COUNT];

extern bool IsDialogueActive;

void DialogueShow(const char *dialogueID, void(*onEnd)(void));
void DialogueUpdate(float dt);
void DialogueRender(float dt);