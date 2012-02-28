#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "../Ling/Player.hpp"
#include <iostream>
using namespace std;
// most frequently you implement test cases as a free functions with automatic registration
BOOST_AUTO_TEST_CASE( PlayerTest )
{
	Player player;
	BOOST_CHECK(player.OpenFile(L"D:\\bzhu\\Dropbox\\VOA\\0. special201101110045.mp3"));

	BOOST_CHECK(player.PlayFrom(0));
    int old_pos = 0;
    int new_pos = 0;
	for(int i = 0;i <10; ++i){
		Sleep(100);
		new_pos = player.GetCurrentPos();
        BOOST_CHECK(new_pos > old_pos);
        old_pos = new_pos;
	}
	BOOST_CHECK(!player.OpenFile(L"D:\\8. Proverbs About How to Live.m3"));

}

