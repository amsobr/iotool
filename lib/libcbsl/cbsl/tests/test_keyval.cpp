
#include <gtest/gtest.h>
#include <keyval.hpp>

TEST(KeyVal,ValidInput)
{
    char const *inp = "good_key=good_val";
    char key[9];
    char value[9];
    ASSERT_TRUE(cbsl::KeyVal::parse(inp,key,sizeof(key),value,sizeof(value)));
    ASSERT_STREQ(key,"good_key");
    ASSERT_STREQ(value,"good_val");
}

TEST(KeyVal,ValidInputBigBuffer)
{
    char const *inp = "good_key=good_val";
    char key[900];
    char value[900];
    ASSERT_TRUE(cbsl::KeyVal::parse(inp,key,sizeof(key),value,sizeof(value)));
    ASSERT_STREQ(key,"good_key");
    ASSERT_STREQ(value,"good_val");
}

TEST(KeyVal,ValidInputSmallKey)
{
    char const *inp = "good_key=good_val";
    char key[5];
    char value[9];
    ASSERT_FALSE(cbsl::KeyVal::parse(inp,key,sizeof(key),value,sizeof(value)));
    ASSERT_STREQ(key,"good");
    ASSERT_STREQ(value,"good_val");
}

TEST(KeyVal,ValidInputSmallValue)
{
    char const *inp = "good_key=good_val";
    char key[9];
    char value[5];
    ASSERT_FALSE(cbsl::KeyVal::parse(inp,key,sizeof(key),value,sizeof(value)));
    ASSERT_STREQ(key,"good_key");
    ASSERT_STREQ(value,"good");
}

TEST(KeyVal,ValidInputKeyOnly)
{
    char const *inp = "good_key";
    char key[9];
    char value[9];
    ASSERT_TRUE(cbsl::KeyVal::parse(inp,key,sizeof(key),value,sizeof(value)));
    ASSERT_STREQ(key,"good_key");
    ASSERT_TRUE( value[0]=='\0' );
}

TEST(KeyVal,ValidInputGetKeyOnly)
{
    char const *inp = "good_key=good_val";
    char key[9];
    char value[9];
    ASSERT_TRUE(cbsl::KeyVal::parse(inp,key,sizeof(key),0,0));
    ASSERT_STREQ(key,"good_key");
}

TEST(KeyVal,ValidInputGetValueOnly)
{
    char const *inp = "good_key=good_val";
    char key[9];
    char value[9];
    ASSERT_TRUE(cbsl::KeyVal::parse(inp,0,0,value,sizeof(value)));
    ASSERT_STREQ(value,"good_val");
}

TEST(KeyVal,ValidInputSmallBuffs)
{
    char const *inp = "good_key=good_val";
    char key[5];
    char value[5];
    ASSERT_FALSE(cbsl::KeyVal::parse(inp,key,sizeof(key),value,sizeof(value)));
    ASSERT_STREQ(key,"good");
    ASSERT_STREQ(value,"good");
}

TEST(KeyVal,ValidInput1CharBuffs)
{
    char const *inp = "good_key=good_val";
    char key[1];
    char value[1];
    ASSERT_FALSE(cbsl::KeyVal::parse(inp,key,sizeof(key),value,sizeof(value)));
    ASSERT_TRUE(key[0]=='\0');
    ASSERT_TRUE(value[0]=='\0');
}



