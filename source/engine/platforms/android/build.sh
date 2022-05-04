#/bin/bash
mkdir gen
mkdir obj
mkdir output

aapt package -f --generate-dependencies -J gen -M AndroidManifest.xml -S res

unset JAVA_HOME
dalvikvm -Xmx512m -Xcompiler-option --compiler-filter=speed -cp "$PREFIX"/share/dex/ecj.jar org.eclipse.jdt.internal.compiler.batch.Main -proc:none -source 1.8 -target 1.8 -cp "$PREFIX"/share/java/android.jar $ECJENT -d ./obj .

dx --dex --output=output/classes.dex ./obj

aapt package -f -M AndroidManifest.xml -S ./res -F pavon.apk

cd output 
aapt add -v -f ../pavon.apk classes.dex 

cd ../lib/arm64-v8a/

aapt add -v -f ../../pavon.apk libnative-activity.so


RDR="/data/data/com.termux/files/home/buildAPKs/"

PHOME=~/PavonEngine/source/engine/platforms/android/
"$RDR/scripts/sh/shlibs/signapk" sign --cert "$RDR/opt/key/cert.x509.pem" --key "$RDR/opt/key/key.pk8" --in "$PHOME/pavon.apk" --out "$PHOME/pavons.apk"


