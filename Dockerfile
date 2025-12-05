FROM debian:bookworm AS builder

RUN dpkg --add-architecture i386 
RUN apt update
    #cmakeをインストール
RUN apt install -y cmake make build-essential 
    #MinGW(debian作)のうち必要なものインストール 
RUN apt install -y mingw-w64 gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 
    #hファイル DLL生成
RUN apt install -y mingw-w64-tools 
RUN apt install -y mingw-w64-x86-64-dev

#作業用ディレクトリ作成
WORKDIR /app/build


#プロジェクトファイルをコピー
ADD CMakeLists.txt /app
ADD start.sh /app
ADD src/ /app/src
ADD include/ /app/include
ADD DxLib/ /app/DxLib
COPY dll_override.reg /app

#コンパイラの指定

RUN cmake   -DCMAKE_SYSTEM_NAME=Windows \
            -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
            -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
            -DCMAKE_BUILD_TYPE=Debug \
            -DCMAKE_C_FLAGS="-O0 -g" \
            -DCMAKE_CXX_FLAGS="-O0 -g" \
            -DCMAKE_INSTALL_DO_STRIP=OFF .. && \
    make



#第二段階
FROM debian:bookworm
RUN dpkg --add-architecture i386
#ライブラリ　ツールをインストール
RUN apt update
RUN apt install -y wget
 #互換レイヤ 仮想ディスプレイ　仮想xサーバ
RUN apt install -y wine64 xvfb x11vnc fluxbox \
    libwine wine64-preloader wine-binfmt wine32:i386
RUN apt install -y x11-apps libx11-6 libxext6
RUN apt install -y fonts-wine alsa-utils
RUN apt install -y mesa-utils dbus-x11
#wine64
RUN ln -s /usr/lib/wine/wine64 /usr/bin/wine64
#wineboot
RUN chmod +x /usr/bin/wineboot
#winetricks
RUN wget https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks -O /usr/local/bin/winetricks 
RUN chmod +x /usr/local/bin/winetricks
#入力イベント処理　描画処理 
RUN apt install -y libxkbcommon0 libxkbcommon-x11-0 libxrender1 fonts-wine alsa-utils

WORKDIR /app/build

#バイナリとDLLをコピー
COPY --from=builder /app/build /app/build
COPY --from=builder /usr/lib/gcc/x86_64-w64-mingw32/12-win32/libstdc++-6.dll /app/build
COPY --from=builder /usr/lib/gcc/x86_64-w64-mingw32/12-win32/libgcc_s_seh-1.dll /app/build
COPY --from=builder /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll /app/build
COPY --from=builder /app/start.sh /app/start.sh
COPY dll_override.reg /tmp/dll_override.reg
RUN wine regedit /tmp/dll_override.reg
#パスワード1234をroot/.vnc/passwdに入れている
RUN mkdir -p /root/.vnc && x11vnc -storepasswd 1234 /root/.vnc/passwd
#wine環境を再構築
RUN wineboot --init
#Dxランタイム デバッグツール
RUN apt install -y cabextract binutils-mingw-w64
#フォント
RUN env WINETRICKS_UNATTENDED=1 
RUN winetricks -q corefonts 
#RUN winetricks -q cjkfonts
#シェダー言語コンパイラ　Diret3D9/11の補助機能　ComponentObjectModel
RUN winetricks -q d3dx9_43 d3dcompiler_47 d3dx11_43
RUN export LIBGL_ALWAYS_SOFTWARE=1
#詳細に表示
RUN WINEDEBUG=+ole,+com,+seh,+tid wine your_app.exe &> wine.log
#wine設定
RUN winecfg -v win7

#起動スクリプト
RUN chmod +x /app/start.sh
CMD ["/app/start.sh"]

#Xpra 経由 でWine起動
#xpra start :100 --start-child="wine /app/build/main.exe" --exit-with-children --bind-tcp=0.0.0.0:14500 & wine /app/build/main.exe
#CMD ["xpra" , "start" , ":100" , "--start-child=wine /app/build/main.exe" , "--exit-with-children" , "--bind-tcp=0.0.0.0:14500"]
#CMD ["xpra", "start", ":100", "--start-child=wine64 /app/build/main.exe", "--exit-with-children", "--bind-tcp=0.0.0.0:14500"]
#RUN env WINETRICKS_UNATTENDED=1 winetricks -q ole32 oleaut32 rpcrt4
#cairo-xpra　バインディングツール
#RUN apt install -y libcairo2-dev python3-cairo-dev gir1.2-freedesktop gir1.2-pango-1.0
#GUIツールキット  GTK描画
#RUN apt install -y libcairo2 python3-cairo python3-gi python3-gi-cairo libcairo-gobject2
#動作確認　通信
#Gstreamer
#RUN apt install -y python3-gi gir1.2-gstreamer-1.0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav¥