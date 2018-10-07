package edu.handong.ghost.ctrlz.manager;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import edu.handong.ghost.ctrlz.ExplorerActivity;
import edu.handong.ghost.ctrlz.ExtraActivity;
import edu.handong.ghost.ctrlz.InformationActivity;
import edu.handong.ghost.ctrlz.R;
import edu.handong.ghost.ctrlz.RecoveryActivity;
import edu.handong.ghost.ctrlz.recovery.obj.Device;

/**
 * @FileName: LayoutManager.java
 * @Date	: Jan 16, 2013 
 * @작성자	: 임의상 
 * @설명		: 타이틀바/탭뷰 레이아웃 관리 - singleton 패턴 
 */
public class LayoutManager {

	private Activity currentActivity;
	private String rootPath;
	private ArrayList<Device> deviceList;

	// 상단 타이틀바 버튼
	private ImageButton buttonTop;
	private ImageButton buttonView;
	private ImageButton buttonCheck;
	private ImageButton buttonSearch;

	// 하단 탭 버튼 
	private ImageButton tabButton01; 
	private ImageButton tabButton02;
	private	ImageButton tabButton03;
	private	ImageButton tabButton04;

	private static LayoutManager layoutManager = new LayoutManager();

	private LayoutManager() {
		return;
	}

	public static LayoutManager getInstance() {
		return layoutManager;
	}
	
	public void destroyInstance(){
		this.layoutManager = null;
	}
	// rootPath 설정
	public void setRootPath(String rootPath){
		this.rootPath = rootPath;
	}
	
	public String getRootPath(){
		return rootPath;
	}
	
	public void setDeviceList(ArrayList<Device> deviceList){
		this.deviceList = deviceList;
	}
	
	public ArrayList<Device> getDeviceList(){
		return this.deviceList;
	}

	// 현재 액티비티를 받아옴
	public void setCurrentActivity(Activity activity) {
		currentActivity = activity;
	}

	// 탭 메뉴를 세팅 
	public void setTabMenu() {
		tabButton01 = (ImageButton)currentActivity.findViewById(R.id.tabButton01);
		tabButton02 = (ImageButton)currentActivity.findViewById(R.id.tabButton02);
		tabButton03 = (ImageButton)currentActivity.findViewById(R.id.tabButton03);
		tabButton04 = (ImageButton)currentActivity.findViewById(R.id.tabButton04);
		setTabMenuClickListener();
		setTabMenuFocus();
	}

	// 탭 메뉴 클릭 리스너 등록 
	private void setTabMenuClickListener() {
		tabButton01.setOnClickListener(mClickListener);
		tabButton02.setOnClickListener(mClickListener);
		tabButton03.setOnClickListener(mClickListener);
		tabButton04.setOnClickListener(mClickListener);
	}

	// 탭 메뉴 이미지(또는 색) 변경 
	private void setTabMenuFocus() {
		if(currentActivity instanceof InformationActivity) {
			tabButton01.setBackgroundResource(R.drawable.tab_button_on);
		} else if(currentActivity instanceof RecoveryActivity) {
			tabButton02.setBackgroundResource(R.drawable.tab_button_on);
		} else if(currentActivity instanceof ExplorerActivity) {
			tabButton03.setBackgroundResource(R.drawable.tab_button_on);
		} else if(currentActivity instanceof ExtraActivity) {
			tabButton04.setBackgroundResource(R.drawable.tab_button_on);
		} 
	}


	// 클릭 리스너 
	private Button.OnClickListener mClickListener = new Button.OnClickListener(){

		public void onClick(View v){

			Intent intent;
			switch (v.getId()) {
			case R.id.tabButton01:
				if(!(currentActivity instanceof InformationActivity)) {
					intent = new Intent(currentActivity, InformationActivity.class);
					currentActivity.startActivity(intent);
					currentActivity.overridePendingTransition(0,0);
					currentActivity.finish();
				}
				break;
			case R.id.tabButton02:
				if(!(currentActivity instanceof RecoveryActivity)) {
					intent = new Intent(currentActivity, RecoveryActivity.class);
					currentActivity.startActivity(intent);
					currentActivity.overridePendingTransition(0,0);
					currentActivity.finish();
				}
				break;
			case R.id.tabButton03:
				if(!(currentActivity instanceof ExplorerActivity)) {
					intent = new Intent(currentActivity, ExplorerActivity.class);
					currentActivity.startActivity(intent);
					currentActivity.overridePendingTransition(0,0);
					currentActivity.finish();
				}
				break;
			case R.id.tabButton04:
				if(!(currentActivity instanceof ExtraActivity)) {
					intent = new Intent(currentActivity, ExtraActivity.class);
					currentActivity.startActivity(intent);
					currentActivity.overridePendingTransition(0,0);
					currentActivity.finish();
				}
				break;
			default:
				break;
			}
		}
	};
}
