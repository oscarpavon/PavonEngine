build.one.bash
fakeroot adb install ./com.pavonstudios.app.apk
am start com.pavonstudios.app/android.app.NativeActivity
