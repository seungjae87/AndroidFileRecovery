package edu.handong.ghost.ctrlz.layout;


import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.RelativeLayout;
import edu.handong.ghost.ctrlz.R;

/**
 * @FileName: TitleBar.java
 * @Date	: Jan 14, 2013 
 * @작성자	: 임의상
 * @설명		: 탭메뉴 레이아웃 
 */
public class TitleBar extends RelativeLayout{
	Context context;
	
	public TitleBar(Context context, AttributeSet attrs) {
		super(context, attrs);
		this.context = context;
		
		LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		inflater.inflate(R.layout.layout_title, this, true);
	}
}