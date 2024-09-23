#include "includes/keyboard.h"
#include "includes/interrupts.h"
#include "includes/video.h"
#include <cstdint>

extern "C" uint8_t inb(uint16_t port);
extern "C" void keyboard_handler();

bool key_pressed[256] = {false};

char get_char()
{
	uint8_t scancode = inb(0x60);

	if (scancode & 0x80)
	{
		key_pressed[scancode & 0x7F] = false;
		return '\0';
	}

	if (key_pressed[scancode])
	{
		return '\0';
	}

	key_pressed[scancode] = true;

	switch (scancode)
	{
	// Ligne AZERTY principale
	case 0x10:
		return 'a'; // A
	case 0x11:
		return 'z'; // Z
	case 0x12:
		return 'e'; // E
	case 0x13:
		return 'r'; // R
	case 0x14:
		return 't'; // T
	case 0x15:
		return 'y'; // Y
	case 0x16:
		return 'u'; // U
	case 0x17:
		return 'i'; // I
	case 0x18:
		return 'o'; // O
	case 0x19:
		return 'p'; // P

	// Ligne "qsdf"
	case 0x1E:
		return 'q'; // Q
	case 0x1F:
		return 's'; // S
	case 0x20:
		return 'd'; // D
	case 0x21:
		return 'f'; // F
	case 0x22:
		return 'g'; // G
	case 0x23:
		return 'h'; // H
	case 0x24:
		return 'j'; // J
	case 0x25:
		return 'k'; // K
	case 0x26:
		return 'l'; // L
	case 0x27:
		return 'm'; // M

	// Ligne "wxcvbn"
	case 0x2C:
		return 'w'; // W
	case 0x2D:
		return 'x'; // X
	case 0x2E:
		return 'c'; // C
	case 0x2F:
		return 'v'; // V
	case 0x30:
		return 'b'; // B
	case 0x31:
		return 'n'; // N

	// Espace et Entrée
	case 0x39:
		return ' '; // Espace
	case 0x1C:
		return '\n'; // Entrée

	case 0x02:
		return '&'; // 1 (&)
	case 0x03:
		return '\x82'; // 2 (é) -> é (ASCII étendu 130)
	case 0x04:
		return '"'; // 3 (")
	case 0x05:
		return '\''; // 4 (')
	case 0x06:
		return '('; // 5 (()
	case 0x07:
		return '-'; // 6 (-)
	case 0x08:
		return '\x8A'; // 7 (è) -> è (ASCII étendu 138)
	case 0x09:
		return '\x87'; // 9 (ç) -> ç (ASCII étendu 135)
	case 0x0A:
		return '\x85'; // 0 (à) -> à (ASCII étendu 133)

	// Touches spéciales
	case 0x0F:
		return '\t'; // Tabulation
	case 0x01:
		return 27; // Echap (ASCII 27 pour Escape)
	case 0x0E:
		return '\b'; // Retour arrière (Backspace)

	// Suppr, Ctrl, Alt, Shift, etc.
	case 0x7F:
		return 127; // Suppr (ASCII 127 pour Delete)
	case 0x2A:
		return 0; // Shift gauche
	case 0x36:
		return 0; // Shift droit
	case 0x1D:
		return 0; // Ctrl gauche
	case 0x38:
		return 0; // Alt gauche
	case 0x3A:
		return 0; // Verrouillage majuscules (Caps Lock)

	default:
		return '\0'; // Caractère nul si scancode non reconnu
	}
}

void keyboard_install()
{
	idt_set_gate(33, (uint32_t)keyboard_handler, 0x08, 0x8E);
	test_message("Clavier installe.", true);
}

extern "C" void keyboard_handler()
{
	uint8_t scancode = inb(0x60);
	kernel_printf("Key pressed\n");
}
