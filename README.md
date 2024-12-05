# Projet : Jeu de Morpion (Tic-Tac-Toe)

## Description

Ce projet est une implémentation simple du jeu de Morpion (Tic-Tac-Toe) en C, utilisant la bibliothèque GTK pour l'interface graphique. Le jeu permet à deux joueurs de s'affronter tour à tour en plaçant leurs symboles ("X" ou "O") sur une grille de 3x3.

Lorsque l'un des joueurs remplit une ligne, une colonne ou une diagonale avec son symbole, il gagne la partie. En cas d'égalité (toutes les cases remplies sans gagnant), une fenêtre de dialogue l'indique.

---

## Fonctionnalités

- Interface graphique simple et compacte.
- Les lettres "X" et "O" sont affichées avec une taille et un style personnalisés grâce à un fichier CSS.
- Détection automatique des conditions de victoire ou d'égalité.
- Une fenêtre popup apparaît pour indiquer le résultat du jeu (victoire ou égalité) avec des options pour :
  - **Recommencer** une nouvelle partie.
  - **Quitter** le jeu.

---

## Structure des fichiers

- **`main.c`** : Contient le code source principal du projet.
- **`style.css`** : Fichier CSS permettant de personnaliser l'apparence des boutons et du texte dans l'interface.
- **`README.md`** : Ce fichier, contenant des informations sur le projet.

---

## Instructions pour exécuter le projet

### Prérequis

- Un compilateur prenant en charge le C, tel que `gcc`.
- La bibliothèque GTK 3.0 ou supérieure installée sur votre système.

## Auteur

Dubost Victor : victor.dubost@reseau.eseo.fr
