#include <gtest/gtest.h>
#include <string>
#include "mlib.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // ::testing::GTEST_FLAG(filter) = "eval.empty";
    // ::testing::GTEST_FLAG(filter) = "math_lib_test.mlib_fact";
    return RUN_ALL_TESTS();
}

TEST(math_lib_test, mlib_add) {
      double result;
      OPERATION_RESULT op;
      op = (mlib_add(3, 4, &result));
      ASSERT_TRUE(op == SUCCESS);
      EXPECT_DOUBLE_EQ(result, 7);

      op = (mlib_add(4, -2, &result));
      ASSERT_TRUE(op == SUCCESS);
      EXPECT_DOUBLE_EQ(result, 2);

      op = (mlib_add(-9, 3, &result));
      ASSERT_TRUE(op == SUCCESS);
      EXPECT_DOUBLE_EQ(result, -6);

      op = (mlib_add(7, 0, &result));
      ASSERT_TRUE(op == SUCCESS);
      EXPECT_DOUBLE_EQ(result, 7);

      op = (mlib_add(1.5, 1, &result));
      ASSERT_TRUE(op == SUCCESS);
      EXPECT_DOUBLE_EQ(result, 2.5);
    
      op = (mlib_add(1.5, 1.5, &result));
      ASSERT_TRUE(op == SUCCESS);
      EXPECT_DOUBLE_EQ(result, 3);
}

TEST(math_lib_test, mlib_add_err) {
  double result;
  OPERATION_RESULT op = mlib_add(1.6E308, 0.2E308, &result);
  ASSERT_TRUE(op == ERR_OVERFLOW);

  op = mlib_add(-0.2E308,-1.69E308 , &result);
  ASSERT_TRUE(op == ERR_OVERFLOW);

}


TEST(math_lib_test, mlib_sub){
     double result;
     OPERATION_RESULT op;
     op = (mlib_sub(4, 2, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 2);
    
     op = (mlib_sub(4, -2, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 6);
    
     op = (mlib_sub(-9, -3, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, -6);
    
     op = (mlib_sub(7, 0, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 7);
    
     op = (mlib_sub(1.5, 1, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 0.5);
    
     op = (mlib_sub(1.5, 1.25, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 0.25);
   
        
}

TEST(math_lib_test, mlib_sub_err) {
    double result;
    OPERATION_RESULT op = mlib_sub(0.2E308,-1.69E308, &result);
    ASSERT_TRUE(op == ERR_OVERFLOW);

    op = mlib_sub(-0.2E308,1.69E308 , &result);
    ASSERT_TRUE(op == ERR_OVERFLOW);
}

TEST(math_lib_test, mlib_div){

     double result;
     OPERATION_RESULT op;

     op = (mlib_div(4, 2, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 2);

     op = (mlib_div(4, -2, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, -2);

     op = (mlib_div(-9, -3, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 3);
     
 
}

TEST(math_lib_test, mlib_div_errs) {

     double result;
     OPERATION_RESULT op = mlib_div(-77, 1E-100, &result);
     ASSERT_TRUE(op == ERR_OVERFLOW);

     op = mlib_div(1.8E307, -1E-100, &result);
     ASSERT_TRUE(op == ERR_OVERFLOW);

     op = mlib_div(1.8E307, 0, &result);
     ASSERT_TRUE(op ==  ERR_DIVIDE_BY_ZERO);
    
}

TEST(math_lib_test, mlib_mul){
     double result;
    OPERATION_RESULT op;

     op = (mlib_mul(4577865, 26767564, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 122538294370860);

     op = (mlib_mul(4577865, -26767564, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, -122538294370860);
     
     op = (mlib_mul(-4577865, -26767564, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 122538294370860);

     op = (mlib_mul(0, -26767564, &result));
      ASSERT_TRUE(op == SUCCESS);
      EXPECT_DOUBLE_EQ(result, 0);
    
}

TEST(math_lib_test, mlib_mul_errs) {
     
     double result;
     OPERATION_RESULT op = mlib_mul(1.8E307, 100, &result);
     ASSERT_TRUE(op == ERR_OVERFLOW);

     op = mlib_mul(1.8E307, -100, &result);
     ASSERT_TRUE(op == ERR_OVERFLOW);
}

TEST(math_lib_test, mlib_root){

    double result;
    OPERATION_RESULT op;

     op = (mlib_root(0, 3, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 0);

     op = (mlib_root(-8, 3, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, -2.0);

     op = (mlib_root(8, 3, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 2);


}

TEST(math_lib_test, mlib_root_errs) {
    double result;
    OPERATION_RESULT op = mlib_root(-77, 2, &result);
    EXPECT_TRUE(op ==  ERR_NEG_ROOT_ARG);
   
}

TEST(math_lib_test, mlib_fact) {
    unsigned long long result;
    OPERATION_RESULT op;

     op = (mlib_fact( 0, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 1);

     op = (mlib_fact( 1, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 1);

     op = (mlib_fact(3, &result));
     ASSERT_TRUE(op == SUCCESS);
     EXPECT_DOUBLE_EQ(result, 6); 
}

TEST(math_lib_test, mlib_fact_negative) {
    unsigned long long result;
    OPERATION_RESULT op = mlib_fact(-2, &result);
    EXPECT_TRUE(op == ERR_NEG_FACT_ARG);
}

TEST(math_lib_test, mlib_fact_too_big) {
    unsigned long long result;
    OPERATION_RESULT op = mlib_fact(203, &result);
    EXPECT_TRUE(op == ERR_TOO_BIG_FACT_ARG);
}

TEST(math_lib_test, mlib_abs) {
    double result;
    OPERATION_RESULT op;

    op = (mlib_abs(4, &result));
    ASSERT_TRUE(op == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 4);

    op = (mlib_abs(-4, &result));
    ASSERT_TRUE(op == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 4);

    op = (mlib_abs(4.5, &result));
    ASSERT_TRUE(op == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 4.5);

     op = (mlib_abs(-4.5, &result));
    ASSERT_TRUE(op == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 4.5);

    op = (mlib_abs(0, &result));
    ASSERT_TRUE(op == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 0);
}

//_________EVAL________________________________________

TEST(eval, basic){
    std::string s;
    double result;
    s = "5+0.5";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 5.5);
    s = "5-0.5";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 4.5);
    s = "5*0.5";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 2.5);
    s = "5/0.5";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 10);
    s = "2^5";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 32);
    s = "9^0.5";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 3);
    s = "5!";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 120);
    s = "|-7+3|";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 4);
}

TEST(eval, empty){
    std::string s = "";
    double result;
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 0.0);
    s = " ";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 0.0);
    s = "       ";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST(eval, whitespaces){
    std::string s = "10 - 3 * 4";
    double result;
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, -2);
}

TEST(eval, nested_brackets){
    std::string s = "(10 * (2+3)) / 10";
    double result;
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 5);
}

TEST(eval, precedence){
    std::string s = "10*4+2";
    double result;
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 42);
    s = "10^2-58";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 42);
    s = "3^3*0.5";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 13.5);
    s = "2^3!";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 64);
    s = "-2*4";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, -8);
}

TEST(eval, mix){
    std::string s = " (2 *4 + 18 /(9-3) )^ ( (2+1)! - 4 )";
    double result;
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 121);
    s = "8^(1/3)";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 2);
    s = "|-6+|-1|*3|!^2";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 36);
}

