#include "dialogue.h"
bool IsDialogueActive;

DialogueSet *currentDialogue;
int dialogueIndex;
int displayCharacter;
const char *displayMessage;
float dialogueTween;

void OnDialogueHide(const char *id)
{
	IsDialogueActive = false;
	currentDialogue = NULL;
	dialogueIndex = 0;
}

void DialogueShow(const char *dialogueID)
{
	IsDialogueActive = false;
	currentDialogue = NULL;
	dialogueIndex = 0;

	for (int i = 0; i < DIALOGUE_COUNT; i++)
	{
		DialogueSet *st = &Dialogues[i];
		if (TextIsEqual(st->ID, dialogueID))
		{
			currentDialogue = st;

			displayCharacter = currentDialogue->Lines[dialogueIndex].CharacterID;
			displayMessage = currentDialogue->Lines[dialogueIndex].Message;

			break;
		}
	}

	if (currentDialogue == NULL)
		return;

	IsDialogueActive = true;
	TweenManager_AddFloatFrom(&dialogueTween, 0, 1, 0.5f, EASING_EASEINQUAD, "Dialogue", NULL);
}

void DialogueUpdate(float dt)
{
	if (dialogueTween < 1 || dialogueIndex >= currentDialogue->Count)
		return;

	if (IsPointerPressed())
	{
		dialogueIndex++;
		if (dialogueIndex >= currentDialogue->Count)
		{
			TweenManager_AddFloatFrom(&dialogueTween, 1, 0, 0.5f, EASING_EASEINQUAD, "Dialogue", OnDialogueHide);
		}
		else
		{
			displayCharacter = currentDialogue->Lines[dialogueIndex].CharacterID;
			displayMessage = currentDialogue->Lines[dialogueIndex].Message;
		}
	}
}

void DialogueRender(float dt)
{
	DrawRectangle(0, 0, VIRTUAL_WIDTH, 100, (Color){10, 10, 10, 150 * dialogueTween});
	DrawText(displayMessage, 12, 12, 30, (Color){255, 255, 255, 255 * dialogueTween});
}



static const Dialogue test_lines[] = {
    { .CharacterID = 1, .Message = "Hello 1" },
    { .CharacterID = 2, .Message = "Hello 2" },
    { .CharacterID = 1, .Message = "End!" },
};


DialogueSet Dialogues[DIALOGUE_COUNT] = 
{
	{
        .ID    = "test",
        .Lines = (Dialogue*)test_lines,
        .Count = arraySize(test_lines)
    }
};