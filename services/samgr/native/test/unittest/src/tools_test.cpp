/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tools_test.h"

#include "test_log.h"
#include "tools.h"

using namespace std;
using namespace testing;
using namespace testing::ext;
using namespace OHOS;

void ToolsTest::SetUpTestCase()
{
    DTEST_LOG << "SetUpTestCase" << std::endl;
}

void ToolsTest::TearDownTestCase()
{
    DTEST_LOG << "TearDownTestCase" << std::endl;
}

void ToolsTest::SetUp()
{
    DTEST_LOG << "SetUp" << std::endl;
}

void ToolsTest::TearDown()
{
    DTEST_LOG << "TearDown" << std::endl;
}

/**
 * @tc.name: DeleteAllMark001
 * @tc.desc: test DeleteAllMark removes all spaces from string with spaces
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, DeleteAllMark001, TestSize.Level3)
{
    DTEST_LOG << "DeleteAllMark001 begin" << std::endl;
    std::u16string input = u"hello world";
    std::u16string mark = u" ";
    std::u16string result = DeleteAllMark(input, mark);
    EXPECT_EQ(u"helloworld", result);
    DTEST_LOG << "DeleteAllMark001 end" << std::endl;
}

/**
 * @tc.name: DeleteAllMark002
 * @tc.desc: test DeleteAllMark with empty string returns empty
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, DeleteAllMark002, TestSize.Level3)
{
    DTEST_LOG << "DeleteAllMark002 begin" << std::endl;
    std::u16string input = u"";
    std::u16string mark = u" ";
    std::u16string result = DeleteAllMark(input, mark);
    EXPECT_EQ(u"", result);
    DTEST_LOG << "DeleteAllMark002 end" << std::endl;
}

/**
 * @tc.name: DeleteAllMark003
 * @tc.desc: test DeleteAllMark with mark not found returns original
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, DeleteAllMark003, TestSize.Level3)
{
    DTEST_LOG << "DeleteAllMark003 begin" << std::endl;
    std::u16string input = u"helloworld";
    std::u16string mark = u" ";
    std::u16string result = DeleteAllMark(input, mark);
    EXPECT_EQ(u"helloworld", result);
    DTEST_LOG << "DeleteAllMark003 end" << std::endl;
}

/**
 * @tc.name: DeleteAllMark004
 * @tc.desc: test DeleteAllMark removes all occurrences of multi-char mark
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, DeleteAllMark004, TestSize.Level3)
{
    DTEST_LOG << "DeleteAllMark004 begin" << std::endl;
    std::u16string input = u"abc/tdef/tghi";
    std::u16string mark = u"/t";
    std::u16string result = DeleteAllMark(input, mark);
    EXPECT_EQ(u"abcdefghi", result);
    DTEST_LOG << "DeleteAllMark004 end" << std::endl;
}

/**
 * @tc.name: DeleteBlank001
 * @tc.desc: test DeleteBlank removes all whitespace-like chars
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, DeleteBlank001, TestSize.Level3)
{
    DTEST_LOG << "DeleteBlank001 begin" << std::endl;
    std::u16string input = u"hello world/tnew line";
    std::u16string result = DeleteBlank(input);
    EXPECT_EQ(u"helloworldnewline", result);
    DTEST_LOG << "DeleteBlank001 end" << std::endl;
}

/**
 * @tc.name: DeleteBlank002
 * @tc.desc: test DeleteBlank with empty string returns empty
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, DeleteBlank002, TestSize.Level3)
{
    DTEST_LOG << "DeleteBlank002 begin" << std::endl;
    std::u16string input = u"";
    std::u16string result = DeleteBlank(input);
    EXPECT_EQ(u"", result);
    DTEST_LOG << "DeleteBlank002 end" << std::endl;
}

/**
 * @tc.name: DeleteBlank003
 * @tc.desc: test DeleteBlank with string with no blanks returns original
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, DeleteBlank003, TestSize.Level3)
{
    DTEST_LOG << "DeleteBlank003 begin" << std::endl;
    std::u16string input = u"helloworld";
    std::u16string result = DeleteBlank(input);
    EXPECT_EQ(u"helloworld", result);
    DTEST_LOG << "DeleteBlank003 end" << std::endl;
}

/**
 * @tc.name: AnonymizeDeviceId001
 * @tc.desc: test AnonymizeDeviceId with long string returns first 6 + "******"
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, AnonymizeDeviceId001, TestSize.Level3)
{
    DTEST_LOG << "AnonymizeDeviceId001 begin" << std::endl;
    std::string deviceId = "abcdefghij";
    std::string result = AnonymizeDeviceId(deviceId);
    EXPECT_EQ("abcdef******", result);
    DTEST_LOG << "AnonymizeDeviceId001 end" << std::endl;
}

/**
 * @tc.name: AnonymizeDeviceId002
 * @tc.desc: test AnonymizeDeviceId with short string returns empty
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, AnonymizeDeviceId002, TestSize.Level3)
{
    DTEST_LOG << "AnonymizeDeviceId002 begin" << std::endl;
    std::string deviceId = "abc";
    std::string result = AnonymizeDeviceId(deviceId);
    EXPECT_EQ("", result);
    DTEST_LOG << "AnonymizeDeviceId002 end" << std::endl;
}

/**
 * @tc.name: AnonymizeDeviceId003
 * @tc.desc: test AnonymizeDeviceId with exactly 6 chars returns first 6 + "******"
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, AnonymizeDeviceId003, TestSize.Level3)
{
    DTEST_LOG << "AnonymizeDeviceId003 begin" << std::endl;
    std::string deviceId = "abcdef";
    std::string result = AnonymizeDeviceId(deviceId);
    EXPECT_EQ("abcdef******", result);
    DTEST_LOG << "AnonymizeDeviceId003 end" << std::endl;
}

/**
 * @tc.name: AnonymizeDeviceId004
 * @tc.desc: test AnonymizeDeviceId with empty string returns empty
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, AnonymizeDeviceId004, TestSize.Level3)
{
    DTEST_LOG << "AnonymizeDeviceId004 begin" << std::endl;
    std::string deviceId = "";
    std::string result = AnonymizeDeviceId(deviceId);
    EXPECT_EQ("", result);
    DTEST_LOG << "AnonymizeDeviceId004 end" << std::endl;
}

/**
 * @tc.name: MaxServices001
 * @tc.desc: test MAX_SERVICES constant value
 * @tc.type: FUNC
 */
HWTEST_F(ToolsTest, MaxServices001, TestSize.Level3)
{
    DTEST_LOG << "MaxServices001 begin" << std::endl;
    EXPECT_EQ(1000u, MAX_SERVICES);
    DTEST_LOG << "MaxServices001 end" << std::endl;
}
