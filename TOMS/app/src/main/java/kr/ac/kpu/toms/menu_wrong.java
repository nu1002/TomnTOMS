package kr.ac.kpu.toms;

import android.os.Bundle;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.widget.ListView;

public class menu_wrong extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu_wrong);

        ListView listView;
        ListViewAdapter adapter;

        adapter = new ListViewAdapter();

        listView = (ListView)findViewById(R.id.WrongListView);
        listView.setAdapter(adapter);

        for(int i=1; i<11; i++){
            adapter.addItem(ContextCompat.getDrawable(this, R.drawable.profile_blank), "여기는"+i, "고장"+i);
        }

    }
}