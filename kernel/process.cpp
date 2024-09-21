#include "process.h"
#include "kernel.h" // Inclure kernel.h pour utiliser kernel_printf

// Déclaration de la fonction d'allocation
extern "C" void *allocate_memory(std::size_t size);
const int MAX_PROCESSES = 256;
Process process_table[MAX_PROCESSES];
int current_process = -1;

// Fonction pour initialiser le gestionnaire de processus
void init_process_manager()
{
	for (int i = 0; i < MAX_PROCESSES; i++)
	{
		process_table[i].pid = -1;					  // Indique qu'il n'y a pas de processus
		process_table[i].state = ProcessState::READY; // Initialiser l'état à READY
	}
}

int create_process(uint32_t entry_point, const char *process_name)
{
	for (int i = 0; i < MAX_PROCESSES; i++)
	{
		if (process_table[i].state == ProcessState::READY)
		{ // Vérifier l'état READY
			// Initialisation du processus
			process_table[i].pid = i;
			process_table[i].stackPointer = (uint32_t)allocate_memory(4096);
			if (!process_table[i].stackPointer)
			{
				return -1; // Échec d'allocation
			}
			process_table[i].programCounter = entry_point;	// Point d'entrée
			process_table[i].state = ProcessState::RUNNING; // Marquer comme en cours d'exécution
			return i;										// Retourner le PID
		}
	}
	return -1; // Aucun espace libre trouvé
}

// Fonction pour changer de contexte
void context_switch()
{
	if (current_process != -1)
	{
		// Sauvegarde l'état du processus actuel
		process_table[current_process].state = ProcessState::READY;
	}

	// Cherche le prochain processus prêt
	do
	{
		current_process = (current_process + 1) % MAX_PROCESSES;
	} while (process_table[current_process].state != ProcessState::READY);

	// Récupère l'état du nouveau processus
	process_table[current_process].state = ProcessState::RUNNING;
}

// Fonction de formatage simple
void format_process_message(char *buffer, int size, const char *process_name, uint32_t pid)
{
	// Assurez-vous que la taille du buffer est suffisante
	if (size > 0)
	{
		// Copier le nom du processus dans le buffer
		int written = 0;
		while (*process_name && written < size - 1)
		{
			buffer[written++] = *process_name++;
		}
		buffer[written++] = ' ';
		buffer[written++] = 'a';
		buffer[written++] = 'v';
		buffer[written++] = 'e';
		buffer[written++] = 'c';
		buffer[written++] = ' ';
		buffer[written++] = 'P';
		buffer[written++] = 'I';
		buffer[written++] = 'D';
		buffer[written++] = ':';
		buffer[written++] = ' ';

		// Convertir PID en chaîne
		int pid_copy = pid;
		if (pid_copy == 0)
		{
			buffer[written++] = '0';
		}
		else
		{
			char pid_buffer[10];
			int pid_length = 0;
			while (pid_copy > 0 && pid_length < 10)
			{
				pid_buffer[pid_length++] = '0' + (pid_copy % 10);
				pid_copy /= 10;
			}
			// Inverser la chaîne du PID
			for (int i = pid_length - 1; i >= 0; i--)
			{
				if (written < size - 1)
				{
					buffer[written++] = pid_buffer[i];
				}
			}
		}
		buffer[written] = '\0'; // Terminer la chaîne
	}
}

extern "C" void process_entry(uint32_t pid, const char *process_name)
{
	char buffer[100]; // Buffer pour le message formaté

	// Utiliser la fonction de formatage
	format_process_message(buffer, sizeof(buffer), process_name, pid);
	kernel_printf(buffer);

	// Boucle infinie pour simuler le travail du processus
	while (1)
	{
		// Ici, vous pourriez mettre le code que ce processus doit exécuter
	}
}
