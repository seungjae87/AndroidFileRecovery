package edu.handong.ghost.ctrlz.layout;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.LinearLayout;

import edu.handong.ghost.ctrlz.R;

/**
 * @FileName: TabMenu.java
 * @Date	: Jan 14, 2013 
 * @작성자	: 임의상 
 * @설명		: 탭메뉴 레이아웃 
 */
public class TabMenu extends LinearLayout{
	Context context;

	public TabMenu(Context context, AttributeSet attrs) {
		super(context, attrs);
		this.context = context;
		
		LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		inflater.inflate(R.layout.layout_tab, this, true);
	}
}