#include "doctest.h"
#include <plog/Log.h>
#include "TestAppender.h"
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <string>

SCENARIO("std containers") 
{
    GIVEN("logger is initialised") 
    {
        plog::TestAppender testAppender;
        plog::Logger<PLOG_DEFAULT_INSTANCE_ID> logger(plog::verbose);
        logger.addAppender(&testAppender);

        WHEN("empty collection") 
        {
            std::vector<int> vectorOfInts;
            PLOGI << vectorOfInts;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[]"));
            }
        }

        WHEN("std::vector") 
        {
            std::vector<int> vectorOfInts;
            vectorOfInts.push_back(1);
            vectorOfInts.push_back(2);
            vectorOfInts.push_back(3);
            PLOGI << vectorOfInts;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[1, 2, 3]"));
            }
        }

        WHEN("std::deque") 
        {
            std::deque<std::string> dequeOfStrings;
            dequeOfStrings.push_back("one");
            dequeOfStrings.push_back("two");
            dequeOfStrings.push_back("three");
            PLOGI << dequeOfStrings;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[one, two, three]"));
            }
        }

        WHEN("std::list") 
        {
            std::list<const char*> listOfCharPointers;
            listOfCharPointers.push_back("one");
            listOfCharPointers.push_back("two");
            listOfCharPointers.push_back(NULL);
            PLOGI << listOfCharPointers;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[one, two, (null)]"));
            }
        }

        WHEN("std::set") 
        {
            std::set<int> setOfInts;
            setOfInts.insert(10);
            setOfInts.insert(20);
            setOfInts.insert(30);
            PLOGI << setOfInts;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[10, 20, 30]"));
            }
        }

        WHEN("std::map") 
        {
            std::map<std::string, int> mapStringToInt;
            mapStringToInt["red"] = 1;
            mapStringToInt["green"] = 2;
            mapStringToInt["blue"] = 4;
            PLOGI << mapStringToInt;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[blue:4, green:2, red:1]"));
            }
        }

        WHEN("std::multimap") 
        {
            std::multimap<int, std::string> multimapIntToString;
            multimapIntToString.insert(std::make_pair(1, "one"));
            multimapIntToString.insert(std::make_pair(1, "uno"));
            multimapIntToString.insert(std::make_pair(2, "two"));
            multimapIntToString.insert(std::make_pair(2, "due"));
            PLOGI << multimapIntToString;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[1:one, 1:uno, 2:two, 2:due]"));
            }
        }

        WHEN("std::vector of std::vector") 
        {
            std::vector<std::vector<int> > vectorOfVectorsOfInts(3);
            vectorOfVectorsOfInts[0].push_back(1);
            vectorOfVectorsOfInts[0].push_back(2);
            vectorOfVectorsOfInts[1].push_back(-1);
            vectorOfVectorsOfInts[1].push_back(-2);
            PLOGI << vectorOfVectorsOfInts;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("[[1, 2], [-1, -2], []]"));
            }
        }

        WHEN("std::pair") 
        {
            std::pair<int, int> pairOfInts(5, 10);
            PLOGI << pairOfInts;

            THEN("the result is as expected") 
            {
                CHECK_EQ(testAppender.getMessage(), PLOG_NSTR("5:10"));
            }
        }
    }
}