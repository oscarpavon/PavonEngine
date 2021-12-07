#/bin/bash
cd ../Game/
mkdir build
cp -r ../assets ./
cd build
cmake ../../ 
cmake --build ./ --config Debug --target Game -- -j 10
st ./Game
