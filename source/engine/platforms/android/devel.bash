if [ ! -d ~/PavonEngine/build ]
then
  echo Creating Engine build directory && mkdir ~/PavonEngine/build && cd ~/PavonEngine/build && cmake .. 
else
  cd ~/PavonEngine/build
fi

make -j8

cd ~/PavonEngine/source/engine/platforms/android/

echo // >> main.c

if [ ! -d ~/PavonEngine/source/engine/platforms/android/build ]
then
  echo Creating Android build directory && mkdir ~/PavonEngine/source/engine/platforms/android/build 
  cd ~/PavonEngine/source/engine/platforms/android/build && cmake ..
else
  cd ~/PavonEngine/source/engine/platforms/android/build
fi

make -j8

cd ~/PavonEngine/android/
./configure_content.sh

cd ~/PavonEngine/source/engine/platforms/android/

./apk_make.sh
adb install ./pavons.apk
./start.sh && ./debug.sh

#Installation process
#fakeroot adb install -r --fastdeploy ./pavons.apk
#sudo pm install ./pavons.apk
#am start com.pavonstudios.app/android.app.NativeActivity
