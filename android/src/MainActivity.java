package com.cananito.barbellcalc;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.ViewGroup.LayoutParams;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MainActivity extends Activity {
  public native String weightToPlates(String weight);
  public native String platesToWeight(String plates);
  private EditText platesEditText;
  private EditText weightEditText;

  static {
    System.loadLibrary("calc");
  }

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    // TODO: Remove focus from EditTexts when dismissing keyboard or tapping outside of them.

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

    // TODO: Restrict keyboard to numbers and symbols.
    platesEditText = new EditText(this);
    platesEditText.setLayoutParams(new LayoutParams(
        LayoutParams.MATCH_PARENT,
        LayoutParams.WRAP_CONTENT));
    platesEditText.addTextChangedListener(new TextWatcher() {
      @Override
      public void onTextChanged(CharSequence s, int start, int before, int count) {
      }
      @Override
      public void afterTextChanged(Editable s) {
        if (platesEditText.isFocused()) {
          String weight = platesToWeight(s.toString());
          weightEditText.setText(weight);
        }
      }
      @Override
      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
      }
    });

    TextView weightTextView = new TextView(this);
    weightTextView.setText("Weight (lbs):");
    weightTextView.setLayoutParams(new LayoutParams(
        LayoutParams.MATCH_PARENT,
        LayoutParams.WRAP_CONTENT));

    // TODO: Restrict keyboard to numbers.
    weightEditText = new EditText(this);
    weightEditText.setLayoutParams(new LayoutParams(
        LayoutParams.MATCH_PARENT,
        LayoutParams.WRAP_CONTENT));
    weightEditText.addTextChangedListener(new TextWatcher() {
      @Override
      public void onTextChanged(CharSequence s, int start, int before, int count) {
      }
      @Override
      public void afterTextChanged(Editable s) {
        if (weightEditText.isFocused()) {
          String plates = weightToPlates(s.toString());
          platesEditText.setText(plates);
        }
      }
      @Override
      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
      }
    });

    linearLayout.addView(platesTextView);
    linearLayout.addView(platesEditText);
    linearLayout.addView(weightTextView);
    linearLayout.addView(weightEditText);
    setContentView(linearLayout);
  }
}
