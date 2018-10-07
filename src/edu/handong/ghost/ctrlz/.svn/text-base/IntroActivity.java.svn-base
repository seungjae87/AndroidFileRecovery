package edu.handong.ghost.ctrlz;

import edu.handong.ghost.ctrlz.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;

/**
 * @FileName: IntroActivity.java
 * @Date	: Dec 31, 2012 
 * @작성자	: 임의상 
 * @변경이력	: 1/14 월 - 시작화면 후 기기 정보가 나오도록 변경 `
 * @수정자	: 임의상
 * @설명		: 시작 화면 
*/
public class IntroActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_intro);
	
		Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            public void run() {
                Intent intent = new Intent(IntroActivity.this, InformationActivity.class);
                startActivity(intent);
                
                overridePendingTransition(android.R.anim.fade_in, android.R.anim.fade_out);	// 시작화면 사라지는 애니메이션 
                		
                finish();	// 종료시켜서 화면이 넘어간 후에 뒤로가기를 누른 경우 나오지 않게 함  
            }
        }, 2000);   
	}

	

}
