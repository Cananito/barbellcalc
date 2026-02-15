package com.cananito.barbellcalc;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.view.ViewGroup.LayoutParams;
import android.widget.EditText;
import android.widget.LinearLayout;
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

    LinearLayout linearLayout = new LinearLayout(this);
    linearLayout.setOrientation(LinearLayout.VERTICAL);
    linearLayout.setLayoutParams(new LinearLayout.LayoutParams(
        LinearLayout.LayoutParams.MATCH_PARENT,
        LinearLayout.LayoutParams.MATCH_PARENT));

    TextView platesTextView = new TextView(this);
    platesTextView.setText("Plates (lbs):");
    platesTextView.setLayoutParams(new LayoutParams(
        LayoutParams.MATCH_PARENT,
        LayoutParams.WRAP_CONTENT));

    EditText platesEditText = new EditText(this);
    platesEditText.setLayoutParams(new LayoutParams(
        LayoutParams.MATCH_PARENT,
        LayoutParams.WRAP_CONTENT));

    TextView weightTextView = new TextView(this);
    weightTextView.setText("Weight (lbs):");
    weightTextView.setLayoutParams(new LayoutParams(
        LayoutParams.MATCH_PARENT,
        LayoutParams.WRAP_CONTENT));

    EditText weightEditText = new EditText(this);
    weightEditText.setLayoutParams(new LayoutParams(
        LayoutParams.MATCH_PARENT,
        LayoutParams.WRAP_CONTENT));

    linearLayout.addView(platesTextView);
    linearLayout.addView(platesEditText);
    linearLayout.addView(weightTextView);
    linearLayout.addView(weightEditText);
    setContentView(linearLayout);

    String plates = weightToPlates("255");
    String weight = platesToWeight("45,45");
  }
}
