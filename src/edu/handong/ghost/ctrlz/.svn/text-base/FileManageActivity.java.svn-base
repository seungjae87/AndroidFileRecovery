package edu.handong.ghost.ctrlz;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;

import edu.handong.ghost.ctrlz.R;

import edu.handong.ghost.ctrlz.manager.FileManager;
import edu.handong.ghost.ctrlz.manager.LayoutManager;
import edu.handong.ghost.ctrlz.model.FileAdapter;

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
public class FileManageActivity extends Activity implements OnItemClickListener{

	private Context context = FileManageActivity.this;
	private String flag;
	private String src;
	private String root = "/";	// 루트 디렉토리
	private String currentPath = "/";	
	
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

	private Button actButton;
	private Button addButton;
	private Button cancelButton;
	
	private ArrayList<File> sortedList = new ArrayList<File>();
	private FileManager fileManager = FileManager.getInstance();	// 파일매니저 
	private LayoutManager layoutManager = LayoutManager.getInstance();	// 파일매니저 
	
	private FileAdapter listAdapter;
	private FileAdapter gridAdapter;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_file_manage);	// 임시 
		Intent intent = getIntent();
		flag = intent.getStringExtra("flag");
		src = intent.getStringExtra("src");
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
		listViewFile = (ListView)findViewById(R.id.fileManageListView); 
		listViewFile.setFastScrollEnabled(true);	// 스크롤이 길어지면 오른쪽에 버튼 생성 

		// 그리드뷰
		gridViewFile = (GridView)findViewById(R.id.fileManagerGridView); 
		gridViewFile.setFastScrollEnabled(true);
		
		actButton = (Button)findViewById(R.id.fileManageActBtn);
		addButton = (Button)findViewById(R.id.fileManageAddBtn);
		cancelButton = (Button)findViewById(R.id.fileManageCancelBtn);
		actButton.setOnClickListener(mClickListener);
		addButton.setOnClickListener(mClickListener);
		cancelButton.setOnClickListener(mClickListener);
		
		if(flag.equals("move")){
			actButton.setText(getResources().getString(R.string.filemanager_btn_mv_to_here));
		}else if(flag.equals("copy")){
			actButton.setText(getResources().getString(R.string.filemanager_btn_cp_to_here));
		}
		
		buttonView.setBackgroundResource(R.drawable.button_grid);
		
		root = layoutManager.getRootPath();
		// 디렉토리 불러오기 
		getDirectory(root);
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
				// 선택 모드로 변경 -> 새로운 액티비티? 
//				intent = new Intent(ExplorerActivity.this, ExplorerSelectActivity.class);
//				startActivity(intent);
//				overridePendingTransition(0,0);
				break;
			case R.id.buttonExplorerSearch:
				// 검색 액티비티 실행 
				intent = new Intent(FileManageActivity.this, ExplorerSearchActivity.class);
				startActivity(intent);
				overridePendingTransition(0,0);	// 애니메이션 없음 
				break;
			case R.id.fileManageActBtn:
				if(flag.equals("move")){
					fileManager.mvFile(src, currentPath);
				}else if(flag.equals("copy")){
					fileManager.cpFile(src, currentPath);
				}
				intent = new Intent();
				intent.putExtra("path", currentPath);
				setResult(RESULT_OK,intent);
				finish();
				break;
			case R.id.fileManageAddBtn:
				AlertDialog.Builder builder = new AlertDialog.Builder(context);
				final EditText ed = new EditText(context);
				builder.setView(ed);
				builder.setTitle(getResources().getString(R.string.filemanager_ed_title_foldername));
				builder.setPositiveButton(getResources().getString(R.string.filemanager_btn_ok), 
						new OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub
						String dirPath = currentPath.concat("/"+ed.getText().toString());
						fileManager.mkdir(dirPath);
						getDirectory(currentPath);
					}
				});
				
				builder.setNegativeButton(getResources().getString(R.string.filemanager_btn_cancel), 
						new OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub
						dialog.cancel();
					}
				});
				
				builder.show();
				break;
			case R.id.fileManageCancelBtn:
				intent = new Intent();
				setResult(RESULT_CANCELED,intent);
				finish();
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
			textViewCurrentPath.setText(R.string.title_filemanage);	// 제목을 보여준다. 
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
			list.add(getResources().getString(R.string.filemanager_msg_nofile)); // -> String으로 변경해야 함 

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

			listAdapter = new FileAdapter(this, R.layout.list_explorer, sortedList);
			gridAdapter = new FileAdapter(this, R.layout.grid_explorer, sortedList);

			listViewFile.setAdapter(listAdapter); 
			listViewFile.setOnItemClickListener(this);

			gridViewFile.setAdapter(gridAdapter); 
			gridViewFile.setOnItemClickListener(this);
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
					.setTitle("[" + file.getName() + "] " + getResources().getString(R.string.filemanager_msg_noread))
					.setPositiveButton(getResources().getString(R.string.filemanager_btn_ok) , 
							new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							//  TODO Auto-generated method stub
						}
					}).show();
				}
			}
			// 디렉토리가 아닌 파일을 선택햇을 경우 
			else{ 
				//// 파일 관련 작업을 할 수 있게 함   
				// 파일 이름 얼럿 
				new AlertDialog.Builder(this)
				.setTitle("[" + file.getName() + "]")
				.setPositiveButton(getResources().getString(R.string.filemanager_btn_ok) , 
						new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub
					}
				}).show();

				////

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
				}else{
					Intent intent = new Intent();
					setResult(RESULT_CANCELED,intent);
					finish();
				}
			}

			return super.onKeyDown(keyCode, event);
		}
		
}