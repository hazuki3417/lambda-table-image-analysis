#include <aws/lambda-runtime/runtime.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/SimpleStringStream.h>

#include <iostream>
#include <fstream>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "selen/base64.hpp"
#include "selen/workspace.hpp"
#include "selen/cell.hpp"

using namespace aws::lambda_runtime;

int get_unix_timestamp()
{
    time_t current_time = time(0);
    tm *tm_current = localtime(&current_time);
    return mktime(tm_current);
}

invocation_response my_handler(invocation_request const &request)
{
    using namespace Aws::Utils::Json;

    JsonValue json(request.payload);
    auto v = json.View();

    JsonValue resp;
    // base64文字を受け取る
    if (!v.ValueExists("body"))
    {
        resp.WithBool("status", false);
        resp.WithString("message", "リクエストボディが存在しません");
        return invocation_response::success(resp.View().WriteCompact(), "application/json");
    }

    if (!v.GetObject("body").IsString())
    {
        resp.WithBool("status", false);
        resp.WithString("message", "リクエストボディのデータ型が不正です");
        return invocation_response::success(resp.View().WriteCompact(), "application/json");
    }

    const std::string tmp_dir_path = "/tmp/";
    const std::string work_dir_name_prefix = "aws-lambda-";
    const std::string work_dir_name_suffix = std::to_string(get_unix_timestamp());
    const std::string work_dir_path =
        tmp_dir_path + work_dir_name_prefix + work_dir_name_suffix;

    const std::string work_file_name = "table_image.png";
    const std::string work_file_path = work_dir_path + "/" + work_file_name;

    selen::workspace *workspace = new selen::workspace(work_dir_path);

    // 作業領域の作成
    if (!workspace->create())
    {
        delete workspace;
        resp.WithBool("status", false);
        resp.WithString("message", "作業ディレクトリの作成に失敗しました");
        return invocation_response::success(resp.View().WriteCompact(), "application/json");
    }

    std::ofstream ofs(work_file_path);

    // 作業ファイルの作成
    if (ofs.fail())
    {
        delete workspace;
        resp.WithBool("status", false);
        resp.WithString("message", "作業領域の確保に失敗しました");
        return invocation_response::success(resp.View().WriteCompact(), "application/json");
    }

    // base64文字列をデコードして画像ファイルとして保存
    // ファイル書き込み
    ofs << selen::base64::decode(v.GetString("body"));

    // ファイルを閉じる
    ofs.close();

    // メイン処理


    float kernel_h_array[1][15] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
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
        {1}};

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

    std::vector<selen::cell> cells;

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

        if (height < min_cell_size || width < min_cell_size)
        {
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

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

    if (api->Init(NULL, "jpn+eng"))
    {
        delete workspace;
        resp.WithBool("status", false);
        resp.WithString("message", "学習ファイルの検索に失敗しました");
        return invocation_response::success(resp.View().WriteCompact(), "application/json");
    }

    // // 文字解析
    // for (auto itr = cells.begin(); itr != cells.end(); ++itr)
    // {
    //     api->SetImage(pixRead((*itr).path.c_str()));
    //     (*itr).str = api->GetUTF8Text();
    // }
    // api->End();
    // delete api;

    // // 出力処理
    // for (auto itr = cells.begin(); itr != cells.end(); ++itr)
    // {
    //     // output(
    //     //     "{str: " + (*itr).str + "," +
    //     //     " top: " + std::to_string((*itr).top) + "," +
    //     //     " right: " + std::to_string((*itr).right) + "," +
    //     //     " bottom: " + std::to_string((*itr).bottom) + "," +
    //     //     " left: " + std::to_string((*itr).left) + "," +
    //     //     " width: " + std::to_string((*itr).width) + "," +
    //     //     " height: " + std::to_string((*itr).height) +
    //         // "}");
    //     resp.WithString("str", (*itr).str);
    //     resp.WithInteger("top", (*itr).top);
    //     resp.WithInteger("right", (*itr).right);
    //     resp.WithInteger("bottom", (*itr).bottom);
    //     resp.WithInteger("left", (*itr).left);
    //     resp.WithInteger("width", (*itr).width);
    //     resp.WithInteger("height", (*itr).height);

    //     break;
    // }

    // 作業領域削除
    if (!workspace->remove())
    {
        // TODO: ログ出力
    }

    delete workspace;


    // とりあえず特になにもなければ下記に到達するはず

    // resp.WithString("base64", ss.str());
    resp.WithString("str", "ocr解析した文字を出力");
    resp.WithInteger("top", 1);
    resp.WithInteger("right", 2);
    resp.WithInteger("bottom", 3);
    resp.WithInteger("left", 4);

    return invocation_response::success(resp.View().WriteCompact(), "application/json");
}

int main()
{
    run_handler(my_handler);
    return 0;
}
