//
// Created by bale2 on 26/09/2023.
//

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
  testpath = testpath + "output.fld";
  string nopath = "";
  string corrpath = "output.fld";
  EXPECT_EQ(validate_output_file(testpath.c_str()), 0);
  EXPECT_EQ(validate_output_file(nopath.c_str()), -1);
  //EXPECT_EQ(validate_output_file(corrpath.c_str()), -1);
}

TEST(TestValidateParameters, Test4){
  array <int,2> errortype1,errortype2,errortype3, errortype4;
  string const file = testpath + "small.fld";
  string const file2 = testpath + "output.fld";
  errortype1 = {-3, 5};
  errortype2 = {-2,-1};
  errortype3 = {-1,0};
  errortype4 = {0,5};

  EXPECT_EQ(validate_parameters(4, {"fluid.cpp", "5" ,"small.fld" ,"final.fld", "1000"}), errortype1);
  EXPECT_EQ(validate_parameters(4, {"fluid.cpp", "-1" ,"small.fld" ,"final.fld"}), errortype2);
  EXPECT_EQ(validate_parameters(3, {"fluid.cpp","5", "small.fld", "final.fld"}), errortype3);
  EXPECT_EQ(validate_parameters(4, {"fluid.cpp","5", testpath, "final.fld"}), errortype4);


}


TEST(TestReadInputFile, Test5){
  string const file = testpath + "small.fld";
  Malla const malla = read_input_file(file.c_str());
  Malla const mallaux(4800, 204);
  EXPECT_EQ(malla.getNp() , 4800);
  int nblocks = malla.getNBlocks()[0] *  malla.getNBlocks()[1] *  malla.getNBlocks()[2];
  EXPECT_EQ(nblocks , 4725);
}

TEST(TestWriteOutputFile, Test6){
  string file = testpath + "small.fld";
  string outfile = testpath + "output.fld";
  Malla malla = read_input_file(file.c_str());
  write_output_file(malla, outfile.c_str());
  Malla mallaaux = read_input_file(outfile.c_str());

  EXPECT_EQ(malla.getNp(), mallaaux.getNp());
  EXPECT_EQ(malla.getPpm(), mallaaux.getPpm());

}

TEST(TestCheckNP, Test7){
  EXPECT_NO_THROW(check_np(1));
  EXPECT_THROW(check_np(0), runtime_error);
  EXPECT_THROW(check_np(-1), runtime_error);
}


TEST(TestCheckMissmatch, Test11){
  EXPECT_THROW(check_missmatch_particles(0,1), runtime_error);
  EXPECT_THROW(check_missmatch_particles(1,0), runtime_error);
  EXPECT_THROW(check_missmatch_particles(0,-1), runtime_error);
  EXPECT_THROW(check_missmatch_particles(-1,0), runtime_error);
  EXPECT_NO_THROW(check_missmatch_particles(0,0));

}


