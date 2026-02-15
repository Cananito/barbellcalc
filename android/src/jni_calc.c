#include <jni.h>

#include "calc.h"

jstring Java_com_cananito_barbellcalc_MainActivity_platesToWeight(
    JNIEnv *env,
    __unused jobject this,
    jstring plates) {
  char const* const plates_c_string = (*env)->GetStringUTFChars(env, plates, 0);
  char result[8] = { 0 };
  calc_plates_to_weight(result, plates_c_string);
  return (*env)->NewStringUTF(env, result);
}

jstring Java_com_cananito_barbellcalc_MainActivity_weightToPlates(
    JNIEnv *env,
    __unused jobject this,
    jstring weight) {
  char const* const weight_c_string = (*env)->GetStringUTFChars(env, weight, 0);
  char result[60] = { 0 };
  calc_weight_to_plates(result, weight_c_string);
  return (*env)->NewStringUTF(env, result);
}
