# MicroOS

**Auteur :** Gaétan Ledoux  
**Version :** 1.0  

## Description

MicroOS est un système d'exploitation minimaliste conçu pour l'apprentissage et l'expérimentation avec les concepts fondamentaux des systèmes d'exploitation. Ce projet vise à fournir une base solide pour explorer la gestion des processus, la gestion de la mémoire, et le développement de pilotes de périphériques. L'ISO de MicroOS est disponible à la racine du projet :)

## Architecture matérielle

Pour exécuter MicroOS, vous aurez besoin d'un ordinateur ou d'une machine virtuelle avec les spécifications suivantes :

### Matériel requis

- **Processeur :** Compatible x86 (32 bits).
- **Mémoire RAM :** Minimum 512 Mo.
- **Espace disque :** Minimum 100 Mo pour les fichiers du projet.

### Configuration recommandée

- **Processeur :** x86-64.
- **Mémoire RAM :** 1 Go ou plus.
- **Espace disque :** 500 Mo ou plus.

## Exécution sur une machine virtuelle

MicroOS peut être exécuté sur des machines virtuelles. Voici quelques options recommandées :

### VirtualBox

1. Téléchargez et installez [VirtualBox](https://www.virtualbox.org/).
2. Créez une nouvelle machine virtuelle avec les spécifications suivantes :
   - **Nom :** MicroOS
   - **Type :** Other
   - **Version :** Other (32-bit)
   - **Mémoire :** 512 Mo (ou plus)
   - **Disque dur :** 100 Mo (ou plus)
3. Montez l'image ISO de MicroOS dans la machine virtuelle.
4. Démarrez la machine virtuelle.

## Compilation

Pour compiler MicroOS, assurez-vous d'avoir un environnement de développement avec les outils suivants :

- **GNU Make**
- **GCC (GNU Compiler Collection)**

### Compilation sous Linux

Si vous êtes sous Linux, vous pouvez utiliser le script `build.sh` pour compiler le projet. Assurez-vous que tous les paquets nécessaires sont installés. Ensuite, exécutez la commande suivante dans le répertoire du projet :

```bash
./build.sh
