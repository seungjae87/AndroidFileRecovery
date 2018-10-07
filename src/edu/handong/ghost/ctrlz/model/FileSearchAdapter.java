package edu.handong.ghost.ctrlz.model;

import java.io.File;
import java.util.List;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import edu.handong.ghost.ctrlz.R;

public class FileSearchAdapter extends ArrayAdapter<File>{
	private Context context;
	private int textViewResourceId;
	private List<File> list;
	
	private class ViewHolder{
		public ImageView imagmeViewSearchedFileImage;
		public TextView textViewSearchedFileName;
		public TextView textViewSearchedFilePath;
	}
	
	public FileSearchAdapter(Context context,
			int textViewResourceId, List<File> objects) {
		super(context, textViewResourceId, objects);
		// TODO Auto-generated constructor stub
		
		this.context = context;
		this.textViewResourceId = textViewResourceId;
		list = objects;
		
		
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		ViewHolder holder = null;
		
		if(convertView == null){
			holder = new ViewHolder();
			
			LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			convertView = inflater.inflate(textViewResourceId, null);
			
			holder.imagmeViewSearchedFileImage = (ImageView)convertView.findViewById(R.id.imageViewSearchedFileImage);
			holder.textViewSearchedFileName = (TextView)convertView.findViewById(R.id.textViewSearchedFileName);
			holder.textViewSearchedFilePath = (TextView)convertView.findViewById(R.id.textViewSearchedFilePath);
			
			convertView.setTag(holder);
		}else{
			holder = (ViewHolder)convertView.getTag();
		}
		
		final int f_position = position;
		File file = list.get(f_position);
		
		holder.textViewSearchedFileName.setText(file.getName());
		
		if(file.isDirectory()){
			holder.textViewSearchedFilePath.setText(file.getParent());
			holder.imagmeViewSearchedFileImage.setImageResource(R.drawable.icon_folder);
		}else{
			String fileName = file.getName();
			String ext = fileName.substring(fileName.lastIndexOf(".")+1, fileName.length()).toLowerCase();
			
			holder.textViewSearchedFilePath.setText(file.getParent());
			
			if(ext.equals("jpg") || ext.equals("jpeg") || ext.equals("png")){
				BitmapFactory.Options options = new BitmapFactory.Options();
				options.inSampleSize = 8;
				Bitmap mPhotoBitmap = BitmapFactory.decodeFile(file.getPath(), options);
				holder.imagmeViewSearchedFileImage.setImageBitmap(mPhotoBitmap);
			}else if(ext.equals("txt") || ext.equals("doc")){
				holder.imagmeViewSearchedFileImage.setImageResource(R.drawable.icon_text);
			}else{
				holder.imagmeViewSearchedFileImage.setImageResource(R.drawable.icon_plain);
			}
			
		}
		
		return convertView;
		
	}
}
