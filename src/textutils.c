#include "textutils.h"

/* From Raylib examples website */

void DrawTextStyled(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color color)
{
	// Text inline styling strategy used: [ ] delimiters for format
	// - Define foreground color:      [cRRGGBBAA]
	// - Define background color:      [bRRGGBBAA]
	// - Reset formating:              [r]
	// Example: [bAA00AAFF][cFF0000FF]red text on gray background[r] normal text

	if (font.texture.id == 0)
		font = GetFontDefault();

	int textLen = TextLength(text);

	Color colFront = color;
	Color colBack = BLANK;
	int backRecPadding = 4; // Background rectangle padding

	float textOffsetY = 0.0f;
	float textOffsetX = 0.0f;
	float textLineSpacing = 0.0f;
	float scaleFactor = fontSize / font.baseSize;

	for (int i = 0; i < textLen;)
	{
		int codepointByteCount = 0;
		int codepoint = GetCodepointNext(&text[i], &codepointByteCount);

		if (codepoint == '\n')
		{
			textOffsetY += (fontSize + textLineSpacing);
			textOffsetX = 0.0f;
		}
		else
		{
			if (codepoint == '[') // Process pipe styling
			{
				if (((i + 2) < textLen) && (text[i + 1] == 'r') && (text[i + 2] == ']')) // Reset styling
				{
					colFront = color;
					colBack = BLANK;

					i += 3;	  // Skip "[r]"
					continue; // Do not draw characters
				}
				else if (((i + 1) < textLen) && ((text[i + 1] == 'c') || (text[i + 1] == 'b')))
				{
					i += 2; // Skip "[c" or "[b" to start parsing color

					// Parse following color
					char colHexText[9] = {0};
					const char *textPtr = &text[i]; // Color should start here, let's see...

					int colHexCount = 0;
					while ((textPtr != NULL) && (textPtr[colHexCount] != '\0') && (textPtr[colHexCount] != ']'))
					{
						if (((textPtr[colHexCount] >= '0') && (textPtr[colHexCount] <= '9')) ||
							((textPtr[colHexCount] >= 'A') && (textPtr[colHexCount] <= 'F')) ||
							((textPtr[colHexCount] >= 'a') && (textPtr[colHexCount] <= 'f')))
						{
							colHexText[colHexCount] = textPtr[colHexCount];
							colHexCount++;
						}
						else
							break; // Only affects while loop
					}

					// Convert hex color text into actual Color
					unsigned int colHexValue = strtoul(colHexText, NULL, 16);
					if (text[i - 1] == 'c')
					{
						colFront = GetColor(colHexValue);
						colFront.a *= (float)color.a / 255.0f;
					}
					else if (text[i - 1] == 'b')
					{
						colBack = GetColor(colHexValue);
					}

					i += (colHexCount + 1); // Skip color value retrieved and ']'
					continue;				// Do not draw characters
				}
			}

			int index = GetGlyphIndex(font, codepoint);
			float increaseX = 0.0f;

			if (font.glyphs[index].advanceX == 0)
				increaseX = ((float)font.recs[index].width * scaleFactor + spacing);
			else
				increaseX += ((float)font.glyphs[index].advanceX * scaleFactor + spacing);

			// Draw background rectangle color (if required)
			if (colBack.a > 0)
				DrawRectangleRec((Rectangle){position.x + textOffsetX, position.y + textOffsetY - backRecPadding, increaseX, fontSize + 2 * backRecPadding}, colBack);

			if ((codepoint != ' ') && (codepoint != '\t'))
			{
				DrawTextCodepoint(font, codepoint, (Vector2){position.x + textOffsetX, position.y + textOffsetY}, fontSize, colFront);
			}

			textOffsetX += increaseX;
		}

		i += codepointByteCount;
	}
}

// Measure inline styled text
// NOTE: Measuring styled text requires skipping styling data
// WARNING: Not considering line breaks
static Vector2 MeasureTextStyled(Font font, const char *text, float fontSize, float spacing)
{
	Vector2 textSize = {0};

	if ((font.texture.id == 0) || (text == NULL) || (text[0] == '\0'))
		return textSize; // Security check

	int textLen = TextLength(text); // Get size in bytes of text

	float textWidth = 0.0f;
	float textHeight = fontSize;
	float scaleFactor = fontSize / (float)font.baseSize;

	int codepoint = 0; // Current character
	int index = 0;	   // Index position in sprite font
	int validCodepointCounter = 0;

	for (int i = 0; i < textLen;)
	{
		int codepointByteCount = 0;
		codepoint = GetCodepointNext(&text[i], &codepointByteCount);

		if (codepoint == '[') // Ignore pipe inline styling
		{
			if (((i + 2) < textLen) && (text[i + 1] == 'r') && (text[i + 2] == ']')) // Reset styling
			{
				i += 3;	  // Skip "[r]"
				continue; // Do not measure characters
			}
			else if (((i + 1) < textLen) && ((text[i + 1] == 'c') || (text[i + 1] == 'b')))
			{
				i += 2; // Skip "[c" or "[b" to start parsing color

				const char *textPtr = &text[i]; // Color should start here, let's see...

				int colHexCount = 0;
				while ((textPtr != NULL) && (textPtr[colHexCount] != '\0') && (textPtr[colHexCount] != ']'))
				{
					if (((textPtr[colHexCount] >= '0') && (textPtr[colHexCount] <= '9')) ||
						((textPtr[colHexCount] >= 'A') && (textPtr[colHexCount] <= 'F')) ||
						((textPtr[colHexCount] >= 'a') && (textPtr[colHexCount] <= 'f')))
					{
						colHexCount++;
					}
					else
						break; // Only affects while loop
				}

				i += (colHexCount + 1); // Skip color value retrieved and ']'
				continue;				// Do not measure characters
			}
		}
		else if (codepoint != '\n')
		{
			index = GetGlyphIndex(font, codepoint);

			if (font.glyphs[index].advanceX > 0)
				textWidth += font.glyphs[index].advanceX;
			else
				textWidth += (font.recs[index].width + font.glyphs[index].offsetX);

			validCodepointCounter++;
			i += codepointByteCount;
		}
	}

	textSize.x = textWidth * scaleFactor + (validCodepointCounter - 1) * spacing;
	textSize.y = textHeight;

	return textSize;
}