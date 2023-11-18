//
// Created by bale2 on 26/09/2023.
//

#include "sim/progargs.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>
#include <filesystem>


TEST(TestValidateTimeSteps, Test1){
  EXPECT_EQ(validate_time_steps(1), 0);
  EXPECT_EQ(validate_time_steps(0), -1);
  EXPECT_EQ(validate_time_steps(-1), -2);
}


TEST(TestValidateInputFile, Test2){
  std::filesystem::path const currentPath = std::filesystem::current_path();
  std::string const pathfile = currentPath.string() + "/";
  std::string const testpath = pathfile + "testfiles/small.fld";
  std::string nopath = pathfile + "incorrectpath";
  std::string corrpath = pathfile + "smallcorr";
  EXPECT_EQ(validate_input_file(testpath.c_str()), 0);
  EXPECT_EQ(validate_input_file(nopath.c_str()), -1);
  EXPECT_EQ(validate_input_file(corrpath.c_str()), -1);


}


TEST(TestValidateOutputFile, Test3){
  std::filesystem::path const currentPath = std::filesystem::current_path();
  std::string const pathfile = currentPath.string() + "/";
  std::string const testpath = pathfile + "output.fld";
  std::string nopath = "";
  std::string corrpath = "output.fld";
  EXPECT_EQ(validate_output_file(testpath.c_str()), 0);
  EXPECT_EQ(validate_output_file(nopath.c_str()), -1);
  //EXPECT_EQ(validate_output_file(corrpath.c_str()), -1);
}


TEST(TestValidateParameters, Test4){
  std::filesystem::path const currentPath = std::filesystem::current_path();
  std::string const pathfile = currentPath.string() + "/";
  std::array <int,2> errortype1,errortype2,errortype3, errortype4;
  std::string const file = pathfile + "small.fld";
  std::string const file2 = pathfile + "output.fld";
  errortype1 = {-3, 5};
  errortype2 = {-2,-1};
  errortype3 = {-1,0};
  errortype4 = {0,5};

  EXPECT_EQ(validate_parameters(4, {"5" ,"small.fld" ,"final.fld", "1000"}), errortype1);
  EXPECT_EQ(validate_parameters(4, {"-1" ,"small.fld" ,"final.fld"}), errortype2);
  EXPECT_EQ(validate_parameters(3, {"5", "small.fld", "final.fld"}), errortype3);
  EXPECT_EQ(validate_parameters(4, {"5", pathfile, "final.fld"}), errortype4);


}


TEST(TestReadInputFile, Test5){
  std::filesystem::path const currentPath = std::filesystem::current_path();
  std::string const testpath = currentPath.string() + "/";
  std::string const file = testpath + "testfiles/small.fld";
  Malla const malla = read_input_file(file.c_str());
  Malla const mallaux(4800, 204);
  EXPECT_EQ(malla.getNumberParticles() , 4800);
  int const nblocks = malla.getNumberBlocks()[0] *  malla.getNumberBlocks()[1] *  malla.getNumberBlocks()[2];
  EXPECT_EQ(nblocks , 4725);
}


TEST(TestWriteOutputFile, Test6){
  std::filesystem::path const currentPath = std::filesystem::current_path();
  std::string const testpath = currentPath.string() + "/";
  std::string const file = testpath + "testfiles/small.fld";
  std::string const outfile = testpath + "testfiles/output.fld";
  Malla malla = read_input_file(file.c_str());
  write_output_file(malla, outfile.c_str());
  Malla const mallaaux  = read_input_file(outfile.c_str());
  EXPECT_EQ(malla.getNumberParticles(), mallaaux.getNumberParticles());
  EXPECT_EQ(malla.getParticlesPerMeter(), mallaaux.getParticlesPerMeter());
}


TEST(TestCheckNP, Test7){
  EXPECT_NO_THROW(check_number_particles(1));
  EXPECT_THROW(check_number_particles(0), std::runtime_error);
  EXPECT_THROW(check_number_particles(-1), std::runtime_error);
}


TEST(TestCheckMissmatch, Test8){
  EXPECT_THROW(check_missmatch_particles(0,1), std::runtime_error);
  EXPECT_THROW(check_missmatch_particles(1,0), std::runtime_error);
  EXPECT_THROW(check_missmatch_particles(0,-1), std::runtime_error);
  EXPECT_THROW(check_missmatch_particles(-1,0), std::runtime_error);
  EXPECT_NO_THROW(check_missmatch_particles(0,0));

}


