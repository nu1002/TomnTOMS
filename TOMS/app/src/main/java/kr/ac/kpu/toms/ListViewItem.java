package kr.ac.kpu.toms;

import android.graphics.drawable.Drawable;

public class ListViewItem  {

    Drawable icon;
    String title;
    String body;

    public void setIcon(Drawable image){
        icon = image;
    }
    public void setTitle(String tText){
        title = tText;
    }
    public void setBody(String bText){
        body = bText;
    }

    public Drawable getIcon() {
        return this.icon;
    }

    public String getTitle() {
        return this.title;
    }

    public String getBody() {
        return this.body;
    }
}
