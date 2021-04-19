# lambda-table-image-analysis
OpenCV + Tesseract OCRを使ったテーブルの文字解析


## 環境構築

当リポジトリには下記のライブラリをサブモジュールとして追加しています。  

 - [boost](https://www.boost.org/doc/libs/1_75_0/more/getting_started/unix-variants.html)
 - [googletest](https://github.com/google/googletest/blob/master/googletest/README.md)
 - [leptonica](http://www.leptonica.org/source/README.html)
 - [opencv](https://docs.opencv.org/master/df/d65/tutorial_table_of_content_introduction.html)
 - [tesseract](https://tesseract-ocr.github.io/tessdoc/Compiling.html)

サブモジュールを複製または更新する場合は、当リポジトリ直下で下記のコマンドを実行してください。

```sh
git submodule update --init --recursive
```

サブモジュールをライブラリとして利用できるようにビルド・インストールを行います。  
ライブラリのインストールディレクトリは`/usr/local`を指定します。

### boost

```sh
```
### googletest

```sh
cd submodules/googletest

mkdir build; cd build

cmake ..

make

make install
```

### leptonica

```sh
cd submodules/leptonica

mkdir build; cd build

cmake .. -DBUILD_PROG=1

make

make install
```

### opencv

```sh
cd submodules/opencv

mkdir build; cd build

cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON ..

make -j8

make install
```

### tesseract

```sh
cd submodules/tesseract

./autogen.sh

./configure --prefix=/usr/local

make

make install
```
