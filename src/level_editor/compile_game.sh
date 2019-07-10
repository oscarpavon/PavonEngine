#/bin/bash
cd ../Game/
mkdir build
#ln -sf ../../assets ./
cp -r ../assets ./
cd build
cmake ../../ --build . --target Game
make Game -j15
st ./Game