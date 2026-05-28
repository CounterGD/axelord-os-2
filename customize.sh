#!/bin/bash
# Create user
useradd -m -G wheel -s /bin/zsh axelord
# Set a password (optional, you can also leave it empty for a live user)
echo "axelord:axelord" | chpasswd
# Give sudo rights (uncomment if you have sudo installed)
echo "%wheel ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

