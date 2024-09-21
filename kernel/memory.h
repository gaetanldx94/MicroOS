#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <cstddef>

struct MemoryBlock {
    size_t size;          // Taille du bloc
    bool free;           // Est-ce que le bloc est libre ?
    MemoryBlock* next;   // Pointeur vers le prochain bloc
};

// Déclarations de fonctions
extern "C" {
    void init_memory();                  // Fonction pour initialiser la mémoire
    void* allocate_memory(size_t size);  // Fonction pour allouer de la mémoire
    void free_memory(void* ptr);         // Fonction pour libérer de la mémoire
}

#endif // MEMORY_H
