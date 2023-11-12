//
// Created by bale2 on 26/09/2023.
//
/*
#include "sim/progargs.hpp"
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

//TEST(TestValidateParameters, Test4){
  //array <int,2> errortype;
  //errortype = {0,0};
  //EXPECT_EQ(validate_parameters(5 ,"small.fld" ,"final.fld", 1000), errortype);
  //EXPECT_EQ(validate_parameters(-1 ,"small.fld" ,"final.fld"), errortype);
  //EXPECT_EQ(validate_parameters(5, "small", "final.fld"), errortype);


//}


TEST(TestReadInputFile, Test5){
  Malla malla;
  string file = testpath + "small.fld";
  read_input_file(malla, file.c_str());
  EXPECT_EQ(malla.np, 4800);
  EXPECT_EQ(malla.blocks.size(), 4725);
}

TEST(TestWriteOutputFile, Test6){
  Malla malla;
  Malla mallaaux;
  string file = testpath + "small.fld";
  string outfile = testpath + "output.fld";
  read_input_file(malla, file.c_str());
  write_output_file(malla, outfile.c_str());
  read_input_file(mallaaux, outfile.c_str());

  EXPECT_EQ(malla.np, mallaaux.np);
  EXPECT_EQ(malla.ppm, mallaaux.ppm);

}

TEST(TestCheckNP, Test7){
  EXPECT_NO_THROW(check_np(1));
  EXPECT_THROW(check_np(0), runtime_error);
  EXPECT_THROW(check_np(-1), runtime_error);
}

//TEST(TestRefactorGordo, Test8){
  // EXPECT_EQ
//}
//
//TEST(TestCalculateBlockIndexes Test9){
//  EXPECT_EQ
//}
//TEST(TestInsertParticleInfo, Test10){
//  EXPECT_EQ
//}
TEST(TestCheckMissmatch, Test11){
  EXPECT_THROW(check_missmatch_particles(0,1), runtime_error);
  EXPECT_THROW(check_missmatch_particles(1,0), runtime_error);
  EXPECT_THROW(check_missmatch_particles(0,-1), runtime_error);
  EXPECT_THROW(check_missmatch_particles(-1,0), runtime_error);
  EXPECT_NO_THROW(check_missmatch_particles(0,0));

}

*/
