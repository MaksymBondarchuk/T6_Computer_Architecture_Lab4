#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Task_tests

#include <boost/test/unit_test.hpp>
#include "../task/task.hpp"


BOOST_AUTO_TEST_CASE(testing_palindromes) {
    BOOST_CHECK(palindromes("") == "");

    BOOST_CHECK(palindromes("aba") == "aba\n");

    if (palindromes("a a abbcbb") != "bb\na a\n a \nbcb\na a a\nbbcbb\n")
        BOOST_ERROR("Failed on testing 'palindromes(\"a a abbcbb\") != \"bb\\na a\\n a \\nbcb\\na a a\\nbbcbb\\n\\n'");

    if (palindromes("iycbmhxwetsxfxvpvhbwxlkeu") != "xfx\nvpv\n")
        throw "Failed on testing 'palindromes(\"iycbmhxwetsxfxvpvhbwxlkeu\") != \"xfx\\nvpv\\n\"'";

    BOOST_CHECK_MESSAGE(palindromes("abcdefg") == "",
                        "palindromes(\"abcdefg\") result: " << palindromes("abcdefg").c_str());

    BOOST_CHECK_EQUAL(palindromes("abcdefghijklmnopponmlkjihgfedcba"), "pp\noppo\nnoppon\nmnopponm\nlmnopponml\n"
            "klmnopponmlk\njklmnopponmlkj\nijklmnopponmlkji\nhijklmnopponmlkjih\n");
}
