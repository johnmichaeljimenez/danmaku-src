#include "dialogue.h"
bool IsDialogueActive;

DialogueSet *currentDialogue;
int dialogueIndex;
int displayCharacter;
const char *displayMessage;
float dialogueTween;
static void (*endFunction)(void);

void OnDialogueHide(const char *id)
{
	IsDialogueActive = false;
	currentDialogue = NULL;
	dialogueIndex = 0;

	if (endFunction != NULL)
	{
		endFunction();
		endFunction = NULL;
	}
}

void DialogueShow(const char *dialogueID, void (*onEnd)(void))
{
	endFunction = onEnd;
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
	{
		endFunction = NULL;
		return;
	}

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
	DrawRectangle(0, 0, VIRTUAL_WIDTH, 120, (Color){10, 10, 10, 150 * dialogueTween});
	DrawTextEx(DefaultFont, displayMessage, (Vector2){12, 12}, 35, 0, (Color){255, 255, 255, 255 * dialogueTween});
}

static const Dialogue tutorial_start_lines[] = {
	{.CharacterID = -1, .Message = "[Riko] Hibiki, you got a minute?\nIt's two days before the event, but wanna test out the\nfireworks dodgeball?"},
	{.CharacterID = -1, .Message = "[Hibiki] Huh, already!? I haven't even finished hearing all\nthe rules yet!"},
	{.CharacterID = -1, .Message = "[Riko] It's fiiine, it's fiiine! Even if you get hit,\nit's totally safety-approved!"},
	{.CharacterID = -1, .Message = "[Riko] They just flash really bright!"},
	{.CharacterID = -1, .Message = "[Hibiki] The way you said 'just' is not reassuring at all..."},
	{.CharacterID = -1, .Message = "[Riko] Well, if it does seem dangerous, as staff I'll put a\nstop to it!\n...Probably! Uh, I'll try my best?"},
	{.CharacterID = -1, .Message = "[Hibiki] Can we ban words like 'probably' and 'try' from\nstaff vocabulary...?"},
	{.CharacterID = -1, .Message = "[Riko] You'll be fine! With your footwork, you can\ndodge fireworks and flower petals alike!"},
	{.CharacterID = -1, .Message = "[Riko] Let's practice here so you can be the coolest\n'Firework Dodger' at the festival!"},
	{.CharacterID = -1, .Message = "[Hibiki] ...If you put it like that, I guess I have no\nchoice."},
	{.CharacterID = -1, .Message = "[Hibiki] All right, Miss Matsuriya, you'd better watch\nclosely, okay? Safety check and hype!"},
	{.CharacterID = -1, .Message = "[Riko] Leave it to me!"},
	{.CharacterID = -1, .Message = "[Riko] Safety check, good!"},
	{.CharacterID = -1, .Message = "[Riko] Hype level, good!"},
	{.CharacterID = -1, .Message = "[Riko] Then... Hanabisaki: Flowerburst tutorial...\nSTART!"}};

	
static const Dialogue tutorial_end_lines[] = {
	{.CharacterID = -1, .Message = "[Hibiki] Haa... haa..."},
	{.CharacterID = -1, .Message = "[Hibiki] This might be... more manageable than I\nthought... maybe."},
	{.CharacterID = -1, .Message = "[Riko] Oooh! That's our Hibiki! You really dodged all the\nfireworks and petals so neatly!"},
	{.CharacterID = -1, .Message = "[Hibiki] Honestly, at first I thought 'no way I can do\nthis'..."},
	{.CharacterID = -1, .Message = "[Hibiki] but once I tried it... it's actually kind of fun...\nmaybe a little."},
	{.CharacterID = -1, .Message = "[Riko] Right, right? In the real thing it'll be even more\ncolorful and over-the-top, you know!"},
	{.CharacterID = -1, .Message = "[Hibiki] ...Don't say something scary all of a sudden."},
	{.CharacterID = -1, .Message = "[Hibiki] But... now that I've come this far,\nI'll see it through."},
};
	
static const Dialogue tutorial_retry_1[] = {
	{.CharacterID = -1, .Message = "[Riko] Welcome back, Hibiki. Right this way to\nthe 'Courage to Try Again' lane~."},
	{.CharacterID = -1, .Message = "[Hibiki] Don't give it weird names...\nBut, well, it's not over yet anyway."},
	{.CharacterID = -1, .Message = "[Riko] Yep, that's the spirit. You can fall as many times\nas you like in practice!"},
	{.CharacterID = -1, .Message = "[Riko] If you nail it on festival day, it all gets wiped\nclean!"},
	{.CharacterID = -1, .Message = "[Hibiki] Then I'll just make up for every fall when the\nfestival comes."},
	{.CharacterID = -1, .Message = "[Hibiki] ...All right. One more round, please."},
	{.CharacterID = -1, .Message = "[Riko] Roger! Fireworks, ready to launch! Dodgeball\nreflexes, ready too?"},
	{.CharacterID = -1, .Message = "[Hibiki] ...Probably."},
};

static const Dialogue tutorial_retry_2[] = {
	{.CharacterID = -1, .Message = "[Riko] All right, one retry coming up~. This time,\nlet's go for no mistakes, okay?"},
	{.CharacterID = -1, .Message = "[Hibiki] Easy for you to say... I'll do it though."},
};

DialogueSet Dialogues[DIALOGUE_COUNT] =
	{
		{.ID = "tutorial-start",
		 .Lines = (Dialogue *)tutorial_start_lines,
		 .Count = arraySize(tutorial_start_lines)},
		{.ID = "tutorial-end",
		 .Lines = (Dialogue *)tutorial_end_lines,
		 .Count = arraySize(tutorial_end_lines)},
		{.ID = "tutorial-retry-1",
		 .Lines = (Dialogue *)tutorial_retry_1,
		 .Count = arraySize(tutorial_retry_1)},
		{.ID = "tutorial-retry-2",
		 .Lines = (Dialogue *)tutorial_retry_2,
		 .Count = arraySize(tutorial_retry_2)}
	};