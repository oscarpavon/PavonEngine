cd ~/PavonEngine/build
make -j4

cd ~/PavonEngine/source/engine/platforms/android/

echo // >> main.c

cd ~/PavonEngine/source/engine/platforms/android/build

make -j4

cd ~/PavonEngine/source/engine/platforms/android/

./build.sh
fakeroot adb install -r --fastdeploy ./pavons.apk
am start com.pavonstudios.app/android.app.NativeActivity
