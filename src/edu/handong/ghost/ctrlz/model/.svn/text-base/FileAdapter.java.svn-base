package edu.handong.ghost.ctrlz.model;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import edu.handong.ghost.ctrlz.FileManageActivity;
import edu.handong.ghost.ctrlz.R;
import edu.handong.ghost.ctrlz.manager.FileManager;

/**
 * @FileName: FileAdapter.java
 * @Date	: Jan 2, 2013 
 * @작성자	: 임의상 
 * @변경이력	:
 * @수정자	: 
 * @설명		: 
 */
public class FileAdapter extends ArrayAdapter<File> {
	private Context context;
	private int textViewResourceId;
	private List<File> list;
	private FileManager fileManager = FileManager.getInstance();	// 파일매니저 

	private static final int MOVEREQ = 1;
	private static final int COPYREQ = 2;
	private class ViewHolder {
		public ImageView imageViewFileImage;
		public TextView textViewFileName;
		public TextView textViewFileInfo;
		public ImageButton buttonExplorerOption;
	}

	public FileAdapter(Context context, int textViewResourceId, List<File> objects) {
		super(context, textViewResourceId, objects);
		// TODO Auto-generated constructor stub
		this.context = context;
		this.textViewResourceId = textViewResourceId;
		list = objects;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		ViewHolder holder = null;

		if(convertView == null || convertView.getId() != textViewResourceId ) {
			holder = new ViewHolder();

			LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			convertView = inflater.inflate(textViewResourceId, null);

			holder.imageViewFileImage = (ImageView) convertView.findViewById(R.id.imageViewFileImage);
			holder.textViewFileName = (TextView) convertView.findViewById(R.id.textViewFileName);
			holder.textViewFileInfo = (TextView) convertView.findViewById(R.id.textViewFileInfo);
			holder.buttonExplorerOption = (ImageButton)convertView.findViewById(R.id.buttonExplorerOption);

			convertView.setTag(holder);

		} else {
			holder = (ViewHolder) convertView.getTag(); 
		}

		final File file = list.get(position);

		if (file != null) {

			holder.textViewFileName.setText(file.getName());

			if (holder.buttonExplorerOption != null) {
				holder.buttonExplorerOption.setVisibility(View.INVISIBLE);
			}


			long fileLength = 0;

			// 함수로 뺄 것 
			long lastModified = file.lastModified();
			String unit = "Bytes";
			Date date = new Date(lastModified);
			SimpleDateFormat simpleDate = new SimpleDateFormat("yyyy.MM.dd HH:mm");

			if(!file.isDirectory()) {

				if( holder.textViewFileInfo != null ) {
					fileLength = file.length();

					if(fileLength >= 1024) {
						fileLength /= 1024;
						unit = "KB";
					} 
					if(fileLength >= 1024) {
						fileLength /= 1024;
						unit = "MB";
					}
					holder.textViewFileInfo.setText(simpleDate.format(date) + " | " + fileLength+unit);
				}

				holder.imageViewFileImage.setImageResource(R.drawable.icon_plain);

				// 임시 -> 확장자 체크 
				int pos = file.getName().lastIndexOf( "." );
				String ext = file.getName().substring( pos + 1 ).toLowerCase();

				if( ext.equals("jpg") || ext.equals("jpeg") || ext.equals("png") ){

					BitmapFactory.Options options = new BitmapFactory.Options();
					options.inSampleSize = 8;
					Bitmap mPhotoBitmap = BitmapFactory.decodeFile(file.getPath(), options);
					//Bitmap resized = Bitmap.createScaledBitmap(src, 100, 100, true);

					holder.imageViewFileImage.setImageBitmap(mPhotoBitmap);

				} else if( ext.equals("txt") || ext.equals("doc") ){
					holder.imageViewFileImage.setImageResource(R.drawable.icon_text);
				} else {
					holder.imageViewFileImage.setImageResource(R.drawable.icon_plain);
				}



			} else {
				if( holder.textViewFileInfo != null ) {
					holder.textViewFileInfo.setText(simpleDate.format(date));
				}
			}



		}

		return convertView;

	}

}
