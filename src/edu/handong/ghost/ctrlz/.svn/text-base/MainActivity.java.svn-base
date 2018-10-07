package edu.handong.ghost.ctrlz;

import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;

import edu.handong.ghost.ctrlz.R;

import edu.handong.ghost.ctrlz.recovery.RecoveryManager;
import edu.handong.ghost.ctrlz.recovery.obj.Device;

/**
 * @FileName: MainActivity.java
 * @Date	: Dec 31, 2012 
 * @작성자	: 임의상 
 * @변경이력	: 1/14 월 - 탭 구현(Tab Activity 사용)  -> 삭제 예정 
 * @수정자	: 임의상 
 * @설명		: 탭 화면 
 */
// 임시 - 버튼 
public class MainActivity extends Activity {

	Device dv;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		ArrayList<Device> deviceList = null;
		try {
			deviceList = RecoveryManager.getDeviceList();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		if(deviceList == null){
			System.out.println("Null");
		}
		else{

			for( int i = 0 ; i < deviceList.size() ; i++ ) {
				dv = (Device)deviceList.get(i);
				System.out.println("<<<<<<<<"+i+">>>>>>>");
				System.out.println(dv.getDevPath());
				System.out.println(dv.getMountDir());
				System.out.println(dv.getFsType());
				System.out.println(dv.getMountOpts());
				System.out.println(dv.getTotalSize());
				System.out.println(dv.getFreeSize());
			}
		}
	}

}

