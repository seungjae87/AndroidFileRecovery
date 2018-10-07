package edu.handong.ghost.ctrlz;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import edu.handong.ghost.ctrlz.manager.FileManager;
import edu.handong.ghost.ctrlz.manager.LayoutManager;
import edu.handong.ghost.ctrlz.model.FileAdapter;
import edu.handong.ghost.ctrlz.recovery.obj.Device;

/**
 * @FileName: RecoveryFolderSearchActivity.java
 * @Date	: Dec 31, 2012 
 * @작성자	: 임의상 
 * @변경이력	: 1/2 수 - 파일 관리 시스템 기본 구조 
 * 			  1/7 월 - 타이틀바 수정, 폴더/이름 정렬 등  
 * 		      1/8 화 - 타이틀바 UI 수정, 버튼 추가 
 * 			  1/9 수 - UI 변경, 버그 수정
 * 			  1/10 목 - 코드 정리, 하위 폴더에서 Back버튼을 누르면 상위 폴더로 올라감  
 * 			  1/14 월 - 탭추가, 리스트뷰-그리드뷰 연동, 코드 정리
 * @수정자	: 임의상 
 * @설명		: 파일 관리 시스템 
 */
public class RecoveryFolderSearchActivity extends Activity implements OnItemClickListener, OnItemLongClickListener{

	//private Context context = FileManageActivity.this;
	private String flag;
	private String src;
	private String root;	// 루트 디렉토리
	private String currentPath = "/";	
	
	private ArrayList<Device> deviceList;

	private List<String> path = null;	// item 클릭시 이동할 경로 

	private File currentFile;

	// 타이틀바 텍스트 및 버튼 
	private TextView textViewCurrentPath;
	private ImageButton buttonTop;

	// 리스트뷰 
	private ListView listViewFile;

	private ArrayList<File> sortedList = new ArrayList<File>();
	private FileManager fileManager = FileManager.getInstance();	// 파일매니저 
	private LayoutManager layoutManager = LayoutManager.getInstance();	// 파일매니저

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_recovery_folder);	// 임시 

		// 타이틀바 버튼 
		buttonTop = (ImageButton)findViewById(R.id.buttonExplorerTop);

		// 타이틀바 버튼 
		buttonTop.setOnClickListener(mClickListener);	
		// 타이틀바 제목
		textViewCurrentPath = (TextView)findViewById(R.id.textViewExplorerTitle);

		// 리스트뷰 
		listViewFile = (ListView)findViewById(R.id.recoverFolderListView); 
		listViewFile.setFastScrollEnabled(true);	// 스크롤이 길어지면 오른쪽에 버튼 생성 

		
		deviceList = layoutManager.getDeviceList();
		root = layoutManager.getRootPath();
		
		if(root == null){
			Device dev = deviceList.get(0);
			root = dev.getMountDir();
			layoutManager.setRootPath(root);
			getDirectory(root);
		}else{
			getDirectory(root);
		}
		
		Toast.makeText(RecoveryFolderSearchActivity.this, getResources().getString(R.string.recovery_explain), Toast.LENGTH_SHORT).show();
	}

	Button.OnClickListener mClickListener = new Button.OnClickListener(){

		public void onClick(View v){
			switch (v.getId()) {
			case R.id.buttonExplorerTop:
				// 위로 가기 -> 변경 필요 
				if(!currentPath.equals(root)) {
					getDirectory(currentFile.getParent());	
				}
				break;
			default:
				break;
			}
		}

	};

	/**
	 * @Method Name  : getDirectory
	 * @작성일   : Jan 2, 2013 
	 * @작성자   : 임의상 
	 * @변경이력	: 1/2 수 
	 * @Method 설명 : 
	 * @param directoryPath
	 */
	private void getDirectory(String directoryPath) {

		// 현재 경로 출력 
		currentPath = directoryPath;

		path = new ArrayList<String>();

		// 현재 path의 file 객체 생성 
		currentFile = new File(currentPath);

		// 
		if(currentPath.equals(root)) {
			textViewCurrentPath.setText(root);	// 제목을 보여준다. 
			buttonTop.setVisibility(View.INVISIBLE);	// 위로 버튼 감추기
		} else {
			textViewCurrentPath.setText(currentFile.getName());	// 현재 위치를 보여준다. 
			buttonTop.setVisibility(View.VISIBLE);	// 위로 버튼 다시 보여주기 
		}

		File[] files = currentFile.listFiles();
		ArrayList<File> fileList = new ArrayList<File>();

		// 파일이 없다면
		if( files.length == 0 ) {

			// 임시 -> 클릭하면 어플이 죽음 
			ArrayList<String> list = new ArrayList<String>();
			list.add(getResources().getString(R.string.recovery_msg_nofile)); // -> String으로 변경해야 함 

			ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, list);

			listViewFile.setAdapter(arrayAdapter);

		} else {		

			Collections.addAll(fileList, files);

			// 임시로 구현, 수정필요함
			int check=0;
			while(true){
				if(fileList.size() == check)
					break;
				else{
					if(!fileList.get(check).isDirectory()){
						fileList.remove(check);
						check--;
					}
				}
				check++;
			}

			sortedList = fileManager.sortFilesByFolderAndName(fileList);

			// currentPath의 하위 디렉토리/파일의 item, path 추가
			for( int i = 0; i < sortedList.size(); i++ ) {
				File file = sortedList.get(i);
				path.add(file.getPath());
			}

			FileAdapter listAdapter = new FileAdapter(this, R.layout.list_explorer, sortedList);
			listViewFile.setAdapter(listAdapter); 
			listViewFile.setOnItemClickListener(this);
			listViewFile.setOnItemLongClickListener(this);
		}
	}

	@Override
	public void onItemClick(AdapterView<?> parent, View v, int position, long id) {

		try {

			File file = new File(path.get(position));
			// 폴더 접근 권한이 있는 경우 
			if(file.canRead()) {
				// 리스트뷰를 다시 생성
				getDirectory(path.get(position));
			}
			// 폴더 접근 권한이 없는 경우 
			else {
				// 접근할 수 없다는 얼럿 띄움
				new AlertDialog.Builder(this)
				.setTitle("[" + file.getName() + "] "+ getResources().getString(R.string.recovery_msg_noread))
				.setPositiveButton("OK", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						//  TODO Auto-generated method stub
					}
				}).show();
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	// 길게 누르면 편집 팝업 메뉴가 뜨게 함 -> 메뉴 선택, 작동 구현 필요 
	@Override
	public boolean onItemLongClick(AdapterView<?> parent, View v, int position, long id) {
		final File file = sortedList.get(position);
		if(!file.canRead()){
			return false;
		}
		else{
			Intent intent = new Intent();
			intent.putExtra("path", file.getPath());
			setResult(RESULT_OK, intent);
			finish();
			return true;
		}

	}


	// 뒤로가기 버튼을 눌렀을 때의 작동
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if(keyCode == KeyEvent.KEYCODE_BACK) {
			// 위로 버튼을 눌렀을 때랑 동일하게 작동 
			if(!currentPath.equals(root)) {
				getDirectory(currentFile.getParent());	
				return false;	// 종료 안 되게 함 
			}
			else{
				Intent intent = new Intent();
				setResult(RESULT_CANCELED, intent);
				finish();
				return true;
			}
		}

		return super.onKeyDown(keyCode, event);
	}

}
