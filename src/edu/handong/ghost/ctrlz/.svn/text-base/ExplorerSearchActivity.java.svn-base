package edu.handong.ghost.ctrlz;

import java.io.File;
import java.util.ArrayList;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import edu.handong.ghost.ctrlz.manager.FileManager;
import edu.handong.ghost.ctrlz.manager.LayoutManager;
import edu.handong.ghost.ctrlz.model.FileSearchAdapter;

/**
 * @FileName: ExplorerSearchActivity.java
 * @Date	: Jan 8, 2013 
 * @작성자	: 임의상 
 * @변경이력	: 1/8 화 - 검색 UI
 * @수정자	: 임의상 
 * @설명		: 파일 관리 시스템 내 검색 액티비티 
*/
public class ExplorerSearchActivity extends Activity {
	
	private EditText search_edit;
	private Button search_btn;
	private ListView search_listView;
	private String root;
	private LayoutManager layoutManager = LayoutManager.getInstance();
	private FileManager fileManager = FileManager.getInstance();
	private ArrayList<File> searchList;
	private ProgressDialog dialog;
	private FileSearchAdapter listAdapter;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_explorer_search);
		
		search_edit = (EditText)findViewById(R.id.editTextExplorerSearch);
		search_btn = (Button)findViewById(R.id.buttonExplorerSearchSearch);
		search_btn.setText(getResources().getString(R.string.file_search_btn));
		
		search_listView = (ListView)findViewById(R.id.listViewExplorerSearch);
		search_listView.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position,long id) {
				// TODO Auto-generated method stub
				File tmpFile = searchList.get(position);
				Intent intent = new Intent(ExplorerSearchActivity.this,ExplorerActivity.class);
				
				if(tmpFile.isFile()){
					File p_file = tmpFile.getParentFile();
					String p_path = p_file.getPath();
					intent.putExtra("intentPath", p_path);
				}else if(tmpFile.isDirectory()){
					String path = tmpFile.getPath();
					intent.putExtra("intentPath", path);
				}
				intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
				startActivity(intent);
				finish();
			}
			
		});
		root = layoutManager.getRootPath();
		
		search_btn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				String search_word = search_edit.getText().toString();
				new SearchFile().execute(search_word);
			}
		});
	}
	
	private class SearchFile extends AsyncTask<String, Void, Void>{

		
		@Override
		protected void onPreExecute() {
			// TODO Auto-generated method stub
			dialog = ProgressDialog.show(ExplorerSearchActivity.this, "", getResources().getString(R.string.file_search_progress));
			dialog.show();
			super.onPreExecute();
		}

		@Override
		protected Void doInBackground(String... arg0) {
			// TODO Auto-generated method stub
			fileManager.searchEntry(root, arg0[0]);
			searchList = fileManager.searchEntry(root, arg0[0]);
			
			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			// TODO Auto-generated method stub
			dialog.dismiss();
			listAdapter = new FileSearchAdapter(ExplorerSearchActivity.this,R.layout.list_search_file,searchList);
			search_listView.setAdapter(listAdapter);
			super.onPostExecute(result);
		}
		
	}
	
	
}
