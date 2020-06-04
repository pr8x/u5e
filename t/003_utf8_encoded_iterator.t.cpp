#include "gtest/gtest.h"
#include <string_view>
#include <string>
#include <iterator>

#include <u5e/utf8_iterator.hpp>
#include <u5e/utf8_string.hpp>
#include <u5e/utf8_string_view.hpp>

using std::string;
using std::string_view;
using u5e::codepoint;
using u5e::utf8_string;
using u5e::utf8_string_view;
using u5e::utf8_iterator;
using u5e::utf8_const_iterator;

TEST(t_003_utf8_encoded_iterator, constructor1a) {
  string_view str("Ol\xC3\x81!");
  utf8_string_view e(str);
}

TEST(t_003_utf8_encoded_iterator, constructor1b) {
  string str("Ol\xC3\x81!");
  utf8_string e(str);
}

TEST(t_003_utf8_encoded_iterator, constructor2a) {
  string_view str("Ol\xC3\x81!");
  utf8_string_view e1(str);
  utf8_string_view e2(e1);
}

TEST(t_003_utf8_encoded_iterator, constructor2b) {
  string str("Ol\xC3\x81!");
  utf8_string e1(str);
  utf8_string e2(e1);
}

TEST(t_003_utf8_encoded_iterator, constructor2c) {
  utf8_string str("Ol\xC3\x81!");
}

TEST(t_003_utf8_encoded_iterator, iteratora) {
  string_view str("Ol\xC3\x81!");
  utf8_string_view::const_iterator it = str.cbegin();
  ASSERT_EQ(*it, 'O');
  it++;
  ASSERT_EQ(*it, 'l');
  it++;
  ASSERT_EQ(*it, 0xC1);
  it++;
  ASSERT_EQ(*it, '!');
  it++;
  ASSERT_EQ(it, str.cend());
}

TEST(t_003_utf8_encoded_iterator, iteratorb) {
  string str("Ol\xC3\x81!");
  utf8_string::const_iterator it = str.cbegin();
  ASSERT_EQ(*it, 'O');
  it++;
  ASSERT_EQ(*it, 'l');
  it++;
  ASSERT_EQ(*it, 0xC1);
  it++;
  ASSERT_EQ(*it, '!');
  it++;
  ASSERT_EQ(it, str.cend());
}

TEST(t_003_utf8_encoded_iterator, iteratorc) {
  const char* foo = "Ol\xC3\x81!";
  utf8_const_iterator<const char*> it(foo);
  ASSERT_EQ(*it, 'O');
  utf8_const_iterator<const char*> copy = ++it;
  ASSERT_EQ(copy, it);
  ASSERT_EQ(*it, 'l');
  copy = it++;
  ASSERT_NE(copy, it);
  ASSERT_EQ(*it, 0xC1);
  it++;
  ASSERT_EQ(*it, '!');
  it++;
  ASSERT_EQ(it, foo+5);
  copy = --it;
  ASSERT_EQ(copy, it);
  ASSERT_EQ(*it, '!');
  copy = it--;
  ASSERT_NE(copy, it);
  ASSERT_EQ(*it, 0xC1);
  it--;
  ASSERT_EQ(*it, 'l');
  it--;
  ASSERT_EQ(*it, 'O');
  ASSERT_EQ(it, foo);
}

TEST(t_003_utf8_encoded_iterator, iteratord) {
  char foo[] = "Olaa!";
  utf8_iterator<char*> it(foo);
  ASSERT_EQ('O', *it);
  it++;
  ASSERT_EQ('l', *it);
  it++;
  *it = 0xC1;
  ASSERT_EQ(0xC1, *it);
  it++;
  ASSERT_EQ((char)foo[3], (char)0x81);
  *it = '!';
  it++;
  ASSERT_EQ(strcmp(foo, "Ol\xC3\x81!"), 0);
}

TEST(t_003_utf8_encoded_iterator, iteratore) {
  const string foo("Ol\xC3\x81!");
  utf8_const_iterator<string::const_iterator> it(foo.cbegin());
  ASSERT_EQ(*it, 'O');
}

TEST(t_003_utf8_encoded_iterator, iteratorf) {
  utf8_string foo("Ol\xC3\x81!");
  utf8_string::const_iterator it(foo.codepoint_cbegin());
  ASSERT_EQ(*it, 'O');
}

TEST(t_003_utf8_encoded_iterator, traits) {
  ASSERT_EQ(sizeof(std::iterator_traits<utf8_iterator<const char*>>::value_type),
            sizeof(codepoint));
  ASSERT_EQ(sizeof(std::iterator_traits<utf8_iterator<string::iterator>>::value_type),
            sizeof(codepoint));
  ASSERT_EQ(sizeof(std::iterator_traits<utf8_iterator<string_view::iterator>>::value_type),
            sizeof(codepoint));
}

TEST(t_003_utf8_encoded_iterator, decode) {
  const unsigned char in1[] = { 0b11010000, 0b10100000 };
  utf8_const_iterator<const unsigned char*> it1(in1);
  ASSERT_EQ(*it1, 1056);
  const unsigned char in2[] = { 0b11101000, 0b10100000, 0b10100000 };
  utf8_const_iterator<const unsigned char*> it2(in2);
  ASSERT_EQ(*it2, 34848);
  const unsigned char in3[] = { 0b11110100, 0b10100000, 0b10100000, 0b10100000 };
  utf8_const_iterator<const unsigned char*> it3(in3);
  ASSERT_EQ(*it3, 1181728);
}

TEST(t_003_utf8_encoded_iterator, output_iterator) {
  utf8_string output("     ");
  utf8_string input("Ol\xC3\x81!");
  std::copy(input.codepoint_cbegin(),input.codepoint_cend(),
            output.codepoint_begin());
  ASSERT_STREQ(input.native_string.c_str(),
	       output.native_string.c_str());
}
