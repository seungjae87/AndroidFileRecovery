package edu.handong.ghost.ctrlz.model;

import java.text.DecimalFormat;
import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import edu.handong.ghost.ctrlz.R;
import edu.handong.ghost.ctrlz.recovery.obj.Device;

/**
 * @FileName: DeviceAdapter.java
 * @Date	: Jan 17, 2013 
 * @작성자	: 임의상
 * @변경이력	:
 * @수정자	: 
 * @설명		: Device 정볼르 불러오기 위한 Adapter  
 */
public class DeviceAdapter extends ArrayAdapter<Device> {
	private Context context;
	private int textViewResourceId;
	private List<Device> list;

	private class ViewHolder {
		public ImageView imageViewDeviceImage;
		public TextView textViewDevicePath;
		public TextView textViewDeviceType;
		public TextView textViewDeviceAvailableSize;
		public TextView textViewDeviceFreeSize;
		public TextView textViewDeviceTotalSize;
		public ProgressBar progressBarSize;
	}

	public DeviceAdapter(Context context, int textViewResourceId, List<Device> objects) {
		super(context, textViewResourceId, objects);
		// TODO Auto-generated constructor stub
		this.context = context;
		this.textViewResourceId = textViewResourceId;
		list = objects;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {

		ViewHolder holder = null;
		String unit = "Bytes";

		if(convertView == null || convertView.getId() != textViewResourceId ) {
			holder = new ViewHolder();

			LayoutInflater inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			convertView = inflater.inflate(textViewResourceId, null);

			holder.imageViewDeviceImage = (ImageView) convertView.findViewById(R.id.imageViewDeviceImage);
			holder.textViewDevicePath = (TextView) convertView.findViewById(R.id.textViewDevicePath);
			holder.textViewDeviceType = (TextView) convertView.findViewById(R.id.textViewDeviceType);
			holder.textViewDeviceAvailableSize = (TextView) convertView.findViewById(R.id.textViewDeviceAvailableSize);
			holder.textViewDeviceFreeSize = (TextView) convertView.findViewById(R.id.textViewDeviceFreeSize);
			holder.textViewDeviceTotalSize = (TextView) convertView.findViewById(R.id.textViewDeviceTotalSize);
			holder.progressBarSize = (ProgressBar) convertView.findViewById(R.id.progressBarSize);

			convertView.setTag(holder);

		} else {
			holder = (ViewHolder) convertView.getTag(); 
		}

		Device device = list.get(position);

		if (device != null) {

			double freeSize = device.getFreeSize();
			double totalSize = device.getTotalSize();
			
			holder.textViewDevicePath.setText(device.getMountDir());
			holder.textViewDeviceType.setText(device.getFsType() +" "+ context.getResources().getString(R.string.device_adapter_filesystem));
			holder.textViewDeviceAvailableSize.setText(getUnitPrefix(totalSize-freeSize));
			holder.textViewDeviceFreeSize.setText(getUnitPrefix(freeSize));
			holder.textViewDeviceTotalSize.setText(context.getResources().getString(R.string.device_adapter_totalsize)+ ": " + getUnitPrefix(totalSize));
			
			holder.progressBarSize.setProgress((int)(100*(totalSize-freeSize)/totalSize));
		}

		return convertView;

	}

	private String getUnitPrefix(double size) {
		double BASE = 1024, KB = BASE, MB = KB*BASE, GB = MB*BASE;
		DecimalFormat df = new DecimalFormat("#.##");
		
		if(size >= GB) {
			return df.format(size/GB) + " GB";
		} 
		if(size >= MB) {
			return df.format(size/MB) + " MB";
		}
		if(size >= KB) {
			return df.format(size/KB) + " KB";
		}
		return "" + size + " bytes";
	}
}
