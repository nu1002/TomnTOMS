package kr.ac.kpu.toms;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

public class menu_write extends AppCompatActivity {
    Button listViewBtn;
    Button webViewBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu_write);


        listViewBtn = (Button)findViewById(R.id.listViewBtn);
        webViewBtn = (Button)findViewById(R.id.webViewBtn);

        listViewBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(menu_write.this, listViewTestActivity.class);
                startActivity(intent);

            }
        });
        webViewBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(menu_write.this, WebViewTestActivity.class);
                startActivity(intent);
            }
        });
    }
}