TEST(eval, negation){//- not as operator but as negative number
    std::string s;
    double result;
    s = "-5 + 3";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, -2);
    s = "4 * -3";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, -12);
    s = " 1 -5 + 3";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, -1);
    s = "((-4+2) +1 ) * 10";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, -10);
    s = "10^-2";
    ASSERT_TRUE(eval(s, &result) == SUCCESS);
    EXPECT_DOUBLE_EQ(result, 0.01);
}   

TEST(eval, malformed){
    std::string s = "((2*4) - 7";
    // EXPECT_TRUE(eval(s, NULL) ==  ERR_MALFORMED_EXPRESSION);
    EXPECT_FALSE(eval(s, NULL) == SUCCESS);
    s = "1-*4";
    // EXPECT_TRUE(eval(s, NULL) ==  ERR_MALFORMED_EXPRESSION);
    EXPECT_FALSE(eval(s, NULL) == SUCCESS);
    s = "1s1 - 5";
    //EXPECT_TRUE(eval(s, NULL) ==  ERR_MALFORMED_EXPRESSION);
    EXPECT_FALSE(eval(s, NULL) == SUCCESS);
    s = "10..5 - 4";
    //EXPECT_TRUE(eval(s, NULL) ==  ERR_MALFORMED_EXPRESSION);
    EXPECT_FALSE(eval(s, NULL) == SUCCESS);
    s = "10,5 - 4";
    //EXPECT_TRUE(eval(s, NULL) ==  ERR_MALFORMED_EXPRESSION);
    EXPECT_FALSE(eval(s, NULL) == SUCCESS);
    s = "10 > 4";
    //EXPECT_TRUE(eval(s, NULL) ==  ERR_MALFORMED_EXPRESSION);
    EXPECT_FALSE(eval(s, NULL) == SUCCESS);
    s = "1234567889.1234567890 * 2";
    //EXPECT_TRUE(eval(s, NULL) ==  ERR_MALFORMED_EXPRESSION);
    EXPECT_FALSE(eval(s, NULL) == SUCCESS);
    s = "1.2 4 + 8";
    //EXPECT_TRUE(eval(s, NULL) ==  ERR_MALFORMED_EXPRESSION);
    EXPECT_FALSE(eval(s, NULL) == SUCCESS);
    s = "1(  )+2";
    //EXPECT_TRUE(eval(s, NULL) ==  ERR_MALFORMED_EXPRESSION);
    EXPECT_FALSE(eval(s, NULL) == SUCCESS);
}