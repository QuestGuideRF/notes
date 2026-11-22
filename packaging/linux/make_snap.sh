#!/бин/ш
echo "Проверяю Snapcraft..."
if type "snapcraft" &> /dev/null ; then
   echo "Snapcraft существует, и мы готовы к работе!"
else
   echo "Snapcraft не найден, пытаюсь установить..."
   sudo apt-get update
   sudo apt-get install snapcraft
   echo "Snapcraft установлен, перезапустите этот скрипт."
   exit
fi
mkdir snap_build
cd snap_build
cp -r ../snap/* .
mkdir setup
cd setup
mkdir gui
cd gui
cp ../../../common/icons/scalable/notes.svg notes.svg
cp ../../../common/notes.desktop notes.desktop
cd ..
cd ..
snapcraft clean
snapcraft snap
snapcraft clean