package edu.handong.ghost.ctrlz;

import java.io.File;
import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ListView;

import edu.handong.ghost.ctrlz.R;

import edu.handong.ghost.ctrlz.model.FileAdapter;

/**
 * @FileName: ExplorerSelectActivity.java
 * @Date	: Jan 8, 2013 
 * @작성자	: 임의상 
 * @변경이력	: 1/8 생성 -> 임시 구현용 
 * @수정자	: 
 * @설명		: 파일 다중선택시 뜨는 창 
*/
public class ExplorerSelectActivity extends ExplorerActivity {

	private ListView listViewFile; 
	
	ArrayList<File> fileList = new ArrayList<File>();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		LayoutInflater inflater = (LayoutInflater) this.getSystemService(LAYOUT_INFLATER_SERVICE);

		LinearLayout layout = new LinearLayout(this);
		layout.setOrientation(LinearLayout.VERTICAL);
		View editLayout = (View)inflater.inflate(R.layout.edit_explorer, null);
		View activityLayout = (View)inflater.inflate(R.layout.activity_explorer, null);
		layout.addView(editLayout);		
		layout.addView(activityLayout);		
		setContentView(layout);

		listViewFile = (ListView)findViewById(R.id.listViewExplorer); 
		listViewFile.setFastScrollEnabled(true);		
		
		// 임시 구현 
		FileAdapter fileAdapter = new FileAdapter(ExplorerSelectActivity.this, R.layout.list_multiple_explorer, fileList);
		listViewFile.setAdapter(fileAdapter); 
		listViewFile.setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);
		
	}

}
