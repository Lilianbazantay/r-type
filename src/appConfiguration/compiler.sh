#!/bin/bash
# ===============================
# Compiler script pour SFML + ImGui
# ===============================

# Arrêter le script dès qu'une commande échoue
set -e

# Dossier de build
BUILD_DIR="build"

# Supprimer le contenu précédent et créer le dossier build
echo "🧹 Nettoyage du dossier de build..."
rm -rf "${BUILD_DIR:?}/*"
mkdir -p "$BUILD_DIR"

# Aller dans le dossier build
cd "$BUILD_DIR"

# Configurer le projet avec CMake
echo "⚙️  Configuration CMake..."
cmake -DCMAKE_BUILD_TYPE=Debug .. || { echo "❌ Erreur CMake"; exit 1; }

# Compiler le projet en parallèle avec tous les coeurs CPU
echo "🔨 Compilation..."
cmake --build . --parallel $(nproc) || { echo "❌ Erreur de compilation"; exit 1; }

echo "✅ Build terminé !"
