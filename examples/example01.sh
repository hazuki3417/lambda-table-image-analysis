#!/bin/sh

curl --location --request POST 'https://925e8i1zw2.execute-api.ap-northeast-1.amazonaws.com/table-image-analysis' \
--header 'Content-Type: image/jpeg' \
--data-binary '@./resources/sample01.jpeg'
