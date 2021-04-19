#include <gtest/gtest.h>
#include "workspace.hpp"


TEST(workspaceTest1, status)
{
    selen::workspace workspace = *new selen::workspace("./testing_dir1");

    EXPECT_EQ(workspace.get_path(), "./testing_dir1");

    EXPECT_EQ(workspace.exists(), false);

    // 初めて実行
    EXPECT_EQ(workspace.create(), true);
    EXPECT_EQ(workspace.exists(), true);

    // 同じ操作を繰り返し実行
    EXPECT_EQ(workspace.create(), false);
    EXPECT_EQ(workspace.exists(), true);

    // 初めて実行
    EXPECT_EQ(workspace.remove(), true);
    EXPECT_EQ(workspace.exists(), false);

    // 同じ操作を繰り返し実行
    EXPECT_EQ(workspace.remove(), false);
    EXPECT_EQ(workspace.exists(), false);
}

TEST(workspaceTest2, status)
{
    selen::workspace *workspace = new selen::workspace("./testing_dir2");

    EXPECT_EQ(workspace->get_path(), "./testing_dir2");

    EXPECT_EQ(workspace->exists(), false);

    // 初めて実行
    EXPECT_EQ(workspace->create(), true);
    EXPECT_EQ(workspace->exists(), true);

    // 同じ操作を繰り返し実行
    EXPECT_EQ(workspace->create(), false);
    EXPECT_EQ(workspace->exists(), true);

    delete workspace;

    // デストラクタにより削除されているか確認
    EXPECT_EQ(workspace->exists(), false);

    // 同じ操作を繰り返し実行
    EXPECT_EQ(workspace->remove(), false);
    EXPECT_EQ(workspace->exists(), false);
}
