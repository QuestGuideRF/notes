#!/бин/ш
read -p "Нажмите Enter, чтобы создать пакет Debian." nothing
clear
sh make_deb.sh
echo "Пакет Debian создан"
read -p "Нажмите Enter, чтобы создать пакет Snap." nothing
clear
sh make_snap.sh
echo "Snap-пакет создан."