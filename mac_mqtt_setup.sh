#!/bin/zsh
# Mac MQTT Full Stack (Mosquitto + Node-RED + Docker)

echo "🚀 Mac MQTT Setup - Phase 1 Ready"

# 1. Homebrew (if missing)
if ! command -v brew &> /dev/null; then
    echo "Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zshrc
    source ~/.zshrc
fi

# 2. Mosquitto
brew install mosquitto
brew services start mosquitto
echo "✅ Mosquitto: localhost:1883"

# 3. Node-RED 
brew install node
npm install -g --unsafe-perm node-red node-red-dashboard
echo "✅ Node-RED: node-red → localhost:1880"

# 4. Docker Desktop
brew install --cask docker
open /Applications/Docker.app
echo "✅ Docker ready (manual start)"

# Test
mosquitto_pub -t test/mac -m "SDM120 Ready $(date)"
echo "✅ MQTT working! Run: node-red"
