#include "includes/kernel.h"
#include "includes/memory.h"
#include "includes/interrupts.h"
#include "includes/keyboard.h"
#include "includes/paging.h"
#include "includes/process.h"
#include "includes/file.h"
#include "includes/video.h"
#include "includes/shell.h"

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber)
{
	clear_screen();
	kernel_printf("Initialisation du kernel...");

	kernel_printf("\nInitialisation de la memoire...");
	init_memory();
	test_message("Memoire initialisee.", true);

	kernel_printf("\nInstallation de l'IDT...");
	idt_install();
	test_message("IDT installee.", true);

	kernel_printf("\nInstallation du clavier...");
	keyboard_install();
	test_message("Clavier installe.", true);

	kernel_printf("\nMise en place de la pagination...");
	setup_paging();
	test_message("Pagination configuree.", true);

	kernel_printf("\nInitialisation du gestionnaire de processus...");
	init_process_manager();
	test_message("Gestionnaire de processus initialise.", true);

	kernel_printf("\nTest d'allocation de memoire...");
	void *ptr = allocate_memory(64);
	if (ptr)
	{
		test_message("64 octets de memoire alloues avec succes.", true);
		free_memory(ptr);
		test_message("Memoire liberee avec succes.", true);
	}
	else
	{
		test_message("Echec de l'allocation de memoire.", false);
	}

	kernel_printf("\nTest de creation de fichier...");
	const char *test_message_content = "Ceci est un message de test.";
	if (create_file(root_directory, "monfichier.txt", test_message_content))
	{
		test_message("Fichier cree avec succes.", true);
	}
	else
	{
		test_message("Echec de la creation du fichier.", false);
	}

	kernel_printf("\nDemarrage du shell...");
	kernel_printf("\n\n");
	shell();

	while (true)
	{
		context_switch();
	}
}