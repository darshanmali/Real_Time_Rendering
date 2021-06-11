package com.darshan_vilas_mali.texture_on_cube_pyramid;

import androidx.appcompat.app.AppCompatActivity; 
import androidx.appcompat.widget.AppCompatTextView;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.content.pm.ActivityInfo;

public class MainActivity extends AppCompatActivity {

    private GLESView glesView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().getDecorView().setBackgroundColor(Color.rgb( 0, 0, 0));
        //object chaining

        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE |
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
                View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_FULLSCREEN |
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION|
                View.SYSTEM_UI_FLAG_FULLSCREEN);

        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        glesView = new GLESView(this);
        setContentView(glesView);

    }
}