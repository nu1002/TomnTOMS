package com.example.junmin.webview;

import android.app.Activity;
import android.os.Bundle;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class RecipeActivity extends Activity {
    WebView webView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_recipe);

        // 메뉴에 맞는 Youtube 동영상을 Url 을 통해 WebView 로 재생
        webView = (WebView)findViewById(R.id.webView);

        webView.getSettings().setJavaScriptEnabled(true);   //자바스크립트 사용
        webView.getSettings().setSupportMultipleWindows(true);  //여러개의 창 사용 가능 (빠져도 될듯?)
        //WebView 클라이언트 지정
        webView.setWebViewClient(new WebViewClient());
        webView.setWebChromeClient(new WebChromeClient());

        webView.loadUrl("https://www.naver.com");    //URL 링크

    }
}
