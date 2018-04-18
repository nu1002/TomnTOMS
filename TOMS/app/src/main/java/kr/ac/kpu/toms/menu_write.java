package kr.ac.kpu.toms;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;

public class menu_write extends AppCompatActivity {
    Button listViewBtn;
    Button webViewBtn;
    ListView reviewList;
    ListView wrongList;
    ListViewAdapter reviewAdapter;
    ListViewAdapter wrongAdapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu_write);

        reviewAdapter = new ListViewAdapter();
        wrongAdapter = new ListViewAdapter();

        listViewBtn = (Button)findViewById(R.id.listViewBtn);
        webViewBtn = (Button)findViewById(R.id.webViewBtn);

        reviewList =(ListView)findViewById(R.id.writeReview);
        reviewList.setAdapter(reviewAdapter);

        wrongList = (ListView)findViewById(R.id.writeWrong);
        wrongList.setAdapter(wrongAdapter);


        for(int i=0; i<10; i++){
            reviewAdapter.addItem(ContextCompat.getDrawable(this, R.drawable.profile_blank), "리뷰타이틀"+i, "리뷰본문"+i);
        }
        for(int i=0; i<10; i++){
            wrongAdapter.addItem(ContextCompat.getDrawable(this, R.drawable.profile_blank), "고장타이틀"+i, "고장본문"+i);
        }



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
