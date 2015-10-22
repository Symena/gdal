#include <gmock/gmock.h>

#include "IndexWarnings.h"

#include <boost/format.hpp>

TEST(IndexWarningsTests, addSingleWarningWorks)
{
	IndexWarnings w;

	w.add("This is a warning!");

	EXPECT_EQ(1, w.size());
	EXPECT_EQ("This is a warning!", *w.begin());

	for(const auto& warning : w)
	{
		EXPECT_EQ("This is a warning!", warning);
	}
}

TEST(IndexWarningsTests, addMultipleWarningWorks)
{
	IndexWarnings w;

	w.add("w1");
	w.add("w2");
	w.add("w3");

	EXPECT_EQ(3, w.size());
	EXPECT_EQ("w1", *(w.begin() + 0));
	EXPECT_EQ("w2", *(w.begin() + 1));
	EXPECT_EQ("w3", *(w.begin() + 2));
}

TEST(IndexWarningTests, warningContextCanBeSet)
{
	IndexWarnings w;
	IndexWarningsContext warningContext(w, "C1: ");
	w.add("W1");

	EXPECT_EQ("C1: W1", *w.begin());
}

TEST(IndexWarningTests, multiplewarningContextsCanBeSet)
{
	IndexWarnings w;
	IndexWarningsContext warningContext1(w, "C1: ");
	IndexWarningsContext warningContext2(w, "C2: ");
	w.add("W1");

	EXPECT_EQ("C1: C2: W1", *w.begin());
}

TEST(IndexWarningTests, warningContextIsScoped)
{
	IndexWarnings w;
	{
		IndexWarningsContext warningContext(w, "C1: ");
	}
	w.add("W1");

	EXPECT_EQ("W1", *w.begin());
}

TEST(IndexWarningTests, warningContextIsAStack)
{
	IndexWarnings w;
	w.add("W1");
	{
		IndexWarningsContext warningContext(w, "C1: ");
		w.add("W2");
		{
			IndexWarningsContext warningContext(w, "C2: ");
			w.add("W3");
		}
		w.add("W4");
		{
			IndexWarningsContext warningContext(w, "C3: ");
			w.add("W5");
		}
		w.add("W6");
	}
	w.add("W7");

	EXPECT_EQ("W1", *(w.begin() + 0));
	EXPECT_EQ("C1: W2", *(w.begin() + 1));
	EXPECT_EQ("C1: C2: W3", *(w.begin() + 2));
	EXPECT_EQ("C1: W4", *(w.begin() + 3));
	EXPECT_EQ("C1: C3: W5", *(w.begin() + 4));
	EXPECT_EQ("C1: W6", *(w.begin() + 5));
	EXPECT_EQ("W7", *(w.begin() + 6));
}

TEST(IndexWarningTests, formattedWarnings)
{
	IndexWarnings w;
	w.add("%d %d %s %s", 1, 1.5, "hello", std::string("mystring"));

	EXPECT_EQ("1 1.5 hello mystring", *(w.begin()));
}

TEST(IndexWarningTests, formattedContext)
{
	IndexWarnings w;
	IndexWarningsContext warningContext(w, "%d %d %s %s", 1, 1.5, "hello", std::string("mystring"));
	w.add("W1");

	EXPECT_EQ("1 1.5 hello mystringW1", *w.begin());
}