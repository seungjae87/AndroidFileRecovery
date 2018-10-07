package edu.handong.ghost.ctrlz;

import java.io.File;
import java.net.URI;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.webkit.MimeTypeMap;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;
import edu.handong.ghost.ctrlz.manager.FileManager;
import edu.handong.ghost.ctrlz.manager.LayoutManager;
import edu.handong.ghost.ctrlz.model.DeviceAdapter;
import edu.handong.ghost.ctrlz.model.FileAdapter;
import edu.handong.ghost.ctrlz.recovery.obj.Device;

/**
 * @FileName: ExplorerActivity.java
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
public class ExplorerActivity extends Activity implements OnItemClickListener, OnItemLongClickListener {

	private String root;	// 루트 디렉토리
	private String currentPath;	
	private Context context; 
	private ArrayList<Device> deviceList;
	private DeviceAdapter adapter;
	
	private List<String> path = null;	// item 클릭시 이동할 경로 

	private File currentFile;

	private boolean isListView = true; 

	// 타이틀바 텍스트 및 버튼 
	private TextView textViewCurrentPath;
	private ImageButton buttonTop;
	private ImageButton buttonView;
	private ImageButton buttonCheck;
	private ImageButton buttonSearch;

	// 리스트뷰 
	private ListView listViewFile; 
	
	// 그리드뷰 
	private GridView gridViewFile; 

	private RelativeLayout layoutNoFileMessage; 

	ArrayList<File> sortedList = new ArrayList<File>();
	private FileManager fileManager = FileManager.getInstance();	// 파일매니저 
	private LayoutManager layoutManager = LayoutManager.getInstance();	// 파일매니저 
	
	private static final int MOVEREQ = 1;
	private static final int COPYREQ = 2;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_explorer);	// 임시 
		context = ExplorerActivity.this;
		
		layoutManager.setCurrentActivity(this);
		layoutManager.setTabMenu();
		
		// 타이틀바 버튼 
		buttonTop = (ImageButton)findViewById(R.id.buttonExplorerTop);
		buttonView = (ImageButton)findViewById(R.id.buttonExplorerView);
		buttonCheck = (ImageButton)findViewById(R.id.buttonExplorerCheck);
		buttonSearch = (ImageButton)findViewById(R.id.buttonExplorerSearch);
		
		// 타이틀바 버튼 
		buttonTop.setOnClickListener(mClickListener);
		buttonView.setOnClickListener(mClickListener);
		buttonCheck.setOnClickListener(mClickListener);
		buttonSearch.setOnClickListener(mClickListener);
		
		buttonView.setVisibility(View.VISIBLE);
		buttonCheck.setVisibility(View.VISIBLE);
		buttonSearch.setVisibility(View.VISIBLE);		
		
		// 타이틀바 제목
		textViewCurrentPath = (TextView)findViewById(R.id.textViewExplorerTitle);

		// 리스트뷰 
		listViewFile = (ListView)findViewById(R.id.listViewExplorer); 
		listViewFile.setFastScrollEnabled(true);	// 스크롤이 길어지면 오른쪽에 버튼 생성 

		// 그리드뷰
		gridViewFile = (GridView)findViewById(R.id.gridViewExplorer); 
		gridViewFile.setFastScrollEnabled(true);

		buttonView.setBackgroundResource(R.drawable.button_grid);
		
		deviceList = layoutManager.getDeviceList();
		root = layoutManager.getRootPath();
		
		Intent intent = getIntent();
		String intentPath = intent.getStringExtra("intentPath");
		
		if(intentPath == null){
			if(root == null){
				Log.i("test","root null");
				Device dev = deviceList.get(0);
				root = dev.getMountDir();
				layoutManager.setRootPath(root);
				getDirectory(root);
			}else{
				Log.i("test","root not null");
				getDirectory(root);
			}
		}else{
			if(root == null){
				Device dev = deviceList.get(0);
				root = dev.getMountDir();
				layoutManager.setRootPath(root);
				getDirectory(intentPath);
			}else{
				getDirectory(intentPath);
			}
		}
		
	}

	Button.OnClickListener mClickListener = new Button.OnClickListener(){

		public void onClick(View v){

			Intent intent;
			switch (v.getId()) {
			case R.id.buttonExplorerTop:
				// 위로 가기 -> 변경 필요 
				if(!currentPath.equals(root)) {
					getDirectory(currentFile.getParent());	
				}
				break;
			case R.id.buttonExplorerView:
				if(isListView) {
					buttonView.setBackgroundResource(R.drawable.button_list);
					listViewFile.setVisibility(View.INVISIBLE);
					gridViewFile.setVisibility(View.VISIBLE);
					isListView = false;
				} else {
					buttonView.setBackgroundResource(R.drawable.button_grid);
					listViewFile.setVisibility(View.VISIBLE);
					gridViewFile.setVisibility(View.INVISIBLE);
					isListView = true;
				}
				break;
			case R.id.buttonExplorerCheck:
				//이름변경 필요
				ArrayList<String> mnt_list= new ArrayList<String>();
				for(int i=0; i<deviceList.size(); i++)
					mnt_list.add(deviceList.get(i).getMountDir());
				
				adapter = new DeviceAdapter(ExplorerActivity.this, R.layout.list_device, deviceList);
				AlertDialog mnt_change = new AlertDialog.Builder(context)
					.setTitle(getResources().getString(R.string.explorer_dialog_title_changeroot))
					.setNegativeButton(getResources().getString(R.string.explorer_dialog_btn_cancel), 
							new DialogInterface.OnClickListener() {
						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							// TODO Auto-generated method stub
							
						}
					})
					.setAdapter(adapter, new DialogInterface.OnClickListener(){
						
						@Override
						public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub
							Device dev = deviceList.get(which);
							LayoutManager layoutManager = LayoutManager.getInstance();
							layoutManager.setRootPath(dev.getMountDir());
							root = layoutManager.getRootPath();
							getDirectory(root);
						}
					}).create();
				
				mnt_change.show();
				
				break;
			case R.id.buttonExplorerSearch:
				// 검색 액티비티 실행 
				intent = new Intent(ExplorerActivity.this, ExplorerSearchActivity.class);
				startActivity(intent);
				overridePendingTransition(0,0);	// 애니메이션 없음 
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
			textViewCurrentPath.setText(R.string.title_explorer);	// 제목을 보여준다. 
			buttonTop.setVisibility(View.INVISIBLE);	// 위로 버튼 감추기
		} else {
			textViewCurrentPath.setText(currentFile.getName());	// 현재 위치를 보여준다. 
			buttonTop.setVisibility(View.VISIBLE);	// 위로 버튼 다시 보여주기 
		}

		if(isListView) {
			listViewFile.setVisibility(View.VISIBLE);
			gridViewFile.setVisibility(View.INVISIBLE);
		} else {
			listViewFile.setVisibility(View.INVISIBLE);
			gridViewFile.setVisibility(View.VISIBLE);
		}

		File[] files = currentFile.listFiles();
		ArrayList<File> fileList = new ArrayList<File>();

		// 파일이 없다면
		if( files.length == 0 ) {
			
			// 임시 -> 클릭하면 어플이 죽음 
			ArrayList<String> list = new ArrayList<String>();
			list.add(getResources().getString(R.string.explorer_msg_nofile)); // -> String으로 변경해야 함 

			ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, list);

			listViewFile.setAdapter(arrayAdapter);
			gridViewFile.setAdapter(arrayAdapter);

		} else {
			
			Collections.addAll(fileList, files);	
			sortedList = fileManager.sortFilesByFolderAndName(fileList);

			// currentPath의 하위 디렉토리/파일의 item, path 추가
			for( int i = 0; i < sortedList.size(); i++ ) {
				File file = sortedList.get(i);
				path.add(file.getPath());
			}

			FileAdapter listAdapter = new FileAdapter(this, R.layout.list_explorer, sortedList);
			FileAdapter gridAdapter = new FileAdapter(this, R.layout.grid_explorer, sortedList);

			listViewFile.setAdapter(listAdapter); 
			listViewFile.setOnItemClickListener(this);
			listViewFile.setOnItemLongClickListener(this);

			gridViewFile.setAdapter(gridAdapter); 
			gridViewFile.setOnItemClickListener(this);
			gridViewFile.setOnItemLongClickListener(this);
		}
	}

	@Override
	public void onItemClick(AdapterView<?> parent, View v, int position, long id) {

		try {

			File file = new File(path.get(position));
			// 디렉토리를 선택했을 경우 
			if (file.isDirectory()) {
				// 폴더 접근 권한이 있는 경우 
				if(file.canRead()) {
					// 리스트뷰를 다시 생성
					getDirectory(path.get(position));
				}
				// 폴더 접근 권한이 없는 경우 
				else {
					// 접근할 수 없다는 얼럿 띄움
					new AlertDialog.Builder(this)
					.setTitle("[" + file.getName() + "] "+getResources().getString(R.string.explorer_msg_noread))
					.setPositiveButton(getResources().getString(R.string.explorer_btn_ok), 
							new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							//  TODO Auto-generated method stub
						}
					}).show();
				}
			}
			// 디렉토리가 아닌 파일을 선택햇을 경우 
			else{ 
				String fileName = file.getName();
				String extension = fileName.substring(fileName.lastIndexOf(".")+1, fileName.length()).toLowerCase();
				String mimeType;
				
				MimeTypeMap mtm = MimeTypeMap.getSingleton();
				mimeType = mtm.getMimeTypeFromExtension(extension);
				if(mtm.hasExtension(extension)){
					Uri uri = Uri.fromFile(file);
					Intent intent = new Intent(Intent.ACTION_VIEW);
					intent.setDataAndType(uri, mimeType);
					startActivity(intent);
				}else{
					Toast toast = Toast.makeText(this, getResources().getString(R.string.explorer_msg_notype), Toast.LENGTH_SHORT);
					toast.show();
				}
				
				
				
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
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
		}

		return super.onKeyDown(keyCode, event);
	}

	// 길게 누르면 편집 팝업 메뉴가 뜨게 함 -> 메뉴 선택, 작동 구현 필요 
	@Override
	public boolean onItemLongClick(AdapterView<?> parent, View v, int position, long id) {
		final File file = sortedList.get(position);
		if(!file.canRead()){
			return false;
		}
		
		new AlertDialog.Builder(this).setItems(R.array.explorer_dialog, new OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int position) {
				Intent intent;
				AlertDialog.Builder builder;
				switch(position){
				case 0://삭제
					builder = new AlertDialog.Builder(context);
					builder.setMessage(file.getName()+" "+getResources().getString(R.string.explorer_msg_delete));
					builder.setCancelable(false);
					builder.setPositiveButton(getResources().getString(R.string.explorer_btn_ok),
							new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							// TODO Auto-generated method stub
							String parent = fileManager.deleteFile(file);
							getDirectory(parent);
						}
					});
					builder.setNegativeButton(getResources().getString(R.string.explorer_btn_cancel),
							new DialogInterface.OnClickListener() {
						
						@Override
						public void onClick(DialogInterface dialog, int which) {
							// TODO Auto-generated method stub
							dialog.cancel();
						}
					});
					builder.show();
					break;
				case 1://이동 
					intent = new Intent(ExplorerActivity.this,FileManageActivity.class);
					intent.putExtra("flag", "move");
					intent.putExtra("src", file.getPath());
					startActivityForResult(intent,MOVEREQ);
					break;
				case 2://복사
					intent = new Intent(ExplorerActivity.this,FileManageActivity.class);
					intent.putExtra("flag", "copy");
					intent.putExtra("src", file.getPath());
					startActivityForResult(intent,COPYREQ);
					break;
				case 3://이름변경
					builder = new AlertDialog.Builder(context);
					final EditText ed = new EditText(context);
					ed.setText(file.getName());
					builder.setTitle(getResources().getString(R.string.explorer_dialog_title_changename));
					builder.setView(ed);
					
					builder.setPositiveButton(getResources().getString(R.string.explorer_btn_ok)
							, new OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							// TODO Auto-generated method stub
							String parent = file.getParent();
							String path = parent.concat("/"+ed.getText().toString());
							File newPath = new File(path);
							file.renameTo(newPath);
							getDirectory(parent);
							
						}
					});
					
					builder.setNegativeButton( getResources().getString(R.string.explorer_btn_cancel)
							, new OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							// TODO Auto-generated method stub
							dialog.cancel();
						}
					});
					
					builder.show();
					break;
				}
			}
		}).show();
		return false;
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data){
		switch(requestCode){
		case MOVEREQ:
		case COPYREQ:
			if(resultCode!=RESULT_CANCELED){
				String path = data.getStringExtra("path");
				getDirectory(path);
			}
			break;
		default:
			break;
		}
	}
}