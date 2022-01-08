package com.darshan_vilas_mali.hello;

import androidx.appcompat.app.AppCompatActivity; 
import androidx.appcompat.widget.AppCompatTextView;
import android.graphics.Color;
import android.view.Gravity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().getDecorView().setBackgroundColor(Color.rgb( 0, 0, 0));
        //object chaining

        AppCompatTextView myView = new AppCompatTextView(getApplicationContext());

        myView.setText("Hello word!!!");
        myView.setTextSize(32);
        myView.setTextColor(Color.rgb(0, 255, 0));
        myView.setGravity(Gravity.CENTER);
        myView.setBackgroundColor(Color.rgb( 0, 0, 0));

        setContentView(myView);

    }
}