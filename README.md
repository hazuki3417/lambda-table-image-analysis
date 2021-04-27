# lambda-table-image-analysis
OpenCV + Tesseract OCRを使ったテーブルの文字解析


## 開発環境

ローカル：ubuntu 20.04
ビルド：ubuntu 20.04

規格:c++17

使用ライブラリ
 - [aws-lambda-cpp-runtime](https://github.com/awslabs/aws-lambda-cpp.git)
 - [aws-sdk-cpp](https://github.com/aws/aws-sdk-cpp.git)
 - [opencv](https://github.com/opencv/opencv.git)
 - [boost](https://github.com/boostorg/boost.git)
 - [leptonica](https://github.com/DanBloomberg/leptonica.git)
 - [tesseract](https://github.com/tesseract-ocr/tesseract.git)
 - [googletest](https://github.com/google/googletest.git)



## 環境構築

下記のライブラリをサブモジュールとして追加  

 - [aws-lambda-cpp-runtime](https://github.com/awslabs/aws-lambda-cpp.git)
 - [aws-sdk-cpp](https://github.com/aws/aws-sdk-cpp.git)
 - [googletest](https://github.com/google/googletest/blob/master/googletest/README.md)

```sh
git submodule add https://github.com/awslabs/aws-lambda-cpp-runtime.git submodules/aws-lambda-cpp-runtime
git submodule add https://github.com/aws/aws-sdk-cpp.git submodules/aws-sdk-cpp
git submodule add https://github.com/google/googletest.git submodules/googletest
```

追加サブモジュールのコードを反映

```sh
git submodule update --init --recursive
```

サブモジュールをライブラリとして利用できるようにビルド・インストールを行う。  
ライブラリのインストールディレクトリは`/usr`を指定。


### aws-lambda-cpp-runtime

```sh
cd submodules/aws-lambda-cpp-runtime

mkdir build

cd build

cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_INSTALL_PREFIX=/usr

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
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DENABLE_UNITY_BUILD=ON

make

sudo make install
```
### googletest

```sh
cd submodules/googletest

mkdir build; cd build

cmake ..

make

make install
```

### その他、依存パッケージ

サブモジュール以外のライブラリはaptリポジトリからインストールして利用

```sh
RUN apt-get install -y \
    zlib1g-dev \
    libarmadillo-dev \
    libtool \
    libpng-dev \
    libjpeg-dev \
    libtiff-dev \
    libboost-filesystem-dev \
    libboost-iostreams-dev \
    libboost-serialization-dev \
    libboost-system-dev \
    libleptonica-dev \
    libopencv-dev \
    libtesseract-dev \
    libgtest-dev
```

## ビルド・テスト

```sh
# ビルド用のDockerコンテナを起動
docker-compose run --rm cpp-build

# ~ docker にアタッチ後
mkdir build

cd build

# ビルド（デバッグ）
cmake ..

make

# ビルド（リリース）
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/install

# パッケージ化。target名は以下の規則で指定する
# {aws-lambda-package-lambda-} + {cmakeのaws_lambda_package_target()に指定した値}
make aws-lambda-package-lambda-table-image-analysis-api

# テスト実行
./lambda-table-image-analysis-api-testing
```


## APIリクエスト

`curl` コマンドを用いたAPIリクエストの例

```sh
curl --location --request POST '{api endpoint}' \
--header 'Content-Type: image/jpeg' \
--data-binary '@{file path}'
```


## APIレスポンス

```json
{
    "items" : [
        {
            "str": "解析結果の文字列1",
            "top": 20,
            "right": 20,
            "bottom": 70,
            "left": 220,
            "height": 50,
            "width": 200,
        },
        {
            "str": "解析結果の文字列1",
            "top": 20,
            "right": 240,
            "bottom": 90,
            "left": 430,
            "height": 70,
            "width": 100,
        }
    ]
}
```
