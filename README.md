# lambda-table-image-analysis
OpenCV + Tesseract OCRを使ったテーブルの文字解析


## 環境構築

当リポジトリには下記のライブラリをサブモジュールとして追加しています。  

 - [aws-lambda-cpp-runtime](https://github.com/awslabs/aws-lambda-cpp)
 - [aws-sdk-cpp](https://github.com/aws/aws-sdk-cpp.git)
 - [boost](https://www.boost.org/doc/libs/1_75_0/more/getting_started/unix-variants.html)
 - [googletest](https://github.com/google/googletest/blob/master/googletest/README.md)
 - [leptonica](http://www.leptonica.org/source/README.html)
 - [opencv](https://docs.opencv.org/master/df/d65/tutorial_table_of_content_introduction.html)
 - [tesseract](https://tesseract-ocr.github.io/tessdoc/Compiling.html)

```sh
git submodule add https://github.com/awslabs/aws-lambda-cpp-runtime.git submodules/aws-lambda-cpp-runtime
git submodule add https://github.com/aws/aws-sdk-cpp.git submodules/aws-sdk-cpp
git submodule add https://github.com/boostorg/boost.git submodules/boost
git submodule add https://github.com/google/googletest.git submodules/googletest
git submodule add https://github.com/DanBloomberg/leptonica.git submodules/leptonica
git submodule add https://github.com/opencv/opencv.git submodules/opencv
git submodule add https://github.com/tesseract-ocr/tesseract.git submodules/tesseract
```

サブモジュールを複製または更新する場合は、当リポジトリ直下で下記のコマンドを実行してください。

```sh
git submodule update --init --recursive
```

サブモジュールをライブラリとして利用できるようにビルド・インストールを行います。  
ライブラリのインストールディレクトリは`/usr/local`を指定します。


### aws-lambda-cpp-runtime

```sh
cd submodules/aws-lambda-cpp-runtime

mkdir build

cd build

cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_INSTALL_PREFIX=/usr/local

make

sudo make install
```

### aws-sdk-cpp

```sh
cd submodules/aws-sdk-cpp

mkdir build

cd build

cmake .. -DBUILD_ONLY="core" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DENABLE_UNITY_BUILD=ON \
    -DCUSTOM_MEMORY_MANAGEMENT=OFF \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DENABLE_UNITY_BUILD=ON

make

sudo make install
```

### boost

```sh
cd submodules/boost

./bootstrap.sh

./b2 install -j8 variant=release 
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
