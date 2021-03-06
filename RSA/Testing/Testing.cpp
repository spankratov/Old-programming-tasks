#include "gtest\gtest.h"
#include "..\RSA\giantint.h"


TEST(GiantInt, ComparingBool)
{
	gint a1 = 767, a2(-87687), a3(2147488293), a4 = 72949972960, a5 = "-92947320960", a6(9223372036854775810);
	EXPECT_TRUE(a1 < 1000);
	EXPECT_FALSE(a2 < -90000);
	EXPECT_FALSE(a1 < -886);
	EXPECT_FALSE(a3 < 2147488291);
	EXPECT_FALSE(a3 < 35);
	EXPECT_TRUE(a4 < 72949972967);
	EXPECT_TRUE(a5 < 0);
	EXPECT_TRUE(a5 < -76);
	EXPECT_FALSE(a5 < -92947320977);
	EXPECT_TRUE(a6 < 10223372036854775810);
	EXPECT_FALSE(a6 < -346583789349);
	EXPECT_TRUE(a2 < a1);
	EXPECT_FALSE(a6 < a3);
	EXPECT_TRUE(a5 < a4);
	EXPECT_TRUE(a5 < a1);
	EXPECT_FALSE(687 <= a2);
	EXPECT_TRUE(9223372036854775810 <= a6);
}

TEST(GiantInt, Arithmetic)
{
	gint a1("-892398322138812998"), a2("3922"), a3(1), a4(0), a5("-792374982"), a6("-2738972937498739287492374982374897294239"), a7("3947394349985"), a8("82634242905828940237892037284897423779562370265264738976529678666785");
	EXPECT_TRUE(gint(0) == (a1 + gint("892398322138812998")));
	EXPECT_TRUE(1 == (a3 + a4));
	EXPECT_TRUE((a6 - a7) == gint("-2738972937498739287492374986322291644224"));
	EXPECT_TRUE((a1 - a5) == gint("-892398321346438016"));
	EXPECT_TRUE((a2 - a7) == gint("-3947394346063"));
	EXPECT_TRUE((a7 - a2) == gint("3947394346063"));
	EXPECT_TRUE((a8 + a6) == gint("82634242905828940237892037282158450842063630977772363994154781372546"));
	EXPECT_TRUE((a6 + a8) == gint("82634242905828940237892037282158450842063630977772363994154781372546"));
	EXPECT_TRUE((a5 - a6) == gint("2738972937498739287492374982374104919257"));
	EXPECT_TRUE((gint(-82183892) + gint(32809183792783279)) == (-82183892 + 32809183792783279));
	EXPECT_TRUE((a6 - a4) == a6);
	EXPECT_TRUE((a3 + a4) == 1);
	EXPECT_TRUE((a2 * a5) == gint("-3107694679404"));
	EXPECT_TRUE((a1 * a6) == gint("2444254853807490862168737643783338393744880520313403718522"));
	EXPECT_TRUE((a8 * a4) == 0);
	EXPECT_TRUE((a8 * a3) == a8);
	EXPECT_TRUE((a1 / a3) == a1);
	EXPECT_TRUE((a2 / a1) == 0);
	EXPECT_TRUE((a1 / a5) == gint("1126232329"));
	EXPECT_TRUE((a6 / a2) == gint("-698361279321453158463124676791151783"));
	EXPECT_TRUE((a8 / a7) == gint("20933870695270906566079748501283794968972792695464878209"));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::system("pause");
	return 0;
}