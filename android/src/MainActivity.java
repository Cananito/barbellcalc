package com.cananito.barbellcalc;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class MainActivity extends Activity {
  public native String weightToPlates(String weight);
  public native String platesToWeight(String plates);

  static {
    System.loadLibrary("calc");
  }

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    RelativeLayout relativeLayout = new RelativeLayout(this);
    RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(
        RelativeLayout.LayoutParams.WRAP_CONTENT,
        RelativeLayout.LayoutParams.WRAP_CONTENT);
    TextView textView = new TextView(this);
    textView.setText("barbellcalc");
    textView.setLayoutParams(layoutParams);
    relativeLayout.addView(textView);
    setContentView(relativeLayout);

    String plates = weightToPlates("255");
    String weight = platesToWeight("45,45");
  }
}
