pkg install neovim
pkg install git
if [ ! -d .config ]
then mkdir .config && cd .conig && git clone https://github.com/oscarpavon/nvim
else
cd .config && git clone https://github.com/oscarpavon/nvim && cd ..
fi
nvim -c "PlugUpdate"
nvim -c "CocInstall coc-clang"

pkg install make
pkg install cmake
pkg install wget
pkg install tsu
pkg install vulkan-headers
pkg install binutils
pkg install mesa
pkg install freetype
pkg install aapt
pkg install dx
pkg install aapt2
pkg install ecj
pkg install automake
pkg install fzf
pkg install ripgrep
pkg install nodejs
pkg install libtool
pkg install zsh
pkg install cmus
#wget https://dl.google.com/android/repository/android-ndk-r25-linux.zip
git config --global user.email pavon_oscar@outlook.com
git config --global user.name Oscar Pavon
