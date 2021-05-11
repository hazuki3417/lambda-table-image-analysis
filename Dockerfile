ARG WORKSPACE=/tmp
ARG PACKAGE_DIR_PATH=/tmp/api-package

# zipファイルの展開
FROM hazuki3417/cpp-build:latest as package-deployment

ARG WORKSPACE
ARG PACKAGE_DIR_PATH
ARG PACKAGE_FILE_NAME=lambda-table-image-analysis-api.zip

WORKDIR ${WORKSPACE}

COPY ./build/${PACKAGE_FILE_NAME} ./

RUN unzip ./${PACKAGE_FILE_NAME} -d ${PACKAGE_DIR_PATH}



# lambda contiainerの作成
FROM amazon/aws-lambda-provided:latest as build-container

ARG PACKAGE_DIR_PATH

COPY --from=package-deployment ${PACKAGE_DIR_PATH} ${PACKAGE_DIR_PATH}

RUN cp -rfv  ${PACKAGE_DIR_PATH}/bootstrap ${LAMBDA_RUNTIME_DIR} && \
    cp -rfv ${PACKAGE_DIR_PATH}/bin ${LAMBDA_TASK_ROOT}/bin && \
    cp -rfv ${PACKAGE_DIR_PATH}/lib ${LAMBDA_TASK_ROOT}/lib

ARG TESSDATA_REPOSITORY=https://github.com/tesseract-ocr/tessdata/raw/4.1.0
ARG TESSDATA_DIR=${LAMBDA_TASK_ROOT}/tessdata

ENV TESSDATA_PREFIX=${TESSDATA_DIR}

RUN mkdir -p ${TESSDATA_DIR} && \
    cd ${TESSDATA_DIR} && \
    pwd && \
    curl -OL "${TESSDATA_REPOSITORY}/eng.traineddata" && \
    curl -OL "${TESSDATA_REPOSITORY}/jpn.traineddata" && \
    curl -OL "${TESSDATA_REPOSITORY}/jpn_vert.traineddata"

CMD [ "function.handler" ]
