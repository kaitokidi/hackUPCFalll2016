#By pinkii

set -e

apt-get install libudev-dev libxrandr-dev libjpeg62-dev libglew1.6-dev libfreetype6-dev libsndfile1-dev libopenal-dev cmake build-essential unzip libxcb-image0-dev
apt-get remove libsfml-*

version=2.4.0
file=SFML-$version-sources.zip

cd /tmp

rm -rf SFML*
wget http://www.sfml-dev.org/download/sfml/$version/$file

unzip $file

cd SFML-$version

cmake -DCMAKE_INSTALL_PREFIX=/usr CMakeLists.txt
make -j4
make install
