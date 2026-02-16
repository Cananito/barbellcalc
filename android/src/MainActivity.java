package com.cananito.barbellcalc;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputFilter;
import android.text.InputType;
import android.text.TextWatcher;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.inputmethod.InputMethodManager;
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

    InputFilter[] platesEditTextFilters = new InputFilter[1];
    platesEditTextFilters[0] = new InputFilter.LengthFilter(58); // Rough upper limit.
    platesEditText = new EditText(this);
    platesEditText.setFilters(platesEditTextFilters);
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

    InputFilter[] weightEditTextFilters = new InputFilter[1];
    weightEditTextFilters[0] = new InputFilter.LengthFilter(4); // Rough upper limit.
    weightEditText = new EditText(this);
    weightEditText.setFilters(weightEditTextFilters);
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

  @Override
  public boolean dispatchTouchEvent (MotionEvent ev) {
    if (ev.getAction() == MotionEvent.ACTION_DOWN) {
      View view = this.getCurrentFocus();
      if (view != null) {
        InputMethodManager inputMethodManager =
          (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
        inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
        platesEditText.clearFocus();
        weightEditText.clearFocus();
      }
    }
    return super.dispatchTouchEvent(ev);
  }
}
