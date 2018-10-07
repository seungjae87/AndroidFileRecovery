package edu.handong.ghost.ctrlz;

import android.app.Activity;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.widget.TextView;
import edu.handong.ghost.ctrlz.manager.LayoutManager;

public class ExtraActivity extends Activity {

	// 타이틀바 
	private TextView textViewCurrentPath;
	private TextView textViewCurrentVersion;
	private LayoutManager layoutManager = LayoutManager.getInstance();	// 파일매니저 

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_extra);
		
		layoutManager.setCurrentActivity(this);
		layoutManager.setTabMenu();

		// 타이틀바 제목
		textViewCurrentPath = (TextView)findViewById(R.id.textViewExplorerTitle);
		textViewCurrentPath.setText("CTRL+Z");
		
		// AppManifest에 있는 버전 정보를 불러온다.
		textViewCurrentVersion = (TextView)findViewById(R.id.textViewCurrentVersion);
		try {
			textViewCurrentVersion.setText(getPackageManager().getPackageInfo(getPackageName(),0).versionName);
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			textViewCurrentVersion.setText("");
		}
		
	}
}
