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


    const std::string resources_dir_path  = "./resources/";
    const std::string input_file_name  = "input_file2.txt";
    const std::string input_file_path  = resources_dir_path + input_file_name;

    const std::string work_file_name = "table_image.png";
    const std::string work_file_path = work_dir_path + "/" + work_file_name;

    selen::workspace *workspace = new selen::workspace(work_dir_path);

    if (!workspace->create())
    {
        output("作業ディレクトリの作成に失敗しました");
        delete workspace;
        exit(EXIT_FAILURE);
    }

    std::ifstream ifs(input_file_path);
    std::ofstream ofs(work_file_path);

    if (ifs.fail() || ofs.fail())
    {
        output("ファイルの操作に失敗しました");
        delete workspace;
        exit(EXIT_FAILURE);
    }

    // base64文字列を保持する変数
    std::string base64_str;

    // base64文字列をデコードして画像ファイルとして保存
    while (getline(ifs, base64_str))
    {
        // ファイル書き込み
        ofs << selen::base64::decode(base64_str);
    }

    // ファイルを閉じる
    ofs.close();



    float kernel_h_array[1][15] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };

    float kernel_v_array[15][1] = {
        {1},
        {1},
        {1},
        {1},
        {1},
        {1},
        {1},
        {1},
        {1},
        {1},
        {1},
        {1},
        {1},
        {1},
        {1}
    };

    // テーブル解析処理
    cv::Mat org_image,
        gray_image,
        binarization_image,
        color_reverse_image,
        analysis_image,
        expansion_image,
        h_image,
        v_image,
        stats_image,
        expansion_reverse_image;

    // kernel情報作成
    std::array<float, 15> kernel_row;
    kernel_row.fill(1);
    cv::Mat expansion_kernel(1, 1, CV_32F, kernel_row.data());

    cv::Mat analysis_kernel(15, 15, CV_8U, kernel_row.data());

    // 縦・横線の検出しきい値
    const int min = 3;
    const int max = 15;

    cv::Mat kernel_v(min, max, CV_8U, kernel_v_array);
    cv::Mat kernel_h(max, min, CV_8U, kernel_h_array);

    // 画像読み込み
    org_image = cv::imread(work_file_path, cv::IMREAD_UNCHANGED);
    // グレースケール変換
    cv::cvtColor(org_image, gray_image, cv::COLOR_BGR2GRAY);
    // 二値化
    cv::threshold(gray_image, binarization_image, 0, 255, cv::THRESH_OTSU);

    // 画像反転
    cv::bitwise_not(binarization_image, color_reverse_image);

    // 縦・横線の検出
    cv::morphologyEx(color_reverse_image, v_image, cv::MORPH_OPEN, kernel_v);
    cv::morphologyEx(color_reverse_image, h_image, cv::MORPH_OPEN, kernel_h);

    analysis_image = h_image | v_image;

    // 膨張処理
    cv::dilate(analysis_image, expansion_image, expansion_kernel);

    // 画像反転
    cv::bitwise_not(expansion_image, expansion_reverse_image);

    cv::Mat LabelImg;
    cv::Mat stats;
    cv::Mat centroids;
    cv::Mat Dst(org_image.size(), CV_8UC3);

    // セル検出
    int nLab = cv::connectedComponentsWithStats(expansion_reverse_image, LabelImg, stats, centroids, 8, 4);

    std::vector <selen::cell>cells;

    int count = 0;
    int min_cell_size = 15;
    // 検出したセルの描画（枠のみ）
    for (int i = 2; i < nLab; ++i)
    {
        count++;
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
        cv::rectangle(Dst, cell, cv::Scalar(0, 255, 0), 2);

        std::string file_name = "cell_" + std::to_string(count) + ".png";
        std::string file_path = workspace->get_path() + "/" + file_name;

        cv::Mat cut_img(org_image, cell);
        cv::imwrite(file_path, cut_img);

        cells.push_back({file_path, "", x, y, x + width, y + height, width, height});
    }

    // デバッグ
    // std::string gray_image_path = work_dir_path + "/gray_image.png";
    // std::string binarization_image_path = work_dir_path + "/binarization_image.png";
    // cv::imwrite(gray_image_path, gray_image);
    // cv::imwrite(binarization_image_path, binarization_image);

    // cv::imshow("org image", org_image);
    // cv::imshow("gray image", gray_image);
    // cv::imshow("binarization image", binarization_image);
    // cv::imshow("color reverse image", color_reverse_image);
    // cv::imshow("v kernel image", v_image);
    // cv::imshow("h kernel image", h_image);
    // cv::imshow("analysis image", analysis_image);
    // cv::imshow("expansion image", expansion_image);
    // cv::imshow("Labels", Dst);

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

    if (api->Init(NULL, "jpn+eng"))
    {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(EXIT_FAILURE);
    }

    // 文字解析
    for (auto itr = cells.begin(); itr != cells.end(); ++itr)
    {
        api->SetImage(pixRead((*itr).path.c_str()));
        (*itr).str = api->GetUTF8Text();
    }
    api->End();
    delete api;

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

    // 作業領域削除
    if (!workspace->remove())
    {
        output("作業ディレクトリの削除に失敗しました");
        delete workspace;
        exit(EXIT_FAILURE);
    }
    delete workspace;

    exit(EXIT_FAILURE);
}
