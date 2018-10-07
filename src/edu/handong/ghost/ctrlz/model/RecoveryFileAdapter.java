package edu.handong.ghost.ctrlz.model;

import java.util.List;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.TextView;
import edu.handong.ghost.ctrlz.R;
import edu.handong.ghost.ctrlz.recovery.obj.RecoverFile;

public class RecoveryFileAdapter extends ArrayAdapter<RecoverFile> {
	private Context context;
	private int textViewResourceId;
	private List<RecoverFile> list;
	private boolean[] isChecked;
	private boolean checkedAll;
	
	private class ViewHolder{
		public ImageView imageViewPossibility;
		public ImageView imageViewFileImage;
		public TextView textViewFileName;
		public TextView textViewFileSize;
		public CheckBox checkbox;
	}
	
	public RecoveryFileAdapter(Context context, int textViewResourceId, List<RecoverFile> objects){
		super(context, textViewResourceId, objects);
		
		this.context = context;
		this.textViewResourceId = textViewResourceId;
		list = objects;
		isChecked = new boolean[objects.size()];
		checkedAll = false;
	}
	
	public void toggleCheckBox(int position){
		isChecked[position] = !isChecked[position];
	}
	
	public void toggleAllCheckBox(){
		int size = isChecked.length;
		if(checkedAll == false){
			for(int i=0; i<size; i++)
				isChecked[i] = true;
			checkedAll = true;
		}
		else{
			for(int i=0; i<size; i++)
				isChecked[i] = false;
			checkedAll = false;
		}
		/*
		int size = isChecked.length;
		int count;
		for(count=0; count<size; count++){
			if(isChecked[count] == false)
				break;
		}
		
		if(count != size){
			for(int i=0; i<size; i++)
				isChecked[i] = true;
		}
		else{
			for(int i=0; i<size; i++)
				isChecked[i] = false;
		}
		*/
	}
	
	public boolean getCheckedAll(){
		return checkedAll;
	}
	
	public boolean[] getIsChecked(){
		return isChecked;
	}
	
	public View getView(int position, View convertView, ViewGroup parent){
		final ViewHolder holder; 
		
		if(convertView == null || convertView.getId() != textViewResourceId){
			holder = new ViewHolder();

			LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			convertView = inflater.inflate(textViewResourceId, null, false);

			holder.imageViewPossibility = (ImageView) convertView.findViewById(R.id.possibilityImage);
			holder.imageViewFileImage = (ImageView) convertView.findViewById(R.id.listRecoveryImage);
			holder.textViewFileName = (TextView) convertView.findViewById(R.id.listRecoveryFileName);
			holder.textViewFileSize = (TextView) convertView.findViewById(R.id.listRecoveryFileSize);
			holder.checkbox = (CheckBox) convertView.findViewById(R.id.recoveryCheckBox);
			
			convertView.setTag(holder);
		}
		else{
			holder = (ViewHolder)convertView.getTag();
		}
		
		final RecoverFile rFile = list.get(position);
		
		if(rFile != null){
			holder.textViewFileName.setText(rFile.getFileName());
			
			//file size 단위 설정
			long fileSize = 0;
			String unit = "Bytes";
			fileSize = rFile.getSize();
			if(fileSize >= 1024){
				fileSize /= 1024;
				unit = "KB";
			}
			if(fileSize >= 1024){
				fileSize /= 1024;
				unit = "MB";
			}
			holder.textViewFileSize.setText(fileSize + unit);
			
			// 복구 가능성에 따른 icon image 설정
			if(rFile.getExpectedPossibility()<60)
				holder.imageViewPossibility.setImageResource(R.drawable.icon_possibility_low);
			else if(rFile.getExpectedPossibility()<80)
				holder.imageViewPossibility.setImageResource(R.drawable.icon_possibility_mid);
			else
				holder.imageViewPossibility.setImageResource(R.drawable.icon_possibility_high);
			
			Log.i("ctrlz", Integer.toString(rFile.getExpectedPossibility()));
			
			// 확장자에 따른 icon image 설정
			holder.imageViewFileImage.setImageResource(R.drawable.icon_plain);
			
			String ext = rFile.getExtender().toLowerCase();
			
			if( rFile.isThumnailGen() == true ){
				BitmapFactory.Options options = new BitmapFactory.Options();
				options.inSampleSize = 8;
				Bitmap mPhotoBitmap = BitmapFactory.decodeFile(rFile.getUrl(), options);

				holder.imageViewFileImage.setImageBitmap(mPhotoBitmap);

			} 
			else if( ext.equals("txt") || ext.equals("doc") ){
				holder.imageViewFileImage.setImageResource(R.drawable.icon_text);
			} 
			else {
				holder.imageViewFileImage.setImageResource(R.drawable.icon_plain);
			}
		}
		
		holder.checkbox.setFocusable(false);
		holder.checkbox.setClickable(false);
		holder.checkbox.setChecked(isChecked[position]);
		
		return convertView;
	}
	
}