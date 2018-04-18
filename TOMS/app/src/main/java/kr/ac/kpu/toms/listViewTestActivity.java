package kr.ac.kpu.toms;

import android.os.Bundle;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.widget.ListView;

public class listViewTestActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_listviewtest);

        ListView listView;
        ListViewAdapter adapter;

       adapter = new ListViewAdapter();

       listView = (ListView)findViewById(R.id.listview1);
       listView.setAdapter(adapter);

       for(int i=0; i<10; i++){
           adapter.addItem(ContextCompat.getDrawable(this, R.drawable.profile_blank), "타이틀"+i, "본문"+i);
       }

    }
}
