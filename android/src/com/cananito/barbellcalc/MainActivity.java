package com.cananito.barbellcalc;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {
  public native void lala();

  static {
    System.loadLibrary("calc");
  }

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    lala();
  }
}
