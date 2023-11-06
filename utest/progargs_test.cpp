#include "sim/progargs.hpp"
#include "sim/grid.hpp"
#include "sim/block.hpp"
#include "sim/progargs.cpp"
#include "sim/grid.cpp"
#include "sim/block.cpp"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>


//Añade tu path de testfiles aquí
string testpath = "/home/carlos/CLionProjects/ac_p1/utest/testfiles/";

TEST(TestValidateTimeSteps, Test1){
  EXPECT_EQ(validate_time_steps(1), 0);
  EXPECT_EQ(validate_time_steps(0), -1);
  EXPECT_EQ(validate_time_steps(-1), -2);
}

TEST(TestValidateInputFile, Test2){
  testpath = testpath + "small.fld";
  string nopath = testpath + "incorrectpath";
  string corrpath = testpath + "smallcorr";
  EXPECT_EQ(validate_input_file(testpath.c_str()), 0);
  EXPECT_EQ(validate_input_file(nopath.c_str()), -1);
  EXPECT_EQ(validate_input_file(corrpath.c_str()), -1);


}

TEST(TestValidateOutputFile, Test3){
  testpath = testpath + "outputest/output.fld";
  string nopath = "";
  string corrpath = "output.fld";
  EXPECT_EQ(validate_output_file(testpath.c_str()), 0);
  EXPECT_EQ(validate_output_file(nopath.c_str()), -1);
  //EXPECT_EQ(validate_output_file(corrpath.c_str()), -1);

}

TEST(TestCheckNP, Test7){
  EXPECT_NO_THROW(check_np(1));
  EXPECT_THROW(check_np(0), runtime_error);
  EXPECT_THROW(check_np(-1), runtime_error);
}


int main(int argc, char ** argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}