#include <iostream>
#include <fstream>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "selen/base64.hpp"
#include "selen/workspace.hpp"
#include "selen/cell.hpp"

int get_unix_timestamp()
{
    time_t current_time = time(0);
    tm *tm_current = localtime(&current_time);
    return mktime(tm_current);
}

void output(std::string str)
{
    std::cout << str << std::endl;
}

int main()
{
    const std::string tmp_dir_path = "./";
    const std::string work_dir_name_prefix = "aws-lambda-";
    const std::string work_dir_name_suffix = std::to_string(get_unix_timestamp());
    const std::string work_dir_path = 
        tmp_dir_path + work_dir_name_prefix + work_dir_name_suffix;

    const std::string target_file_path = "./resources/sample.png";

    selen::workspace *workspace = new selen::workspace(work_dir_path);

    if (!workspace->create())
    {
        output("作業ディレクトリの作成に失敗しました");
        delete workspace;
        exit(EXIT_FAILURE);
    }




    // 画像変換・解析処理
    cv::Mat org_image, tmp_image, tmp_v_image, tmp_h_image;

    // 画像読み込み
    org_image = cv::imread(target_file_path, cv::IMREAD_UNCHANGED);
    // グレースケール変換
    // FIXME: すでにグレースケールに変換された画像を再変換しようとするとエラーが発生する
    cv::cvtColor(org_image, tmp_image, cv::COLOR_BGR2GRAY);
    
    cv::imwrite(workspace->get_path() + "/gray.png", tmp_image); // debug:

    // 二値化
    cv::threshold(tmp_image, tmp_image, 150, 255, cv::THRESH_BINARY);

    cv::imwrite(workspace->get_path() + "/binarization.png", tmp_image); // debug:

    // 画像反転
    cv::bitwise_not(tmp_image, tmp_image);

    cv::imwrite(workspace->get_path() + "/reverse.png", tmp_image); // debug:



    // 画像解析

    // 縦・横線の検出しきい値
    const int min = 1;
    const int max = 15;

    float kernel_h_array[min][max] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    float kernel_v_array[max][min] = {
        {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}
    };

    cv::Mat kernel_v(min, max, CV_8U, kernel_v_array);
    cv::Mat kernel_h(max, min, CV_8U, kernel_h_array);

    // 縦・横線の検出
    cv::morphologyEx(tmp_image, tmp_v_image, cv::MORPH_OPEN, kernel_v);
    cv::morphologyEx(tmp_image, tmp_h_image, cv::MORPH_OPEN, kernel_h);
    // 結果をマージ
    tmp_image = tmp_h_image | tmp_v_image;

    cv::imwrite(workspace->get_path() + "/analysis.png", tmp_image); // debug:

    // 膨張処理
    std::array<float, 15> kernel_row;
    kernel_row.fill(1);
    cv::Mat expansion_kernel(1, 1, CV_32F, kernel_row.data());

    cv::dilate(tmp_image, tmp_image, expansion_kernel);

    cv::imwrite(workspace->get_path() + "/expansion.png", tmp_image); // debug:

    // 画像反転
    cv::bitwise_not(tmp_image, tmp_image);

    cv::imwrite(workspace->get_path() + "/expansion_reverse.png", tmp_image); // debug:

    // セル検出
    cv::Mat label, stats, centroids, dst(org_image.size(), CV_8UC3);
    int nLab = cv::connectedComponentsWithStats(
        tmp_image, label, stats, centroids, 8, 4);

    std::vector <selen::cell>cells;

    // cellとして認識する最小サイズ
    const int min_cell_size = 15;

    for (int i = 2; i < nLab; ++i)
    {
        int *param = stats.ptr<int>(i);

        int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
        int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
        int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
        int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];

        if(height < min_cell_size || width < min_cell_size){
            // 指定したセルのサイズより小さい場合は除外する
            continue;
        }

        cv::Rect cell = cv::Rect(x, y, width, height);
        cv::rectangle(dst, cell, cv::Scalar(0, 255, 0), 2);

        std::string file_name = "cell_" + std::to_string(i) + ".png";
        std::string file_path = workspace->get_path() + "/" + file_name;

        cv::Mat cut_img(org_image, cell);
        cv::imwrite(file_path, cut_img);

        cells.push_back({file_path, "", x, y, x + width, y + height, width, height});
    }


    /**
     * 文字解析処理
     */
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

    if (api->Init("/var/task/tessdata/", "jpn+eng"))
    {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(EXIT_FAILURE);
    }

    for (auto itr = cells.begin(); itr != cells.end(); ++itr)
    {
        // 文字解析する画像を設定
        api->SetImage(pixRead((*itr).path.c_str()));
        // 文字解析して結果を保持
        (*itr).str = api->GetUTF8Text();
    }

    api->End();

    // メモリの開放
    delete api;
    // delete workspace;

    // 出力処理
    for (auto itr = cells.begin(); itr != cells.end(); ++itr)
    {
        output(
            "{str: " + (*itr).str + "," +
            " top: " + std::to_string((*itr).top) + "," +
            " right: " + std::to_string((*itr).right) + "," +
            " bottom: " + std::to_string((*itr).bottom) + "," +
            " left: " + std::to_string((*itr).left) + "," +
            " width: " + std::to_string((*itr).width) + "," +
            " height: " + std::to_string((*itr).height) +
            "}"
        );
    }

    exit(EXIT_SUCCESS);
}
