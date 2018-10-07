package edu.handong.ghost.ctrlz;

import java.util.ArrayList;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.TextView;
import edu.handong.ghost.ctrlz.manager.LayoutManager;
import edu.handong.ghost.ctrlz.model.DeviceAdapter;
import edu.handong.ghost.ctrlz.recovery.RecoveryManager;
import edu.handong.ghost.ctrlz.recovery.obj.Device;

/**
 * @FileName: InformationActivity.java
 * @Date	: Dec 31, 2012 
 * @작성자	: 임의상 
 * @변경이력	: 1/17 목 - 디바이스 정보 출력
 * @수정자	: 임의상
 * @설명		: 디바이스 및 어플리케이션 정보 출력 
 */
public class InformationActivity extends Activity implements OnItemClickListener {

	// 타이틀바 
	private TextView textViewCurrentPath;

	// 디바이스 정보 리스트뷰
	private ListView listViewDevice;
	private ArrayList<Device> deviceList;
	private LayoutManager layoutManager = LayoutManager.getInstance();	// 파일매니저 
	private DeviceAdapter adapter;

	private ProgressDialog dialog;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_information);

		layoutManager.setCurrentActivity(this);
		layoutManager.setTabMenu();
	
		// 타이틀바 제목
		textViewCurrentPath = (TextView)findViewById(R.id.textViewExplorerTitle);
		textViewCurrentPath.setText(R.string.title_device);

		new InitializeDevice().execute();

		// 디바이스 정보 리스트뷰
		listViewDevice = (ListView)findViewById(R.id.listViewDevice);
		listViewDevice.setOnItemClickListener(this);

	}

	@Override
	public void onItemClick(AdapterView<?> parentView, View view, int position, long id) {
		// TODO Auto-generated method stub
		Device dev = deviceList.get(position);
		String rootPath = dev.getMountDir();
		layoutManager.setRootPath(rootPath);
		Intent intent = new Intent(InformationActivity.this,ExplorerActivity.class);
		startActivity(intent);
		finish();
	}

	private class InitializeDevice extends AsyncTask<Void, Void, Void>{

		protected void onPreExecute(){
			showDialog(0);
		}

		@Override
		protected Void doInBackground(Void... arg0) {
			try {				
				deviceList = RecoveryManager.InitializeRecover();
			} catch (Exception e) {
				showDialog(1);
			}
			return null;
		}

		@Override
		protected void onPostExecute(Void arg0){
			removeDialog(0);
			if(deviceList != null){
				adapter = new DeviceAdapter(InformationActivity.this, R.layout.list_device, deviceList);
				listViewDevice.setAdapter(adapter); 
				layoutManager.setDeviceList(deviceList);
			}			
		}
	}

	@Override
	protected Dialog onCreateDialog(int id) {
		switch(id){
		case 0:
			dialog = new ProgressDialog(this);
			dialog.setMessage(getResources().getString(R.string.device_activity_prgmsg_fetch));
			dialog.setIndeterminate(true);
			return dialog;
		case 1:
			//dialog = new ProgressDialog(this);
			dialog.setMessage(getResources().getString(R.string.device_activity_prgmsg_error));
			dialog.setIndeterminate(true);
			return dialog;
		default:
			return null;
		}
	}
}
