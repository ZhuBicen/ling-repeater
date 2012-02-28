#include <iostream>
#include <string>
#include "../ling/Utils.hpp"
//#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/optional.hpp>

BOOST_AUTO_TEST_CASE( IniFileTest )
{

    boost::optional<std::string> path = GetRepoPath();
    if (path){
        LOG(logINFO) << GetRepoPath() << std::endl;
    }
    else{
        LOG(logINFO) << "Can not get the repo path" << std::endl;
    }
}