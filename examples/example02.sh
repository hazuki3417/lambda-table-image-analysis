#!/bin/sh

curl --location --request POST 'https://925e8i1zw2.execute-api.ap-northeast-1.amazonaws.com/table-image-analysis' \
--header 'Content-Type: image/png' \
--data-binary '@./resources/sample02.png'
