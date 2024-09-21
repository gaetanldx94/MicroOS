#ifndef PROCESS_H
#define PROCESS_H

#include <cstdint>

// États possibles pour un processus
enum class ProcessState
{
	FREE,	   // Processus non alloué
	READY,	   // Prêt à être exécuté
	RUNNING,   // En cours d'exécution
	BLOCKED,   // Bloqué (attente d'un événement)
	TERMINATED // Terminé
};

// Structure représentant un processus
struct Process
{
	uint32_t pid;			 // Identifiant du processus
	ProcessState state;		 // État du processus
	uint32_t stackPointer;	 // Pointeur de pile
	uint32_t programCounter; // Compteur de programme
};

// Déclarations externes
extern "C"
{
	void init_process_manager();										// Initialise le gestionnaire de processus
	int create_process(uint32_t entry_point, const char *process_name); // Crée un processus et retourne son PID
	void context_switch();												// Change de contexte entre les processus
	void terminate_process(uint32_t pid);								// Termine un processus par son PID
	void process_entry(uint32_t pid, const char *process_name);			// Ajout de la fonction ici
}

#endif // PROCESS_H
